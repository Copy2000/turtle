#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>
#include "math.h"
#include <vector>

#define error 0.1       //距离误差
#define Kp 2
#define Ki 0.5
#define Kd 0.5

//pid控制
/*
args:   distance 位置向量distance[0]是最新位置error[0],distance[3]是error[k-2]
*/
float vecl_coefficient(std::vector<float>& distance)
{   
    float coefficient;
    if(distance[0] > error)
    {
        coefficient = Kp*(distance[0]-distance[1])+Ki*distance[0]+Kd*(distance[0]-2*distance[1]+distance[2]);
    }
    return coefficient;
}

//更新位置
/*
args:   distance    位置向量distance[0]是最新位置error[0],distance[3]是error[k-2]
        flag        标志位，请结合main函数查看
*/
void update_distance(std::vector<float>& distance,int flag)
{
    if(flag == 0)
        return;
    if(flag == 1)
        distance[1] = distance[0];
    else if(flag >= 2)
    {
        distance[2] = distance[1];
        distance[1] = distance[0];    
    }
}

int main(int argc, char** argv)
{
    // 初始化节点
    ros::init(argc, argv, "my_tf_listener");
 
    ros::NodeHandle node;
 
    // 通过服务调用，产生第二只乌龟turtle2
    ros::service::waitForService("spawn");
    ros::ServiceClient add_turtle =
    node.serviceClient<turtlesim::Spawn>("spawn");
    turtlesim::Spawn srv;
    add_turtle.call(srv);
 
    // 定义turtle2的速度控制发布器
    ros::Publisher turtle_vel =
    node.advertise<geometry_msgs::Twist>("turtle2/cmd_vel", 10);
 
    // tf监听器
    tf::TransformListener listener;
    
    std::vector<float> distance(3);
    int flag = 0;

    ros::Rate rate(10.0);
    while (node.ok())
    {
        tf::StampedTransform transform;
        try
        {
            // 查找turtle2与turtle1的坐标变换
            listener.waitForTransform("/turtle2", "/turtle1", ros::Time(0), ros::Duration(3.0));
            listener.lookupTransform("/turtle2", "/turtle1", ros::Time(0), transform);
        }
        catch (tf::TransformException &ex) 
        {
            ROS_ERROR("%s",ex.what());
            ros::Duration(1.0).sleep();
            continue;
        }
 
        // 根据turtle1和turtle2之间的坐标变换，计算turtle2需要运动的线速度和角速度
        // 并发布速度控制指令，使turtle2向turtle1移动
        geometry_msgs::Twist vel_msg;
        distance[0] = sqrt(pow(transform.getOrigin().x(), 2) + pow(transform.getOrigin().y(), 2));
        vel_msg.angular.z = 4.0 * atan2(transform.getOrigin().y(),
                                        transform.getOrigin().x());
        /*速度控制二选一*/

        //  1   直接给定系数0.5                                
        // vel_msg.linear.x = 0.5 * sqrt(pow(transform.getOrigin().x(), 2) +
        //                              pow(transform.getOrigin().y(), 2));

        //  2   PID控制给定系数
        float coefficient=0;
        coefficient = vecl_coefficient(distance);
        vel_msg.linear.x = coefficient * distance[0];
        flag++;
        update_distance(distance,flag);

        /*速度控制二选一*/

        ROS_INFO("[velocity is %f;distance is %f",vel_msg.linear.x,distance[0]);
        turtle_vel.publish(vel_msg);
        rate.sleep();
    }
    return 0;
};
