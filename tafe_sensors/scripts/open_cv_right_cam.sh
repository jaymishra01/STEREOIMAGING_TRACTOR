#ros2 run usb_cam usb_cam_node_exe --ros-args --remap __ns:=/right_camera --params-file /home/ecsperception/ros2_ws/src/tafe_sensors/config/params_right_cam.yaml

ros2 run opencv_cam opencv_cam_main \
  --ros-args --param index:=2 \
  --param camera_info_path:=/home/ecsperception/ros2_ws/src/tafe_sensors/config/camera_info_right.yaml \
  --remap __ns:=/right_camera \
  --remap /left/image_raw:=/image_raw \
  --param camera_frame_id:=right_camera \
  --param file:=false \
  --param fps:=30 \
  --param height:=480 \
  --param width:=640