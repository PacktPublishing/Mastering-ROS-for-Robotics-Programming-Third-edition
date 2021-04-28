#include "ros/ros.h"
#include "geometry_msgs/PoseStamped.h"
#include "mavros_msgs/State.h"
#include "mavros_msgs/CommandBool.h"
#include "mavros_msgs/SetMode.h"
#include "mavros_msgs/CommandTOL.h"

using namespace std;


mavros_msgs::State mav_state;


void mavros_state_cb( mavros_msgs::State mstate) {
  mav_state = mstate;
}


int main(int argc, char** argv ) {
  
  ros::init(argc, argv, "px4_ctrl_example");

  ros::NodeHandle nh;
	
	ros::ServiceClient 	arming_client = 		nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
	ros::ServiceClient 	set_mode_client = 	nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");
	ros::Publisher 			local_pos_pub = 		nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local", 1);
	ros::ServiceClient 	land_client = 			nh.serviceClient<mavros_msgs::CommandTOL>("/mavros/cmd/land");
	ros::Subscriber 		mavros_state_sub = 	nh.subscribe( "/mavros/state", 1, mavros_state_cb);

  mavros_msgs::CommandBool arm_cmd;
  arm_cmd.request.value = true;

  mavros_msgs::SetMode offb_set_mode;
  offb_set_mode.request.custom_mode = "OFFBOARD";

  if( set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent){
    ROS_INFO("Manual mode enabled");
  }

  if( arming_client.call(arm_cmd) && arm_cmd.response.success){
		ROS_INFO("Ready to be armed");
  }

  while(!mav_state.armed ) {
  	usleep(0.1*1e6);
		ros::spinOnce();
	}
  ROS_INFO("Vehicle armed");

  if( set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent){
    ROS_INFO("Manual mode enabled");
  }

  geometry_msgs::PoseStamped pose;
  pose.pose.position.x = 1;
  pose.pose.position.y = 0;
  pose.pose.position.z = 2;

  //send a few setpoints before starting
  ros::Rate r(10);
  float t = 0.0;
  while( ros::ok() &&  (t < 20.0) ) {
    local_pos_pub.publish(pose);
		t += (1.0/10.0);
    r.sleep();
		ros::spinOnce();
  }


  mavros_msgs::CommandTOL land_srv;
  land_client.call( land_srv );


  return 0;
}
