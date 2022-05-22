import rclpy

from rclpy.node import Node
from rpi_lcd import LCD

from lcd_interfaces.msg import Stat


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
        timer_period = 8  # Seconds to display a particular stat
        self.timer = self.create_timer(timer_period, self.iter_lcd)
        self.stat_idx = 0  # Current stat index
        self.stat = []  # Currently displayed stat

        # Create LCD object
        self.lcd = LCD(width=self.lcd_width, rows=self.lcd_height)

        self.get_logger().info(f"LCD Subscriber created.")

        # Inital callback
        self.iter_lcd()

    def listener_callback(self, msg):
        self.get_logger().debug(f"Got: {msg.value} for stat {msg.key}")

        self.statdict[str(msg.key)] = str(msg.value)
        self.get_logger().debug(f"Holding {len(self.statdict)} stats.")

        # Special case where if the value on screen is being displayed, we can 'live update' the value
        try:
            if str(msg.key) == self.stat[0]:
                self.lcd.text(msg.value, 2)  # Value
        except IndexError:
            pass

    def iter_lcd(self):
        try:
            self.stat = list(self.statdict.items())[self.stat_idx]

            self.get_logger().debug(
                f"Updating stat {self.stat[0]} with value {self.stat[1]} at index {self.stat_idx}."
            )

            self.lcd.text(self.stat[0], 1)  # key
            self.lcd.text(self.stat[1], 2)  # Value

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
