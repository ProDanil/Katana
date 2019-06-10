#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

int main(int argc, char **argv)
{
	ros::init(argc, argv, "talker");
	
	ros::NodeHandle joint_state_sub_ = nh_.subscribe("/joint_states", 1, &CallBackFunctionForArm, this);
    scanSub = nh_.subscribe<sensor_msgs::LaserScan> ("/rrbot/laser/scan",10, &CallBackFunctionForLaser,this);
    {
		const size_t NUM_JOINTS = 5;
		trajectory_msgs::JointTrajectory trajectory;
		for (ros::Rate r = ros::Rate(10); !got_joint_state_; r.sleep())
		{
			ROS_DEBUG("waiting for joint state...");
			if (!ros::ok())
			exit(-1);
		}

 // First, the joint names, which apply to all waypoints
		trajectory.joint_names = joint_names_;
		trajectory.points.resize(2);

 // trajectory point:
		int ind = 0;
		trajectory.points[ind].time_from_start = ros::Duration(5 * ind);
		trajectory.points[ind].positions.resize(NUM_JOINTS);
		trajectory.points[ind].positions = current_joint_state_;
		ind++;

		trajectory.points[ind].time_from_start = ros::Duration(5 * ind);
		trajectory.points[ind].positions = pos;
 
		control_msgs::FollowJointTrajectoryGoal goal;
		goal.trajectory = trajectory;
 
// When to start the trajectory: 1s from now
		goal.trajectory.header.stamp = ros::Time::now() + ros::Duration(1.0);
		traj_client_.sendGoal(goal);
	}
	
	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
	
	ros::Rate loop_rate(10);
	
	return 0;
}
