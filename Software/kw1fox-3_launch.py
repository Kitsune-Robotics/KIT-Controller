from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription(
        [
            Node(
                package="lcd_viewer",
                namespace="lcd_viewer",
                executable="test",
                name="lcd_test",
            ),
            Node(
                package="lcd_viewer",
                namespace="lcd_viewer",
                executable="subscriber",
                name="lcd_subscriber",
            ),
        ]
    )
