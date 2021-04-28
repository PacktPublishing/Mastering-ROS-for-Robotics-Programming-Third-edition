#include "ros/ros.h"
#include "mav_msgs/Actuators.h"


using namespace std;

int main(int argc, char ** argv ) {

	ros::init(argc, argv, "motor_example");
	ros::NodeHandle nh;
	ros::Publisher actuators_pub;
	actuators_pub = nh.advertise<mav_msgs::Actuators>("/iris/gazebo/command/motor_speed", 1);

	ros::Rate r(10);

	mav_msgs::Actuators m;
	m.angular_velocities.resize(4);
	while(ros::ok()) {
		for(int i=0; i<4; i++) {
			for(int j=0; j<4; j++) {
				if( i!=j) m.angular_velocities[j] = 0.0;
				else m.angular_velocities[i] = 800;
			}	

			actuators_pub.publish(m);
			ros::spinOnce();
			sleep(1);

		}		
	}
	return 0;
}
