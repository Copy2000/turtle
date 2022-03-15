#include "ros/ros.h"
#include "turtlesim/Spawn.h"

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "addturtle");
    ros::NodeHandle nh;
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
    return 0;
}