#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/core.hpp>

int main() {
    std::string directory_path = "../";
    std::cout << "Using OpenCV Program" << std::endl;
    cv::Mat img(300,300,CV_8UC3,cv::Scalar(255,0,0));
    //cv::imshow("Test",img);
    std::cout << "Image created with rows: " << img.rows << ", cols: " << img.cols << std::endl;
    cv::imwrite(directory_path + "test.png", img);
    cv::waitKey(10);
    return 0;
}
