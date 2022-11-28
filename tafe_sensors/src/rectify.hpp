#ifndef _RECTIFY_HPP_
#define _RECTIFY_HPP_

#include <mutex>

#include "image_geometry/pinhole_camera_model.h"

#include <image_transport/image_transport.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/image.hpp>

namespace image_proc
{

class RectifyNode
  : public rclcpp::Node
{
public:
  explicit RectifyNode(const rclcpp::NodeOptions &);

private:
  image_transport::CameraSubscriber sub_camera_;

  int queue_size_;
  int interpolation;
  std::mutex connect_mutex_;
  image_transport::Publisher pub_rect_;

  // Processing state (note: only safe because we're using single-threaded NodeHandle!)
  image_geometry::PinholeCameraModel model_;

  void subscribeToCamera();
  void imageCb(
    const sensor_msgs::msg::Image::ConstSharedPtr & image_msg,
    const sensor_msgs::msg::CameraInfo::ConstSharedPtr & info_msg);
};

}  // namespace image_proc

#endif  // IMAGE_PROC__RECTIFY_HPP_
