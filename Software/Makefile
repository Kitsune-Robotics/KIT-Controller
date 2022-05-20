# Shortcuts and stuff idk

all: rosdep colcon

rosdep:
	rosdep install -i --from-path src --rosdistro galactic -y

colcon:
	colcon build --packages-select py_pubsub lcd_viewer
