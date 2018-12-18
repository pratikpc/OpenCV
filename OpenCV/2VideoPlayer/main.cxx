#include "include/Window.hxx"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

int main()
{
    PC::OpenCV::UI::Window window{"2_Video_Player_Window"};
    cv::VideoCapture video_player("F:\\pvlc\\SONGS\\Seeing Is Believingor Is It.wmv");
    std::cout << std::boolalpha << video_player.isOpened();
    std::cerr << "Hello\n\n";

    cv::Mat frame;
    while(true)
    {
        video_player >> frame;

        if(std::empty(frame))
        {
            window.destroyAllWindows();
            break;
        }
        window.displayImage(frame);
        window.waitKey(0);
    }
    std::cin.get();
}
