#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Pose.h"
#include "turtlesim/Spawn.h"
#include "tf2_ros/static_transform_broadcaster.h"
#include "geometry_msgs/TransformStamped.h"
#include "tf2/LinearMath/Quaternion.h"

void SubCallBcak(const turtlesim::Pose::ConstPtr& TurtlePoseInfo)
{
    //static属性保证发布者对象保持不变
    static tf2_ros::StaticTransformBroadcaster pub;
    geometry_msgs::TransformStamped TransformInfo;
    tf2::Quaternion qtn;

    TransformInfo.child_frame_id = "Turtle2Frame";
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
    ros::init(argc, argv, "getTurtle2Pose");
    ros::NodeHandle nh("turtle2");

    //在GUI界面中生成turtle2
    ros::ServiceClient client = nh.serviceClient<turtlesim::Spawn>("/spawn");
    turtlesim::Spawn init_place;
    init_place.request.name = "turtle2";
    init_place.request.theta = 0;
    init_place.request.x = 1;
    init_place.request.y = 1;
    ros::service::waitForService("/spawn");
    if(client.call(init_place))
    {
        ROS_INFO("%s created successfully ...",init_place.response.name.c_str());
    }

    //获取turtle2的位姿
    ros::Subscriber sub = nh.subscribe<turtlesim::Pose>("pose",10,SubCallBcak);
    ros::spin();
    return 0;
}