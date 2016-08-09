 #include "ros/ros.h"
 #include "nav_msgs/Odometry.h"
 #include <std_msgs/Float64.h>
 #include <std_msgs/String.h>
 #include "stdio.h"
 #include "geometry_msgs/Point.h"
 #include "geometry_msgs/TwistWithCovariance.h"
 #include <cstdlib>
 #include <std_msgs/Float64MultiArray.h>
 #include <iostream>
 #include <math.h>
 #include <vector>

 using namespace std;
 
 struct vect
 {
 	float a;
 	float b;
 	float c;
 	float d;
 	float e;
 	float f;
 };

 struct point
 {
 	float x;
 	float y;
 	float z;
 	float tx;
 	float ty;
 	float tz;
 };

 ros::Publisher pub;
 std_msgs::Float64MultiArray msg;
 vect sum = {0, 0, 0, 0, 0, 0};
 point station_p = {0, 0, 0, 0, 0, 0};
 point current_p = {0, 0, 0, 0, 0, 0};
 int i = 0;

 void odomCallback(const nav_msgs::Odometry::ConstPtr& slam_msg)
	{
  	 current_p = {slam_msg->pose.pose.position.x, slam_msg->pose.pose.position.y, slam_msg->pose.pose.position.z,
  	 				slam_msg->twist.twist.linear.x, slam_msg->twist.twist.linear.y, slam_msg->twist.twist.linear.x};
	}

 void orderCallback(const std_msgs::String::ConstPtr& msg_order)
 {
 	if(msg_order->data == "stop"){
 		
 		if(i == 0){ 
 			station_p = current_p;
 		}

 		msg.data.resize(3);
 		msg.data[0] = station_p.x - current_p.x;
 		msg.data[1] = station_p.y - current_p.y;
 		msg.data[2] = station_p.z - current_p.z;
 		
 		pub.publish(msg);
 		ros::spin();

 		i++;
 	}
 	if(msg_order->data == "move"){
 		i = 0;
 	}
 }
 
 int main(int argc, char **argv)
 {

   ros::init(argc, argv, "listener");

   ros::NodeHandle n;
   pub = n.advertise<std_msgs::Float64MultiArray>("station_point", 1000);
   ros::Subscriber sub = n.subscribe("chatter", 1000, orderCallback);
   ros::Subscriber sub1 = n.subscribe("/rtabmap/odom", 1000, odomCallback);
   ros::spin();
 
   return 0;
  }