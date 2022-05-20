import rclpy

from rclpy.node import Node
from rpi_lcd import LCD

from std_msgs.msg import String


class LCDSubscriber(Node):
    def __init__(self):
        super().__init__("lcd_subscriber")
        self.subscription = self.create_subscription(
            String, "lcd/lcd", self.listener_callback, 10
        )

        # Create LCD object
        self.lcd = LCD()

    def listener_callback(self, msg):
        self.get_logger().info(f"Got: {msg.data}")

        # String truncate here

        self.lcd.text(str(msg.data), 1)


def main(args=None):
    rclpy.init(args=args)

    lcd_subscriber = LCDSubscriber()

    rclpy.spin(lcd_subscriber)

    # Destroy the node explicitly
    lcd_subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
