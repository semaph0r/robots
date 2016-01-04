
#include <stdlib.h>
#include <ctime>

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Pose.h>


float maxValue = 4.0f;
ros::Publisher drive_pub_tur;
bool avoid;
float theta;

void poseCallback(const turtlesim::Pose::ConstPtr& pose)
{
    if(pose->x <= 1.0 || pose->x >= 10.0 || pose->y <= 1.0 || pose->y >= 10.0)
    {
        avoid = true;
        theta = pose->theta; 
    }
    //ROS_INFO("[poseCallback]: [avoid] %i [theta] %f", avoid ,theta);
}

void calculateNextMove(geometry_msgs::Twist &drive)
{
    // calculate random drive values
    // TODO calculate random float number in the range from 0 to maxValue
    drive.linear.x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxValue));
    // TODO calculate random float number in the range from -maxValue to maxValue
    //drive.angular.z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxValue));
    drive.angular.z = ((float(rand()) / float(RAND_MAX)) * -2 * maxValue) + maxValue;
    ROS_INFO("Next move: [linear.x] %f [angular.z] %f", drive.linear.x, drive.angular.z);

}

void avoidCollision()
{
    geometry_msgs::Twist drive;

    ROS_WARN("Avoiding collision");
    // stop
    drive.linear.x = 0;
    drive.linear.y = 0;
    drive.linear.z = 0;

    drive.angular.x = 0;
    drive.angular.y = 0; //to be safe
    drive.angular.z = 0; //to be safe

    drive_pub_tur.publish(drive);

    // Drive backwards and turn to the left or right

    drive.linear.x = -0.5;
    drive_pub_tur.publish(drive);

    sleep(2);

    int direction = rand() % 2;

    drive.linear.x = 0.0;
    
    if(direction) 
    {
        drive.angular.z = -3.6;    
    }
    else
    {
    drive.angular.z = 3.6;
    }

    drive_pub_tur.publish(drive);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "robot_drive_square");
    ros::NodeHandle n;
 
    // subscribe to topics
    ros::Subscriber sub_tur = n.subscribe<turtlesim::Pose>("/turtle1/pose", 1, poseCallback);

    // advertise topics
    drive_pub_tur = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1);

    // init random number generator
    srand(time(NULL));

    avoid = false;
    int counter = 0;
	geometry_msgs::Twist drive;

    // call callback at 10 Hz
    ros::Rate loop_rate(10);
    while(ros::ok())
    {
        // publish message at 1 Hz
        if(counter % 10 == 0)
        {
            drive.linear.x = 0.0;
            drive.angular.z = 0.0;
            // calculate next move
            calculateNextMove(drive);
            drive_pub_tur.publish(drive);
        }

        // avoid collision if too close to border
        if (avoid) {
            avoidCollision();
            avoid = false;
        }

        loop_rate.sleep();
        ros::spinOnce();
        counter++;
    }

    return 0;
}