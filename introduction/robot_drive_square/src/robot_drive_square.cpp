
#include <ros/ros.h>

#include <nav_msgs/Odometry.h>

#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Quaternion.h>

#include <turtlesim/Pose.h>

#include <tf/transform_broadcaster.h>

geometry_msgs::Point start_position;
geometry_msgs::Quaternion start_orientation; // for the robot
float start_theta; // for the turtle
bool reset_start;
geometry_msgs::Point current_position;
geometry_msgs::Quaternion current_orientation; // for the robot
float current_theta; // for the turtle

enum state
{
    DRIVING,
    TURNING
} typedef state;

state robot_state;

int counter;

ros::Publisher drive_pub_tur;

// void odomCallback(const nav_msgs::Odometry::ConstPtr& odom)
// {
//     // reset start values for each side of the square
//     if(reset_start)
//     {
//         reset_start = false;
//         // TODO save odometry data in these variables
//         //start_position = 
//         //start_orientation = 
//     }

//     // save current odometry data
//     // TODO save odometry data in these variables
//     //current_position = 
//     //current_orientation =


//     // send tf info for /odom frame
//     static tf::TransformBroadcaster odom_broadcaster;
//     odom_broadcaster.sendTransform(tf::StampedTransform(
//             tf::Transform(
//                 tf::Quaternion(
//                     odom->pose.pose.orientation.x,
//                     odom->pose.pose.orientation.y,
//                     odom->pose.pose.orientation.z,
//                     odom->pose.pose.orientation.w),
//                 tf::Vector3(
//                     odom->pose.pose.position.x,
//                     odom->pose.pose.position.y,
//                     odom->pose.pose.position.z)),
//             odom->header.stamp, "/odom", "/base_link"));
// }

void poseCallback(const turtlesim::Pose::ConstPtr& odom)
{
    // reset start values for each side of the square
    if(reset_start)
    {
        // TODO save position data in these variables +
        reset_start = false;
        start_position.x = odom->x;
        start_position.y = odom->y;
        start_theta = odom->theta;
    }

    // save current position data
    // TODO save odometry data in these variables +
    current_position.x = odom->x;
    current_position.y = odom->y;
    current_theta = odom->theta;
}

void calculateNextMove(geometry_msgs::Twist &drive)
{
    // init drive message
    // TODO init drive message with zeros +
    drive.linear.x = 0;
    drive.linear.y = 0;
    drive.linear.z = 0;
    
    drive.angular.x = 0;
    drive.angular.y = 0;
    drive.angular.z = 0;

    // handle driving state
    if(robot_state == DRIVING)
    {
        // calculate travelled distance
        // TODO calculate travelled distance as the difference between start position and current position
        float distance = current_position.x - start_position.x;
        if(counter == 1)
            distance = current_position.y - start_position.y;
        else if(counter == 2)
            distance = start_position.x - current_position.x;
        else if(counter == 3)
            distance = start_position.y - current_position.y;

        ROS_INFO_STREAM("counter"  << counter);
        ROS_INFO_STREAM("x " << current_position.x << "y " << current_position.y);

        ROS_INFO_STREAM("state: driving, distance: " << distance);
        // TODO if one meter travelled, change state from DRIVING to TURNING, else set linear speed
        if(distance >= 0.999 || distance >= 1.0)
        {
            robot_state = TURNING;
        }
        else
        {
            drive.linear.x = 0.5;
        }

    }
    // handle turning state
    else if(robot_state == TURNING)
    {
        // calculate turning angle
        //float angle_rob = 2 * (acos(current_orientation.w) - acos(start_orientation.w));
        //float angle_tur = current_theta - 1.570796*(counter+1);
        if(counter == 4)
            return;

        float angle_tur = (start_theta + 1.570796) - current_theta;
        drive.angular.z = 1.0f;

        //ROS_INFO_STREAM("state: turning, angle robot: " << angle_rob);
        ROS_INFO_STREAM("state: turning, angle turtle: " << angle_tur);
        ROS_INFO_STREAM("current_theta: " << current_theta);
        // TODO robot: if already turned by 90 (or better 85 degrees), change state to DRIVING, increment counter and set reset_start, else turn the robot a little more
        // TODO turtle: if angle_tur is almost zero (e.g. only 2 degrees away from zero), change state to DRIVING, increment counter and set reset_start, else turn the turtle a little more
        if(angle_tur <= ((1.570796 * 2.0f / 90.0f)))
        {
            robot_state = DRIVING;
            counter++;
            reset_start = true;
            drive.angular.z = 0;
        }
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "robot_drive_square");
    ros::NodeHandle n;
 
    // subscribe to topics
    //// TODO robot: subscribe to topic /odom to receive the nav_msgs::Odometry message and call odomCallback

    // TODO turtle: subscribe to topic /turtle1/pose to receive the turtlesim::Pose message and call poseCallback +
    // advertise topics
    ros::Subscriber sub_tur = n.subscribe<turtlesim::Pose>("/turtle1/pose", 1, poseCallback);

    //// TODO robot: advertise the message geometry_msgs::Twist on topic /cmd_vel
    // TODO turtle: advertise the message geometry_msgs::Twist on topic /turtle1/cmd_vel +
    drive_pub_tur = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 1);

    // init start values
    reset_start = true;
    robot_state = DRIVING;

    // stop as soon as 4 is reached
    counter = 0;

    geometry_msgs::Twist drive;
    ros::Rate loop_rate(10);
    while(ros::ok())
    {
        // calculate next move
        calculateNextMove(drive);
        // publish message
        // TODO publish drive message +
        drive_pub_tur.publish(drive);

        loop_rate.sleep();
        // TODO make sure that callbacks are called +
        ros::spinOnce();
        // if all 4 square sides were passed, stop
        if(counter == 4) break;
    }

    return 0;
}


