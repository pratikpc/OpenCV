#include "include/Image.hxx"
#include "include/Window.hxx"

int main(int argc, char** argv)
{
    if(argc != 2) /*1st Pos occupied by Exec Name. 2nd by Data*/
        return -1;

    PC::OpenCV::Image img{argv[1]};

    if(std::empty(img))
        return -1;

    PC::OpenCV::UI::Window window{"PC_Window_Display_Image"}; // Just Keep this code Unique in your program
    window.displayImage(img);
    window.resize(img.getSize());
    window.waitKey();

    return 0;
}
