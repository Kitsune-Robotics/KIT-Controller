from setuptools import setup

package_name = "pubsub"

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
    maintainer="KW1FOX",
    maintainer_email="kenwood364@gmail.com",
    description="Simple pubsub",
    license="BSD",
    tests_require=["pytest"],
    entry_points={
        "console_scripts": [
            "talker = pubsub.publisher_member_function:main",
        ],
    },
)
