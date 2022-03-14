#include <ros/ros.h>
#include "geometry_msgs/Twist.h"



int main(int argc,char** argv)
{
    ros::init(argc, argv, "turtle_circle");
    
    ros::NodeHandle nh;
    ros::Publisher cmd_vel = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",10);

    geometry_msgs::Twist twist;
    twist.linear.x = 1;

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

// int main(int argc,char** argv)
// {
//     ros::init(argc,argv,"init");
//     ros::NodeHandle nh;
//     ros::Publisher Pub = nh.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel",10);
    
// // 初始化geometry_msgs::Twist类型的消息    
//     geometry_msgs::Twist vel_msg;
//     vel_msg.linear.x=0.2;
//     vel_msg.angular.z=0.2;
    
// // 设置循环的频率
//     ros::Rate loop_rate(10);
    
//     while(ros::ok)
//     {
        
//         Pub.publish(vel_msg);
        
//         ROS_INFO("Publsh velocity command[%0.2f m/s, %0.2f rad/s]", 
// 				vel_msg.linear.x, vel_msg.angular.z);
        
//         loop_rate.sleep();
//         ros::spinOnce();
//     }
//     return 0;
// }