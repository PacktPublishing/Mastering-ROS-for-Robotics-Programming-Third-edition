/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2013, Open Source Robotics Foundation
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the Open Source Robotics Foundation
 *     nor the names of its contributors may be
 *     used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/**
 *  \author Dave Coleman
 *  \desc   Example ROS plugin for Gazebo
 */

#include <gazebo_ros_hello_world/gazebo_ros_template.h>

#include <functional>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <ignition/math/Vector3.hh>

#include <ros/ros.h>

namespace gazebo
{
// Register this plugin with the simulator
GZ_REGISTER_MODEL_PLUGIN(GazeboRosTemplate);

////////////////////////////////////////////////////////////////////////////////
// Constructor
GazeboRosTemplate::GazeboRosTemplate(): nh_(NULL)
{
}

////////////////////////////////////////////////////////////////////////////////
// Destructor
GazeboRosTemplate::~GazeboRosTemplate() 
{
  if (nh_) {
    nh_->shutdown();
    delete nh_;
    nh_ = NULL;
  }
  
}

////////////////////////////////////////////////////////////////////////////////
// Load the controller
void GazeboRosTemplate::Load( physics::ModelPtr _parent, sdf::ElementPtr _sdf )
{
  // Make sure the ROS node for Gazebo has already been initalized
  if (!ros::isInitialized())
  {
    ROS_FATAL_STREAM_NAMED("template", "A ROS node for Gazebo has not been initialized, unable to load plugin. "
      << "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
    return;
    
  }
  else
  {
      ROS_WARN_STREAM("Gazebo ROS Hello World Plugin Initialized and Loaded");
      
      //this->updateConnection = event::Events::ConnectWorldUpdateBegin(
      //    std::bind(&GazeboRosTemplate::UpdateChild, this));
      
     robot_namespace_ = "/";
     if (_sdf->HasElement("robotNamespace")) {
    	robot_namespace_ = _sdf->GetElement("robotNamespace")->Get<std::string>();
  	}  

     // Create node handle
     nh_ = new ros::NodeHandle(robot_namespace_);

     pub_ = nh_->advertise<std_msgs::String>("gazebo_pub", 10);
     
     sub_ = nh_->subscribe("gazebo_sub", 10, &GazeboRosTemplate::chatterCallback,this);


     timer = nh_->createTimer(ros::Duration(0.1), &GazeboRosTemplate::UpdateChild,this);
  
  
  
  }
  

}

////////////////////////////////////////////////////////////////////////////////
// Update the controller
void GazeboRosTemplate::UpdateChild(const ros::TimerEvent&)
{

  	ROS_WARN_STREAM("Gazebo Controller Loop");
  	
  	std_msgs::String msg;
  	std::string msg_ = "Hello from Gazebo";
  	msg.data = msg_;
  	
  	pub_.publish(msg);

}

void GazeboRosTemplate::chatterCallback(std_msgs::String msg)
{
  ROS_INFO("I heard: [%s]", msg.data.c_str());
}


}
