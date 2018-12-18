#pragma once

#include <opencv2/highgui/highgui.hpp>

#include "Image.hxx"

namespace PC { namespace OpenCV { namespace UI
{
	struct Window
		{
			using SizeFlags = cv::WindowFlags;
			using Property = cv::WindowPropertyFlags;
		private:
			const cv::String m_window_id;
		public:
            Window(const cv::String& p_window_id, const SizeFlags p_flag = SizeFlags::WINDOW_AUTOSIZE) noexcept: m_window_id{p_window_id}
			{
				cv::namedWindow(m_window_id, p_flag);
			}
			Window& resize(const cv::Size& p_size)
			{
				cv::resizeWindow(m_window_id, p_size);
				return *this;
			}
			Window& resize(const std::int16_t p_x, const std::int16_t p_y)
			{
				cv::resizeWindow(m_window_id, p_x, p_y);
				return *this;
			}

			Window& setTitle(const cv::String& p_title)
			{
				cv::setWindowTitle(m_window_id, p_title);
				return *this;
			}

			Window& addProperty(const Property& p_flag, const SizeFlags& p_val)
			{
				cv::setWindowProperty(m_window_id, p_flag, p_val);
				return *this;
			}
            auto getProperty(const Property& p_id)
			{
                return (cv::getWindowProperty(m_window_id, p_id));
			}

			template<typename MouseHandler>
			Window& addMouseHandler(MouseHandler p_handler, void * p_user_data = nullptr)
			{
				cv::setMouseCallback(m_window_id, p_handler, p_user_data);
				return *this;
			}

			Window& move(const std::int16_t p_x, const std::int16_t p_y)
			{
				cv::moveWindow(m_window_id, p_x, p_y);
				return *this;
			}

            Window& displayImage(cv::InputArray& p_img)
			{
				cv::imshow(m_window_id, p_img);
				return *this;
			}
            Window& displayImage(const OpenCV::Image& p_image)
            {
                return displayImage(p_image.as<cv::Mat>());
            }
            cv::Rect getAsRect()
			{
				return cv::getWindowImageRect(m_window_id);
			}
			
			~Window()
			{
				cv::destroyWindow(m_window_id);
			}

			static void destroyAllWindows()
			{
				cv::destroyAllWindows();
			}
			
			static int waitKey(const std::uint16_t p_delay = 0/*Wait Forever*/)
			{
				return cv::waitKey(p_delay);
			}
			static int waitKeyEx(const std::uint16_t p_delay = 0/*Wait Forever*/)
			{
				return cv::waitKeyEx(p_delay);
			}

		};
} } }
