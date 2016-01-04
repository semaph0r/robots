#include "ros/ros.h"
#include "beginner_tutorials/Num.h"

void oddNumsCallback(const beginner_tutorials::Num::ConstPtr& msg)
{
  ROS_INFO("Odd number: [%i]", (int)msg->num);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "listener");

  ros::NodeHandle n;
  ros::Subscriber oddNum_sub = n.subscribe("oddNums", 1000, oddNumsCallback);

  ros::spin();

  return 0;
}
