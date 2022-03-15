#include <ros/ros.h>
#include "geometry_msgs/Twist.h"



int main(int argc,char** argv)
{
    ros::init(argc, argv, "turtle_circle");
    
    ros::NodeHandle nh;
    ros::Publisher cmd_vel = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",10);

    geometry_msgs::Twist twist;
    twist.linear.x = 5;

    //现在让乌龟绕着z轴向左旋转，以2pi/s的速度
    twist.angular.z = 2;

    ros::Rate rate(10);

    ROS_INFO("\ntwist.linear.x = %f;\n twist.angular.z = %f;\n",twist.linear.x,twist.angular.z);
    while (ros::ok())
    {
        cmd_vel.publish(twist);
        rate.sleep();
        ros::spinOnce();
    }
    return 0;
    
}

