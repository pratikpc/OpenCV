#pragma once

#include <opencv2/imgcodecs/imgcodecs.hpp>

#include <vector>

namespace PC
{
namespace OpenCV
{
	struct Image
	{
        using ReadMode = cv::ImreadModes;
        using WriteMode = cv::ImwriteFlags;
    private:
		cv::Mat m_img;
    public:
        Image(const cv::String& p_file_name, const Image::ReadMode p_read_mode = Image::ReadMode::IMREAD_COLOR) : m_img {cv::imread(p_file_name, p_read_mode)}
		{
		}
        Image(cv::InputArray& buffer, const Image::ReadMode p_read_mode) : m_img {cv::imdecode(buffer, p_read_mode)}
        {
        }

        template<typename Type, typename = std::enable_if_t<std::is_same_v<Type, cv::Mat> || std::is_same_v<Type, cv::InputArray>>>
        Type as() const noexcept
        {
            return m_img.clone();
        }

        cv::Size getSize() const noexcept
        {
            return m_img.size();
        }

        bool empty()
        {
            return std::empty(m_img);
        }

        bool write(const cv::String& p_file_name, std::vector<int>& p_write_mode)
        {
            return cv::imwrite(p_file_name, p_write_mode);
        }
	};
}
}
