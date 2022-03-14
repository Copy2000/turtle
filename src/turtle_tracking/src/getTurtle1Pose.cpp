#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include "tf2_ros/static_transform_broadcaster.h"
#include "geometry_msgs/TransformStamped.h"
#include "tf2/LinearMath/Quaternion.h"

void SubCallBcak(const turtlesim::Pose::ConstPtr& TurtlePoseInfo)
{
    //static属性保证发布者对象保持不变
    static tf2_ros::StaticTransformBroadcaster pub;
    geometry_msgs::TransformStamped TransformInfo;
    tf2::Quaternion qtn;

    TransformInfo.child_frame_id = "Turtle1Frame";
    TransformInfo.header.frame_id = "world";
    TransformInfo.header.stamp = ros::Time::now();
    TransformInfo.transform.translation.x = TurtlePoseInfo->x;
    TransformInfo.transform.translation.y = TurtlePoseInfo->y;
    qtn.setRPY(0,0,TurtlePoseInfo->theta);
    TransformInfo.transform.rotation.w = qtn.getW();
    TransformInfo.transform.rotation.x = qtn.getX();
    TransformInfo.transform.rotation.y = qtn.getY();
    TransformInfo.transform.rotation.z = qtn.getZ();

    pub.sendTransform(TransformInfo);
}

int main(int argc,char** argv)
{
    setlocale(LC_ALL,"");
    ros::init(argc, argv, "getTurtle1Pose");
    ros::NodeHandle nh("turtle1");

    ros::Subscriber sub = nh.subscribe<turtlesim::Pose>("pose",10,boost::bind(SubCallBcak,_1));
    ros::spin();
    return 0;
}