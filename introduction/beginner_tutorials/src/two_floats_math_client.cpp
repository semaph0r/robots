#include "ros/ros.h"
#include "beginner_tutorials/TwoFloatsMath.h"
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "two_floats_math_client");
  if (argc != 4)
  {
    ROS_INFO("usage: add_two_ints_client float1 float2 operand");
    return 1;
  }

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<beginner_tutorials::TwoFloatsMath>("two_floats_math");
  beginner_tutorials::TwoFloatsMath srv;
  srv.request.a = atof(argv[1]);
  srv.request.b = atof(argv[2]);
  srv.request.operand = argv[3];
  if (client.call(srv))
  {
    ROS_INFO("Result: %f", (float)srv.response.result);
  }
  else
  {
    ROS_ERROR("Failed to call service two_floats_math");
    return 1;
  }

  return 0;
}