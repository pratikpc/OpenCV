#include <vector>

#include <opencv2/bgsegm.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp>

#include "Window.hxx"

#include "Shapes.hxx"

namespace Detector
{
	enum class BackGroundSubtractorTypes
	{
		// Default Option
		// No BackGround Subtractor Selected
		NONE,
		// MOG Selected
		// For further details, visit
		//	https://docs.opencv.org/3.1.0/d6/da7/classcv_1_1bgsegm_1_1BackgroundSubtractorMOG.html
		MOG,
		// MOG2 Selected
		// For further details, visit
		// https://docs.opencv.org/master/d7/d7b/classcv_1_1BackgroundSubtractorMOG2.html
		MOG2,
		// GMG Selected
		// For further details, visit
		// https://docs.opencv.org/master/d1/d5c/classcv_1_1bgsegm_1_1BackgroundSubtractorGMG.html
		GMG,
		// CNT Selected
		// For Further Details, visit
		// https://docs.opencv.org/master/de/dca/classcv_1_1bgsegm_1_1BackgroundSubtractorCNT.html
		// Note that, this Algorithm May Prove to Be Twice As Fast on a RasPI like Device
		CNT,
		// KNN Selected
		// For Further Details, visit
		// https://docs.opencv.org/master/db/d88/classcv_1_1BackgroundSubtractorKNN.html
		KNN
	};
	struct Characteristics
	{
	 private:
		// Define the Lower and Higher Colour Bounds
		cv::Scalar m_lower_colour_bound{0, 0, 0};
		cv::Scalar m_higher_colour_bound{0, 0, 0};

		// The BackGround Subtractor Algorithm To Use
		BackGroundSubtractorTypes m_bckgrnd_sbtrctr_type = BackGroundSubtractorTypes::NONE;

		// The Frames Per Second(FPS) Rate of Optic Device Involved
		// Default Value is 15
		// Note this value is chosen because that is what CNT uses
		// For further details, refer
		// https://github.com/sagi-z/BackgroundSubtractorCNT
		std::int32_t m_fps = 15;

		// Store a Pair of Values
		// These Values Represent Canny Thresholds
		std::uint32_t m_canny_threshold1;
		std::uint32_t m_canny_threshold2;

		// Add Characteristics as and when required

	 public:
		// Note provide Limits as HSV
		Characteristics& setColourBounds(const cv::Scalar& p_lower_colour_bound,
													const cv::Scalar& p_higher_colour_bound) noexcept
		{
			m_lower_colour_bound  = p_lower_colour_bound;
			m_higher_colour_bound = p_higher_colour_bound;
			return *this;
		}
		cv::Scalar getLowerColourBounds() const noexcept
		{
			return m_lower_colour_bound;
		}
		cv::Scalar getHigherColourBounds() const noexcept
		{
			return m_higher_colour_bound;
		}

		std::int32_t getFramesPerSecond() const noexcept
		{
			return m_fps;
		}
		Characteristics& setFramesPerSecond(const std::int32_t p_fps) noexcept
		{
			m_fps = p_fps;
			return *this;
		}

		Characteristics& setCannyThreshold(const std::uint32_t p_canny_threshold1, const std::uint32_t p_canny_threshold2)
		{
			m_canny_threshold1 = p_canny_threshold1;
			m_canny_threshold2 = p_canny_threshold2;

			return *this;
		}
		std::uint32_t getCannyThresholdFirst() const noexcept
		{
			return m_canny_threshold1;
		}
		std::uint32_t getCannyThresholdSecond() const noexcept
		{
			return m_canny_threshold2;
		}

		Characteristics& setBackGroundSubtractor(const BackGroundSubtractorTypes p_bckgrnd_sbtrctr_type)
		{
			m_bckgrnd_sbtrctr_type = p_bckgrnd_sbtrctr_type;
			return *this;
		}
		BackGroundSubtractorTypes getBackGroundSubtractorType() const noexcept
		{
			return m_bckgrnd_sbtrctr_type;
		}
	};

	struct Detector
	{
	 private:
		Characteristics						 m_obj_detect_properties;
		cv::Ptr<cv::BackgroundSubtractor> m_bckgrnd_sbtrctr;

	 public:
		Detector(const Characteristics& p_obj_detect_properties) : m_obj_detect_properties{p_obj_detect_properties}
		{
			createBackgroundSubtractor();
		}
		inline Detector& setCharacteristics(const Characteristics& p_obj_detect_properties)
		{
			m_obj_detect_properties = p_obj_detect_properties;
			createBackgroundSubtractor();
			return *this;
		}

