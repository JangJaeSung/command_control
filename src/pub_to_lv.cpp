#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "ackermann_msgs/AckermannDriveStamped.h"

float speed;

void ackerCallback(const ackermann_msgs::AckermannDriveStamped::ConstPtr& msg){
	speed = msg->drive.speed;
	//ROS_INFO("%f", steer);
	speed = speed * 150;
}


int main(int argc, char **argv){
	ros::init(argc, argv, "leading_control");
	ros::NodeHandle nh_;

	ros::Subscriber acker_sub = nh_.subscribe("/ackermann_cmd", 1000, ackerCallback);

	ros::Publisher lv_left_pub = nh_.advertise<std_msgs::Float64>("/leading_vehicle/left_position_controller/command", 1000);
	ros::Publisher lv_right_pub = nh_.advertise<std_msgs::Float64>("/leading_vehicle/right_position_controller/command", 1000);

	ros::Publisher to_fv_pub = nh_.advertise<std_msgs::Float64>("vel_info_of_lv", 1000);
	
	ros::Rate loop_rate(10);

	while (ros::ok()){
		std_msgs::Float64 msg;

		msg.data = speed;
		//ROS_INFO("data : %f ", msg.data);

		lv_left_pub.publish(msg);
		lv_right_pub.publish(msg);

		to_fv_pub.publish(msg);

		ros::spinOnce();

		loop_rate.sleep();
	}
	return 0;
}
