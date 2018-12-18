#pragma once

#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>

namespace PC {
namespace OpenCV {
struct Image {
  using ReadMode = cv::ImreadModes;
  using WriteMode = cv::ImwriteFlags;
  using BorderType = cv::BorderTypes;

private:
  cv::Mat m_img;

public:
  Image(const cv::String &p_file_name,
        const Image::ReadMode p_read_mode = Image::ReadMode::IMREAD_COLOR)
      : m_img{cv::imread(p_file_name, p_read_mode)} {}
  Image(cv::InputArray &buffer, const Image::ReadMode p_read_mode)
      : m_img{cv::imdecode(buffer, p_read_mode)} {}
  Image(const cv::Mat &buf) : m_img{buf} {}

  template <typename Type,
            typename = std::enable_if_t<std::is_same_v<Type, cv::Mat> ||
                                        std::is_same_v<Type, cv::InputArray>>>
  Type as() const noexcept {
    return m_img.clone();
  }

  Image GaussianBlur(
      const cv::Size& ksize, const double sigmaX, const double sigmaY = 0,
      const BorderType borderType = Image::BorderType::BORDER_DEFAULT) {
    cv::Mat out;
    cv::GaussianBlur(m_img, out, ksize, sigmaX, sigmaY, borderType);
    return out;
  }
  Image medianBlur(const int ksize) {
    cv::Mat out;
    cv::medianBlur(m_img, out, ksize);
    return out;
  }

  cv::Size getSize() const noexcept { return m_img.size(); }

  Image changeColour()
  {
      cv::Mat out;
      cv::cvtColor(m_img, out, CV_BGR2GRAY);
      return out;
  }

  Image clone() const
  {
      return m_img.clone();
  }

  bool empty() const noexcept(noexcept(std::empty(m_img))) {
    return std::empty(m_img);
  }

  bool write(const cv::String &p_file_name, std::vector<int> &p_write_mode) {
    return cv::imwrite(p_file_name, p_write_mode);
  }
};
} // namespace OpenCV
} // namespace PC