		inline bool supplyBackgroundImage(const cv::Mat& p_img)
		{
			if (std::empty(p_img))
				return false;

			// Note that ProcessImage Function Also Subtracts from Background
			processImage(p_img);

			return true;
		}
		Shape::Circle<> detectCircularObjectCenters(const cv::Mat& img_src)
		{
			if (std::empty(img_src))
				return nullptr;

			// Processing Done as No 2 Images are the Same
			// Ever. Hence after Processing and removing
			// Unnecessary details, we get a real clean image
			// This clean Image contains exactly the data we need

			auto img_proc = processImage(img_src);
			if (std::empty(img_proc))
				return nullptr;

			// UI::Window window{"abc"};
			// window.displayImage(img_proc);
			// window.waitKey();

			// std::vector<cv::Vec3f> circle_points;
			// cv::HoughCircles(img_proc, circle_points, CV_HOUGH_GRADIENT, 2, img_proc.rows/8,90,90);

			cv::Canny(img_proc,
						 img_proc,
						 m_obj_detect_properties.getCannyThresholdFirst(),
						 m_obj_detect_properties.getCannyThresholdSecond());

			std::vector<std::vector<cv::Point>> circle_points;
			cv::findContours(img_proc, circle_points, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

			// Remove All Elements that are not Circles
			// This can be done by finding the Approximate Number of Vertices
			// If the Approximate Number of Vertices are less than, say 8
			// Than it is not a circle
			circle_points.erase(std::remove_if(std::begin(circle_points),
														  std::end(circle_points),
														  [](const auto& elem) {
															  std::vector<cv::Point> approx_curve;
															  cv::approxPolyDP(
																	elem, approx_curve, 0.01 * cv::arcLength(elem, true), true);

															  // Remove if Elem is Not a Circle or Circular in Shape
															  // The Size of approx_curve gives the Number of Vertices
															  return (std::size(approx_curve) < 8);
														  }),
									  std::end(circle_points));

			if (std::empty(circle_points))
				return nullptr;

			// We Shall work under the Assumption that the Largest Circle
			// Is the Object We want to Detect
			const auto circle_detected = (*std::max_element(
				 std::cbegin(circle_points), std::cend(circle_points), [](const auto& p_left, const auto& p_right) {
					 // Sort in Descending order of area
					 return cv::contourArea(p_left) < cv::contourArea(p_right);
				 }));

			return circle_detected;
		}

	 private:
		// Applies Blurr, inRange dilate and erode to make the image better and more visible
		// Original Image Assumed to be in BGR Format
		// Also performs Background Subtraction
		inline cv::Mat processImage(const cv::Mat& img_src)
		{

			if (std::empty(img_src))
				return cv::Mat(); // Returns Empty Matrix

			cv::Mat img_out;

			// Source is
			// https://www.opencv-srf.com/2010/09/object-detection-using-color-seperation.html

			// Convert Original Image to HSV Thresh Image
			cv::cvtColor(img_src, img_out, cv::ColorConversionCodes::COLOR_BGR2HSV);

			cv::inRange(img_out,
							m_obj_detect_properties.getLowerColourBounds(),
							m_obj_detect_properties.getHigherColourBounds(),
							img_out);

			const cv::Size kSize{3, 3};

			// blur effect
			cv::GaussianBlur(img_out, img_out, kSize, 0);

			const auto structuring_elem = cv::getStructuringElement(cv::MORPH_ELLIPSE, kSize);

			// morphological opening (remove small objects from the foreground)
			cv::erode(img_out, img_out, structuring_elem);
			cv::dilate(img_out, img_out, structuring_elem);

			// morphological closing (fill small holes in the foreground)
			cv::dilate(img_out, img_out, structuring_elem);
			cv::erode(img_out, img_out, structuring_elem);

			// Apply Background Subtraction
			// if Background Subtractor Initialised

			if (!std::empty(m_bckgrnd_sbtrctr))
				m_bckgrnd_sbtrctr->apply(img_out, img_out);

			return img_out;
		}

		// Note that In order to make this more efficient
		// You would later have to modify the parameters.
		// Please refer to Appropriate Documentation for Appropriate Background Subtracter
		inline void createBackgroundSubtractor()
		{
			switch (m_obj_detect_properties.getBackGroundSubtractorType())
			{
				 case BackGroundSubtractorTypes::CNT:
					// These Settings Have Been taken from the Semi Official GitHub Page at
					// https://github.com/sagi-z/BackgroundSubtractorCNT
					m_bckgrnd_sbtrctr =
						 cv::bgsegm::createBackgroundSubtractorCNT(m_obj_detect_properties.getFramesPerSecond(),
																				 true,
																				 60 * m_obj_detect_properties.getFramesPerSecond());
					break;
				 case BackGroundSubtractorTypes::GMG:
					m_bckgrnd_sbtrctr = cv::bgsegm::createBackgroundSubtractorGMG();
					break;
				 case BackGroundSubtractorTypes::MOG:
					m_bckgrnd_sbtrctr = cv::bgsegm::createBackgroundSubtractorMOG();
					break;
				case BackGroundSubtractorTypes::MOG2:
					m_bckgrnd_sbtrctr = cv::createBackgroundSubtractorMOG2();
					break;
				case BackGroundSubtractorTypes::KNN:
					m_bckgrnd_sbtrctr = cv::createBackgroundSubtractorKNN();
					break;
				case BackGroundSubtractorTypes::NONE:
					m_bckgrnd_sbtrctr.release();
					break;
			}
		}
	};
} // namespace Detector