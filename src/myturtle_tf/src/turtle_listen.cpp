#include "ros/ros.h"
#include "tf/transform_listener.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Spawn.h"

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "tf_listener");
    ros::NodeHandle nh("~");

    ros::service::waitForService("spawn");
    ros::ServiceClient add_turtle = nh.serviceClient<turtlesim::Spawn>("spawn");
    turtlesim::Spawn init_place;
    init_place.request.name = "turtle2";
    init_place.request.theta = 0;
    init_place.request.x = 1;
    init_place.request.y = 1;
    ros::service::waitForService("/spawn");
    if(add_turtle.call(init_place))
    {
        ROS_INFO("%s created successfully ...",init_place.response.name.c_str());
    }
    std::string newturtle;
    nh.getParam("newturtle",newturtle);
    ROS_INFO("newturtle is %s",newturtle.c_str());
    ros::Publisher newturtle_vel = nh.advertise<geometry_msgs::Twist>(newturtle+"/cmd_vel",10);

    tf::TransformListener listener;

    ros::Rate loop_rate(10);
    while (ros::ok())
    {
        tf::StampedTransform transform;
        loop_rate.sleep();
        try
        {
            //listener.waitForTransform(newturtle,"/turtle1",ros::Time(0),ros::Duration(3));
            listener.lookupTransform(newturtle,"/turtle1",ros::Time::now()-ros::Duration(0.1),transform);
        }
        catch (tf::TransformException &ex )
        {
            ROS_ERROR("%s",ex.what());
            ros::Duration(1).sleep();
            continue;
        }
        geometry_msgs::Twist vel_msg;
        //得到turtle1坐标系的原点在turtle2坐标系的位置
        //transform.getOrigin().x()
        //transform.getOrigin().y()
        //transform.getRotation().getW()
        //transform.getRotation().getX()
        vel_msg.angular.z = 4*atan2(transform.getOrigin().y(),transform.getOrigin().x());
        vel_msg.linear.x = 0.5*sqrt(pow(transform.getOrigin().x(),2)+pow(transform.getOrigin().y(),2));
        newturtle_vel.publish(vel_msg);
    }
    

    
    return 0;
}