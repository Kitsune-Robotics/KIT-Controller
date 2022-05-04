# ROS2 Setup on pi

## Build and run

### Deps

```shell
# This did not work for me, rosdep not found
rosdep install -i --from-path src --rosdistro galactic -y
```

### Build

```shell
colcon build --packages-select pubsub
```

### Run

```shell
# zsh did not work, bad substitution? Should make a bug report.
bash

# Had to set mode on setup.bash
chmod +x install/setup.bash

# Setup (space is important)
. install/setup.bash

# Run!
ros2 run pubsub talker
```
