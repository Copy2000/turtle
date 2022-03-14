#include "ros/ros.h"
#include "tf2_ros/transform_listener.h"
#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/Twist.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "math.h"

int main(int argc,char** argv)
{
    setlocale(LC_ALL,"");
    ros::init(argc, argv, "frameTransform");
    //创建listener的buffer
    tf2_ros::Buffer buffer;
    tf2_ros::TransformListener listener(buffer);
    //创建cmd）vel的publisher
    geometry_msgs::Twist pub_vel;
    ros::NodeHandle nh("turtle2");
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("cmd_vel",10);

    ros::Rate rate(1);
    while (ros::ok())
    {
        /* code for loop body */
        try
        {

            geometry_msgs::TransformStamped Turtle1ToTurtle2 = buffer.lookupTransform("Turtle2Frame","Turtle1Frame",ros::Time(0));
            //pub_vel.angular.z = std::atan2(Turtle1ToTurtle2.transform.rotation.y,Turtle1ToTurtle2.transform.rotation.x);
            // pub_vel.angular.z = Turtle1ToTurtle2.transform.rotation.z;
            // pub_vel.angular.x = Turtle1ToTurtle2.transform.rotation.x;
            // pub_vel.angular.y = Turtle1ToTurtle2.transform.rotation.y;
            pub_vel.linear.x = Turtle1ToTurtle2.transform.translation.x;
            pub_vel.linear.y = Turtle1ToTurtle2.transform.translation.y;
        }
        catch (const std::exception& e)
        {
            /* code for Catch */
            std::cerr<<e.what()<<'\n';
        }
        pub.publish(pub_vel);
        rate.sleep();
        //注意这里没有ros::spin()   
    }

}