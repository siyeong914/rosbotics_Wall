void subscribe_scan(const sensor_msgs::msg::LaserScan::SharedPtr scan)
  {
    geometry_msgs::msg::Twist vel;

    vel.linear.x = 0.20;
    vel.angular.z = 0.;

    if (scan->ranges[0] <= 0.25 || scan->ranges[20] <= 0.25)
    {
      vel.linear.x = 0.;
      vel.angular.z = -1.6;
    }
   
    else if(scan->ranges[30] <= 0.25 || scan->ranges[50] <=0.25)
    {
      vel.linear.x = 0.15;
      vel.angular.z = -0.005;
    }
    else if(scan->ranges[60] <= 0.25 ||  scan->ranges[80] <= 0.25)
    {
      vel.linear.x = 0.15;
      vel.angular.z = 2.0;
    }
    else if(scan->ranges[90] > 0.25)
    {
      vel.linear.x = 0.05;
      vel.angular.z = 1.5;
    }
    else if(scan->ranges[100] <= 0.25 || scan->ranges[120] <= 0.25)
    {
      vel.linear.x = 0.1;
      vel.angular.z = 1.5;
    }
    else if(scan->ranges[130] <= 0.25 || scan->ranges[150] <= 0.25)
    {
      vel.linear.x = 0.1;
      vel.angular.z = 1.5;
    }
    else
    {
      vel.linear.x = 0.20;
      vel.angular.y = 0.;
    }
   
    RCLCPP_INFO(rclcpp::get_logger("self_drive"),
                "step=%d, range=%1.2f, linear=%1.2f, angular=%1.2f", step_, scan->ranges[0],
                vel.linear.x, vel.angular.z);
    pose_pub_->publish(vel);
    step_++;
  }
};
