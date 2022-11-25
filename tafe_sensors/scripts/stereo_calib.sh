ros2 run camera_calibration cameracalibrator \
  --size=10x7 \
  --square=0.025 \
  --approximate=0.3 \
  --no-service-check \
  --ros-args --remap /left:=/leftcamera/image_raw \
  --remap /right:=/right_camera/image_raw