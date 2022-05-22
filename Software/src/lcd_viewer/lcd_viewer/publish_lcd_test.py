# Copyright 2016 Open Source Robotics Foundation, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import psutil
import random

import rclpy
from rclpy.node import Node

from lcd_interfaces.msg import Stat


class MinimalPublisher(Node):
    def __init__(self):
        super().__init__("lcd_test_publisher")
        self.stat_pub = self.create_publisher(Stat, "lcd/lcd", 10)

        # Update Voltage
        self.create_timer(2, self.update_voltage)

        # Update Temp
        self.create_timer(1, self.update_temp)

        self.get_logger().info(f"Test Publisher Created")

    def update_voltage(self):
        msg = Stat()
        msg.key = "Voltage"
        msg.value = f"{12.8 + random.random() - 0.5:.2f}"

        self.get_logger().debug(f"Publishing {msg.value} to {msg.key}.")
        self.stat_pub.publish(msg)

    def update_temp(self):
        msg = Stat()
        msg.key = "CPU Temp"
        msg.value = f"{psutil.sensors_temperatures()['cpu_thermal'][0].current:.2f}"

        self.get_logger().debug(f"Publishing {msg.value} to {msg.key}.")
        self.stat_pub.publish(msg)


def main(args=None):
    rclpy.init(args=args)

    minimal_publisher = MinimalPublisher()

    rclpy.spin(minimal_publisher)

    # Destroy the node explicitly
    # (optional - otherwise it will be done automatically
    # when the garbage collector destroys the node object)
    minimal_publisher.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
