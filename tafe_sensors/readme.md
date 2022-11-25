https://navigation.ros.org/tutorials/docs/camera_calibration.html






    ros2 run camera_calibration cameracalibrator --pattern chessboard--size 8x11 --square 0.025 --ros-args -r image:=/left_camera/image_raw -p camera:=/left_camera

    ros2 run camera_calibration cameracalibrator --pattern chessboard--size 8x11 --square 0.025 --ros-args -r image:=/left_camera/image_raw -p camera:=/left_camera