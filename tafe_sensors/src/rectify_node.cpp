
#include <functional>
#include <mutex>

#include "cv_bridge/cv_bridge.h"
#include "tracetools_image_pipeline/tracetools.h"

#include <rectify.hpp>
#include <image_transport/image_transport.hpp>
#include <opencv2/imgproc.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/image.hpp>

namespace image_proc
{

RectifyNode::RectifyNode(const rclcpp::NodeOptions & options)
: rclcpp::Node("RectifyNode", options)
{
  queue_size_ = this->declare_parameter("queue_size", 5);
  interpolation = this->declare_parameter("interpolation", 1);
  pub_rect_ = image_transport::create_publisher(this, "image_rect");
  subscribeToCamera();
}

// Handles (un)subscribing when clients (un)subscribe
void RectifyNode::subscribeToCamera()
{
  std::lock_guard<std::mutex> lock(connect_mutex_);

  /*
  *  SubscriberStatusCallback not yet implemented
  *
  if (pub_rect_.getNumSubscribers() == 0)
    sub_camera_.shutdown();
  else if (!sub_camera_)
  {
  */
  sub_camera_ = image_transport::create_camera_subscription(
    this, "image", std::bind(
      &RectifyNode::imageCb,
      this, std::placeholders::_1, std::placeholders::_2), "raw");
  // }
}

void RectifyNode::imageCb(
  const sensor_msgs::msg::Image::ConstSharedPtr & image_msg,
  const sensor_msgs::msg::CameraInfo::ConstSharedPtr & info_msg)
{
  TRACEPOINT(
    image_proc_rectify_init,
    static_cast<const void *>(this),
    static_cast<const void *>(&(*image_msg)),
    static_cast<const void *>(&(*info_msg)));

  if (pub_rect_.getNumSubscribers() < 1) {
    TRACEPOINT(
      image_proc_rectify_fini,
      static_cast<const void *>(this),
      static_cast<const void *>(&(*image_msg)),
      static_cast<const void *>(&(*info_msg)));

    return;
  }

  // Verify camera is actually calibrated
  if (info_msg->k[0] == 0.0) {
    RCLCPP_ERROR(
      this->get_logger(), "Rectified topic '%s' requested but camera publishing '%s' "
      "is uncalibrated", pub_rect_.getTopic().c_str(), sub_camera_.getInfoTopic().c_str());
    TRACEPOINT(
      image_proc_rectify_fini,
      static_cast<const void *>(this),
      static_cast<const void *>(&(*image_msg)),
      static_cast<const void *>(&(*info_msg)));
    return;
  }

  // If zero distortion, just pass the message along
  bool zero_distortion = true;

  for (size_t i = 0; i < info_msg->d.size(); ++i) {
    if (info_msg->d[i] != 0.0) {
      zero_distortion = false;
      break;
    }
  }

  // This will be true if D is empty/zero sized
  if (zero_distortion) {
    pub_rect_.publish(image_msg);
    TRACEPOINT(
      image_proc_rectify_fini,
      static_cast<const void *>(this),
      static_cast<const void *>(&(*image_msg)),
      static_cast<const void *>(&(*info_msg)));
    return;
  }

  // Update the camera model
  model_.fromCameraInfo(info_msg);

  // Create cv::Mat views onto both buffers
  const cv::Mat image = cv_bridge::toCvShare(image_msg)->image;
  cv::Mat rect;

  // Rectify and publish
  model_.rectifyImage(image, rect, interpolation);

  // Allocate new rectified image message
  sensor_msgs::msg::Image::SharedPtr rect_msg =
    cv_bridge::CvImage(image_msg->header, image_msg->encoding, rect).toImageMsg();
  pub_rect_.publish(rect_msg);

  TRACEPOINT(
    image_proc_rectify_fini,
    static_cast<const void *>(this),
    static_cast<const void *>(&(*image_msg)),
    static_cast<const void *>(&(*info_msg)));
}

}  // namespace image_proc

#include "rclcpp_components/register_node_macro.hpp"

// Register the component with class_loader.
// This acts as a sort of entry point, allowing the component to be discoverable when its library
// is being loaded into a running process.
RCLCPP_COMPONENTS_REGISTER_NODE(image_proc::RectifyNode)
