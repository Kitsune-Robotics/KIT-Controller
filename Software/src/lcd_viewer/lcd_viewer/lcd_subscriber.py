import rclpy

from rclpy.node import Node
from rpi_lcd import LCD

from lcd_interfaces.msg import Stat
from textwrap import wrap


class LCDSubscriber(Node):
    def __init__(self):
        super().__init__("lcd_subscriber")

        # Consts
        self.lcd_width = 16
        self.lcd_height = 2

        # Subscribe to a particular topic with a callback
        self.subscription = self.create_subscription(
            Stat, "lcd/lcd", self.listener_callback, 10
        )

        # Create LCD object
        self.lcd = LCD()

    def listener_callback(self, msg):
        self.get_logger().info(f"Got: {msg.value}")

        self.lcd.text(str(msg.key), 1)
        self.lcd.text(str(msg.value), 2)


def main(args=None):
    rclpy.init(args=args)

    lcd_subscriber = LCDSubscriber()

    rclpy.spin(lcd_subscriber)

    # Destroy the node explicitly
    lcd_subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
