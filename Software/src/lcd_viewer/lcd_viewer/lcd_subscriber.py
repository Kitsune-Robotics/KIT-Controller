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

        # Store all the keys we've heard since startup
        self.statdict = {}

        # Subscribe to a particular topic with a callback
        self.subscription = self.create_subscription(
            Stat, "lcd/lcd", self.listener_callback, 10
        )

        # Setup a timer to iterate through the stats
        timer_period = 6
        self.timer = self.create_timer(timer_period, self.iter_lcd)
        self.stat_idx = 0

        # Create LCD object
        self.lcd = LCD(width=self.lcd_width, rows=self.lcd_height)

        # Inital callback
        self.iter_lcd()

    def listener_callback(self, msg):
        self.get_logger().info(f"Got: {msg.value} for stat {msg.key}")

        self.statdict[str(msg.key)] = str(msg.value)
        self.get_logger().debug(f"Holding {len(self.statdict)} stats.")

        self.get_logger().debug(str(list(self.statdict.items())))

    def iter_lcd(self):
        try:
            stat = list(self.statdict.items())[self.stat_idx]

            self.get_logger().debug(
                f"Updating stat {stat[0]} with value {stat[1]} at index {self.stat_idx}."
            )

            self.lcd.text(stat[0], 1)  # key
            self.lcd.text(stat[1], 2)  # Value

            self.stat_idx = self.stat_idx + 1
            if self.stat_idx >= len(self.statdict):
                self.stat_idx = 0

        except IndexError:
            self.get_logger().warn(
                f"No elements in dict yet, at index {self.stat_idx}."
            )
            self.lcd.text("Waiting for", 1, "center")  # key
            self.lcd.text("Startup", 2, "center")  # Value


def main(args=None):
    rclpy.init(args=args)

    lcd_subscriber = LCDSubscriber()

    rclpy.spin(lcd_subscriber)

    # Destroy the node explicitly
    lcd_subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
