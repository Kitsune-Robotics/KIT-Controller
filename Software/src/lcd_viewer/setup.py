from setuptools import setup

package_name = "lcd_viewer"

setup(
    name=package_name,
    version="0.0.0",
    packages=[package_name],
    data_files=[
        ("share/ament_index/resource_index/packages", ["resource/" + package_name]),
        ("share/" + package_name, ["package.xml"]),
    ],
    install_requires=["setuptools", "smbus", "rpi-lcd"],
    zip_safe=True,
    maintainer="Joe",
    maintainer_email="kenwood364@gmail.com",
    description="Manages Displaying and Cycling information on a 20x2 LCD display.",
    license="BSD",
    tests_require=["pytest"],
    entry_points={
        "console_scripts": [
            "test = lcd_viewer.publish_lcd_test:main",
            "subscriber = lcd_viewer.lcd_subscriber:main",
        ],
    },
)
