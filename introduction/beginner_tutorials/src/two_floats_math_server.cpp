#include <string>
#include "ros/ros.h"
#include "beginner_tutorials/TwoFloatsMath.h"

bool compute(beginner_tutorials::TwoFloatsMath::Request &req,
              beginner_tutorials::TwoFloatsMath::Response &res)
{
  std::string operand = req.operand;

  if(operand.compare("+") == 0)
  {
    res.result = (float)req.a + (float)req.b;
  }
  else if(operand.compare("-") == 0)
  {
    res.result = (float)req.a - (float)req.b;
  }
  else if(operand.compare("*") == 0)
  {
    res.result = (float)req.a * (float)req.b;
  }
  else if(operand.compare("/") == 0)
  {
    res.result = (float)req.a / (float)req.b;
  }
  else
  {
    ROS_INFO("Invalid operand! Only +,-,*,/ are permitted.");
  }

  ROS_INFO("request: a=%f, b=%f, operand=%s", (float)req.a, (float)req.b);
  ROS_INFO("sending back response: [%f]", (float)res.result);
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "two_float_math_server");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("two_floats_math", compute);

  ROS_INFO("Ready to compute two floats");
  ros::spin();

  return 0;
}