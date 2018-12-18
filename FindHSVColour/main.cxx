#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

cv::Mat img;
void onMouse(int evt, int x, int y, int flags, void *param) {
  if (evt == CV_EVENT_LBUTTONDOWN) {
    cv::Mat img_hsv;
    cv::cvtColor(img, img_hsv, cv::COLOR_BGR2HSV);
    cv::Vec3i hsv = img_hsv.at<cv::Vec3b>(y, x);
    std::cout << "H=" << hsv[0] << ",S=" << hsv[1] << ",V=" << hsv[2] << '\n';
  }
}
int main() {

	img = cv::imread("f:\\abc.jpg");

	if (std::empty(img))
    return 1;

  cv::namedWindow("imgdisp");
  cv::setWindowTitle("imgdisp", "Colour Picker. The HSV of Colour Will Be Displayed on CommandLine Window");
  cv::imshow("imgdisp", img);
  cv::setMouseCallback("imgdisp", onMouse);
  cv::waitKey();
  return 0;
}