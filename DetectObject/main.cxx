#include <iostream>

#include "include/CircularObjectDetector.hxx"
#include "include/Window.hxx"

int main()
{
	cv::VideoCapture video("video");

	if (!video.isOpened())
		return 1;

	Detector::Characteristics props;
	props
		 .setBackGroundSubtractor(Detector::BackGroundSubtractorTypes::CNT)

		 // Get Colour Values for Yellow from
		 // https://stackoverflow.com/questions/9179189/detect-yellow-color-in-opencv
		 // .setColourBounds(cv::Scalar{20, 70, 70}, cv::Scalar{30, 255, 255})

		 // Get Colour Values for Purple from
		 // https://stackoverflow.com/questions/17474020/finding-exact-hsv-values-of-colors
		 .setColourBounds(cv::Scalar{115, 50, 50}, cv::Scalar{116, 255, 255})
		 .setCannyThreshold(100, 100)
		 .setFramesPerSecond(2);

	Detector::Detector detector{props};

	cv::Mat img;
	while (true)
	{
		cv::Mat src;
		video >> src;

		if (std::empty(src))
			break;

		if (std::empty(img))
			img = cv::Mat::zeros(src.rows, src.cols, src.type());

		const auto circle = detector.detectCircularObjectCenters(src);
		if (!std::empty(circle))
		{
			cv::circle(img, circle.getCenter(), 2, {0, 255, 0}, 3);
			
			UI::Window motion{"images"};
			motion.displayImage(img);
			
			UI::Window source{"src"};
			source.displayImage(src);
			source.move(800,0);

			UI::Window::waitKey(500);
		}
	}
	UI::Window motion{"images"};
	motion.displayImage(img);
	UI::Window::waitKey();
	return 0;
}