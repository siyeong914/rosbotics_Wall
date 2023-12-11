 void subscribe_scan(const sensor_msgs::msg::LaserScan::SharedPtr scan)
  {
    geometry_msgs::msg::Twist vel;
    
    vel.linear.x = 2.0;
    vel.angular.z = 0.;
    
    if (scan->ranges[0] <0.25 || scan->ranges[10] < 0.25)
    {
      vel.linear.x = 0.;
      vel.angular.z = -2.5;
    }
   
    else
    {
      vel.linear.x = 0.15;
      vel.angular.z = 0.;
    }
    RCLCPP_INFO(rclcpp::get_logger("self_drive"),
                "step=%d, range=%1.2f, linear=%1.2f, angular=%1.2f", step_, scan->ranges[0],
                vel.linear.x, vel.angular.z);
    pose_pub_->publish(vel);
    step_++;
  }
};
