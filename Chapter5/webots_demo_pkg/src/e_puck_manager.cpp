#include "ros/ros.h"

#include <webots_ros/Int32Stamped.h>

#include <webots_ros/set_float.h>
#include <webots_ros/set_int.h>

#include <webots_ros/robot_get_device_list.h>

#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>

#include <signal.h>
#include <stdio.h>

using namespace std;

static char modelList[10][100];
static int cnt = 0;

static float left_vel = 0.0;
static float right_vel = 0.0;

static float wheel_radius = 0.205;
static float axes_length = 0.52;

void cmdVelCallback(const geometry_msgs::Twist::ConstPtr &vel) {
	
	left_vel = ( 1/wheel_radius)*(vel->linear.x-axes_length/2*vel->angular.z);
	right_vel = ( 1/wheel_radius)*(vel->linear.x+axes_length/2*vel->angular.z);  

}

void modelNameCallback(const std_msgs::String::ConstPtr &name) {
  cnt++;
  strcpy(modelList[cnt], name->data.c_str());
  ROS_INFO("Model #%d: %s.", cnt, name->data.c_str());
}


int main(int argc, char** argv ) {

	ros::init(argc, argv, "e_puck_manager");
  ros::NodeHandle n;
 	std::string modelName;
  
  
  // get the name of the robot
  ros::Subscriber nameSub = n.subscribe("model_name", 100, modelNameCallback);
  while (cnt == 0 ) {
    ros::spinOnce();
  }
  modelName = modelList[1];
	
	ros::Subscriber cmdVelSub = n.subscribe("cmd_vel", 1, cmdVelCallback);
  
	

	// set the motors to veloctiy control
	webots_ros::set_float wheelSrv;
	wheelSrv.request.value = INFINITY;
	ros::ServiceClient leftWheelPositionClient =
	n.serviceClient<webots_ros::set_float>(modelName + "/left_wheel_motor/set_position");
	leftWheelPositionClient.call(wheelSrv);
	ros::ServiceClient rightWheelPositionClient =
	n.serviceClient<webots_ros::set_float>(modelName + "/right_wheel_motor/set_position");
	rightWheelPositionClient.call(wheelSrv);
	
	wheelSrv.request.value = 0.0;
	ros::ServiceClient leftWheelVelocityClient =
	n.serviceClient<webots_ros::set_float>(modelName + "/left_wheel_motor/set_velocity");
	leftWheelVelocityClient.call( wheelSrv );
	ros::ServiceClient rightWheelVelocityClient =
	n.serviceClient<webots_ros::set_float>(modelName + "/right_wheel_motor/set_velocity");
	rightWheelVelocityClient.call( wheelSrv );
	
	ros::Rate r(10);
	while(ros::ok()) {
		wheelSrv.request.value = left_vel;	
		leftWheelVelocityClient.call( wheelSrv );

		wheelSrv.request.value = right_vel;	
		rightWheelVelocityClient.call( wheelSrv );

		r.sleep();
		ros::spinOnce();
	}

	
	return 0;
}
