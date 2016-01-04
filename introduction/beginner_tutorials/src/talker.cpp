#include "ros/ros.h"
#include "beginner_tutorials/Num.h"

#include <sstream>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;

  ros::Publisher num_pub = n.advertise<beginner_tutorials::Num>("oddNums", 1000);

  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok())
  {
    if(count%2)
    {
      beginner_tutorials::Num msg;
      msg.num = count;

      ROS_INFO("%i", (int)msg.num);

      num_pub.publish(msg);
    }

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }

  return 0;
}
