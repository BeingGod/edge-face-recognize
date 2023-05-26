#include "opencv2/videoio.hpp"
#include <iostream>


int main(int argc, char* argv[])
{
  cv::VideoCapture cap(0);

  if (cap.isOpened()) {
    std::cout << "open camera success!" << std::endl;
  }
  else {
    std::cout << "open camera failed!" << std::endl;
  }

  return 0;
}