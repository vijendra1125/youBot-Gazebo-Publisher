//Sample program for simulation of KUKA youBot in Gazebo using ROS

#include "ros/ros.h"
#include "boost/units/systems/si.hpp"
#include "boost/units/io.hpp"
#include "brics_actuator/JointPositions.h"
#include "geometry_msgs/Twist.h"
#include "trajectory_msgs/JointTrajectory.h"

ros::Publisher platformPublisher;
ros::Publisher armPublisher;
ros::Publisher gripperPublisher;

// create a message with the given joint position values
trajectory_msgs::JointTrajectory createArmPositionCommand(std::vector<double>& newPositions) 
{
	int numberOfJoints = 5;
	trajectory_msgs::JointTrajectory msg;

	trajectory_msgs::JointTrajectoryPoint point;

	for (int i = 0; i < 5; i++) { 
		point.positions.push_back(newPositions[i]);
		point.velocities.push_back(0);
		point.accelerations.push_back(0);
	}
	point.time_from_start = ros::Duration(0.5);
	msg.points.push_back(point);

	for (int i = 0; i < 5; i++) {
		std::stringstream jointName;
		jointName << "arm_joint_" << (i + 1);
		msg.joint_names.push_back(jointName.str());
	}
	
	msg.header.frame_id = "arm_link_0";
	msg.header.stamp = ros::Time::now();

	return msg;
}

// create a message for the gripper
trajectory_msgs::JointTrajectory createGripperPositionCommand(double newPosition) {
	
	trajectory_msgs::JointTrajectory msg;

	trajectory_msgs::JointTrajectoryPoint point;

	for (int i = 0; i < 2; i++) { 
		point.positions.push_back(newPosition);
		point.velocities.push_back(0);
		point.accelerations.push_back(0);
	}
	point.time_from_start = ros::Duration(0.5);
	msg.points.push_back(point);

	for (int i = 0; i < 2; i++) {
		std::stringstream jointName;
		if(i==0)
			jointName << "gripper_finger_joint_l";
		else
			jointName << "gripper_finger_joint_r";

		msg.joint_names.push_back(jointName.str());
	}
	
	// fill message header and sent it out
	msg.header.frame_id = "gripper_finger_joint_l";
	msg.header.stamp = ros::Time::now();

	return msg;
}


// move platform 
void movePlatform() {
	geometry_msgs::Twist twist;

	// forward
	twist.linear.x = 0.1;  
	platformPublisher.publish(twist);
	ros::Duration(3).sleep();

	// backward
	twist.linear.x = -0.1;
	platformPublisher.publish(twist);
	ros::Duration(3).sleep();

	// to the left
	twist.linear.x = 0;
	twist.linear.y = 0.1;
	platformPublisher.publish(twist);
	ros::Duration(3).sleep();

	// to the right
	twist.linear.y = -0.1;
	platformPublisher.publish(twist);
	ros::Duration(3).sleep();

	// stop
	twist.linear.y = 0;
	platformPublisher.publish(twist);
}

// move arm 
void moveArm() {
	trajectory_msgs::JointTrajectory msg;

	std::vector<double> jointvalues(5);

	// move arm straight up
	jointvalues[0] = 2.95;
	jointvalues[1] = 1.05;
	jointvalues[2] = -2.44;
	jointvalues[3] = 1.73;
	jointvalues[4] = 2.95;
	msg = createArmPositionCommand(jointvalues);
	armPublisher.publish(msg);
	ros::Duration(5).sleep();

	// move arm back
	jointvalues[0] = 0.11;
	jointvalues[1] = 0.11;
	jointvalues[2] = -0.11;
	jointvalues[3] = 0.11;
	jointvalues[4] = 0.111;
	msg = createArmPositionCommand(jointvalues);
	armPublisher.publish(msg);
	ros::Duration(5).sleep();
}

// open and close gripper
void moveGripper() {
	trajectory_msgs::JointTrajectory msg;	

	// open gripper
	msg = createGripperPositionCommand(0.011);
	gripperPublisher.publish(msg);

	ros::Duration(3).sleep();

	// close gripper
	msg = createGripperPositionCommand(0);
	gripperPublisher.publish(msg);
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "youbot_ros_hello_world");
	ros::NodeHandle n;

	platformPublisher = n.advertise<geometry_msgs::Twist>("cmd_vel", 1);
	armPublisher = n.advertise<trajectory_msgs::JointTrajectory>("arm_1/arm_controller/command", 1);
	gripperPublisher = n.advertise<trajectory_msgs::JointTrajectory>("arm_1/gripper_controller/command", 1);
	sleep(1);

	movePlatform();
	moveArm();
	moveGripper();

	sleep(1);
	ros::shutdown();

	return 0;
}
