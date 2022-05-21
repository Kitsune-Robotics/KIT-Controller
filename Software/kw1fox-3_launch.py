from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription(
        [
            Node(
                package="lcd_viewer",
                namespace="lcd_viewer",
                executable="talker",
                name="lcd_talker",
            ),
            Node(
                package="lcd_viewer",
                namespace="lcd_viewer",
                executable="listener",
                name="lcd_listener",
            ),
        ]
    )
