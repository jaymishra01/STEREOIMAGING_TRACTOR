https://navigation.ros.org/tutorials/docs/camera_calibration.html

ls /dev/video*

1st terminal
```bash
/home/ecsperception/ros2_ws/src/tafe_sensors/scripts/left_cam.sh
```

2nd terminal 
```bash
/home/ecsperception/ros2_ws/src/tafe_sensors/scripts/right_cam.sh
```

3rd termianl 
```bash
ros2 launch tafe_sensors stereo_image_processing.launch.py 
```

###########
Visualization

1st terminal 
```bash
ros2 run rqt_gui rqt_gui
```

2nd terminal
```bash
ros2 run image_view disparity_view --ros-args --remap image:=/disparity
```

3rd termianl 
```bash
rviz2
```
