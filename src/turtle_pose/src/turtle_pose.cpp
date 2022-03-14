#include "ros/ros.h"
#include "turtlesim/Pose.h"

void poseCallBack(const turtlesim::Pose::ConstPtr& msg)
{
    ROS_INFO("turtle position is:\t x = %f;\t y = %f\n",msg->x,msg->y);
}


int main(int argc,char** argv)
{
    ros::init(argc, argv, "turtle_pose");
    ros::NodeHandle nh;
    ros::Subscriber pose_sub = nh.subscribe("/turtle1/pose",1,poseCallBack);



    ros::spin();
    return 0;
    
}