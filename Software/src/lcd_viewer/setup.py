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
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="joe",
    maintainer_email="kenwood364@gmail.com",
    description="TODO: Package description",
    license="TODO: License declaration",
    tests_require=["pytest"],
    entry_points={
        "console_scripts": [
            "talker = lcd_viewer.publish_lcd_test:main",
            "listener = lcd_viewer.lcd_subscriber:main",
        ],
    },
)
