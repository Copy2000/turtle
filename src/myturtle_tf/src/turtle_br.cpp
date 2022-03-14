#include "ros/ros.h"
#include "tf/transform_broadcaster.h"
#include "turtlesim/Pose.h"

std::string turtle_name;

void PoseCallBack(const turtlesim::PoseConstPtr& msg)
{
    //定义一个广播，相当于发布话题定义一个publisher
    static tf::TransformBroadcaster br;

    //根据当前turtle乌龟位姿，设置相对于世界坐标系world的tf：：transform类型的坐标变换
    //定义存放转换信息（平动、转动）的变量
    tf::Transform Transform;
    //设置坐标原点,msg->x,msg->y指的是子坐标系turtle1在父坐标系下的坐标
    Transform.setOrigin(tf::Vector3(msg->x,msg->y,0.0));
    //定义旋转
    tf::Quaternion q;   
    //setRPY函数的参数为turtle1在world坐标系下的roll（绕x轴），pitch（绕y轴），yaw（绕z轴）
    q.setRPY(0,0,msg->theta);
    Transform.setRotation(q);
    //转换为广播，如果是一个平面移动的机器人，只需要知道x，y和yaw可以得到位置
    /*Args:
    Transform:存储变换关系的变量
    ros::Time::now():广播tf的时间戳
    world：父坐标系的名字
    turtle_name:子坐标系的名字
    */
    br.sendTransform(tf::StampedTransform(Transform,ros::Time::now(),"world",turtle_name));
}

int main(int argc, char *argv[])
{
    /* code for main function */
    ros::init(argc, argv, "tf_br");
    ros::NodeHandle nh("~");
    nh.getParam("turtle_name",turtle_name);
    // if(argc != 2)
    // {
    //     ROS_ERROR("\t argument setting is wrong!!! \t");
    //     return -1;
    // }
    // turtle_name = argv[1];
    std::cout<<turtle_name<<std::endl;
    ros::Subscriber sub = nh.subscribe(turtle_name+"/pose",10,&PoseCallBack);

    ros::spin();

    return 0;
}