#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <nav_msgs/msg/odometry.hpp>

using namespace std::chrono_literals;

class SelfDrive : public rclcpp::Node
{
  rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr scan_sub_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr pose_pub_;
  int step_;

public:
  SelfDrive() : rclcpp::Node("self_drive"), step_(0)
  {
    auto lidar_qos_profile = rclcpp::QoS(rclcpp::KeepLast(1));
    lidar_qos_profile.reliability(rclcpp::ReliabilityPolicy::BestEffort);
    auto callback = std::bind(&SelfDrive::subscribe_scan, this, std::placeholders::_1);
    scan_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>("/scan", lidar_qos_profile,
                                                                       callback);
    auto vel_qos_profile = rclcpp::QoS(rclcpp::KeepLast(1));
    pose_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", vel_qos_profile);
  }

  void subscribe_scan(const sensor_msgs::msg::LaserScan::SharedPtr scan)
  {
    geometry_msgs::msg::Twist vel;

    vel.linear.x = 0.20;
    vel.angular.z = 0.;

    switch (detectObstacleType(scan))
    {
      case 1:
      vel.linear.x = 0.;
      vel.angular.z = -1.6;
      break;

      case 2:
      vel.linear.x = 0.15;
      vel.angular.z = -0.005;
      break;

      case 3:
      vel.linear.x = 0.15;
      vel.angular.z = 0.8;
      break;

      case 4:
      vel.linear.x = 0.1;
      vel.angular.z = 1.6;
      break;

      case 5:
       vel.linear.x = 0.20;
      vel.angular.z = 0.;
      break;

      default:
      vel.linear.x = 0.20;
      vel.angular.z = 0.;
    }

    RCLCPP_INFO(rclcpp::get_logger("self_drive"),
                "step=%d, range=%1.2f, linear=%1.2f, angular=%1.2f", step_, scan->ranges[0],
                vel.linear.x, vel.angular.z);
    pose_pub_->publish(vel);
    step_++;
  }

  int detectObstacleType(const sensor_msgs::msg::LaserScan::SharedPtr scan)
  {
    if ((scan->ranges[0] > 0.025 && scan->ranges[0] <= 0.235) || (scan->ranges[20] > 0.025 && scan->ranges[20] <= 0.235))
    {
      return 1;
    }
    else if((scan->ranges[30] > 0.035 && scan->ranges[30] <= 0.235) || (scan->ranges[50] > 0.035 && scan->ranges[50] <= 0.235))
    {
      return 2;
    }
    else if((scan->ranges[60] > 0.025 && scan->ranges[60] <= 0.235)|| (scan->ranges[80] > 0.025 && scan->ranges[80] <= 0.235))
    {
      return 3;
    }
    else if(scan->ranges[90] > 0.025 && scan->ranges[90] <= 0.185)
    {
       return 4;
    }
    else
    {
      return 5;
    }
  }
};

int main(int argc, char* argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<SelfDrive>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
