```python
import subprocess

def run_ros_cmd(cmd: str):
	PREFIX = "source /opt/ros/humble/setup.bash && "
	result = subprocess.run(PREFIX + cmd, 
		capture_output=True, text=True, 
         	executable="/bin/bash", shell=True)
	if result.returncode == 0:
		return result.stdout.strip()
	else:
		raise ValueError("Error running ROS command:", result.stderr)

def get_active_topics():
	return run_ros_cmd("ros2 topic list").splitlines()

def get_topic_type(topic):
	return run_ros_cmd("ros2 topic type " + topic)

def show_interface(interface_type):
	return run_ros_cmd("ros2 interface show --no-comments " + interface_type)

def main():
	topics = get_active_topics()
	unique_types = set()
	for topic in topics:
		topic_type = get_topic_type(topic)
		if topic_type:
			unique_types.add(topic_type)
	
	with open("interfaces.txt", "w") as f:
		for topic_type in unique_types:
			f.write("\ntype: " + topic_type + "\n")
			f.write("def: \n" + show_interface(topic_type) + "\n")
			f.write("-" * 80)

if __name__ == "__main__":
	main()
```