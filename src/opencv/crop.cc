#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

/*
This programs demonstrates the following:
- Load the Source Image: Read a standard color image file (like a .jpg or .png) using OpenCV.
- Generate a Crop: Extract a sub-matrix named cropped using a bounding box that 
  - starts at pixel coordinate (x=10, y=10)
  - Have a width of 100 pixels and a height of 100 pixels
- Analyze and Compare Metadata: Print a clean, formatted comparison table
- Verify Memory Sharing: 
  - Change the color of the very first pixel of the cropped image to pure green. 
  - Print out the color values of the pixel at coordinate (10, 10) in the original image

Output:
------------------
Original: 
Height: 600, Width: 800
Channels: 3
Continuous: 1
Step: 2400
------------------
Cropped: 
Height: 100, Width: 100
Channels: 3
Continuous: 0
Step: 2400
Original pixel at (Row 10, Col 10) after crop change: B=0, G=255, R=0
------------------
Cropped After Cloning: 
Height: 100, Width: 100
Channels: 3
Continuous: 1
Step: 300

Observations:
- The cropped image is basically a shared pointer to the original image.
- Modifying a cropped image pixel modifies the input image. 
- The cropped image strides are non-continuous. Therefore if a crop is even a small sub-region of original
  - Processing this image still requires the memory stride as input image.
  - This will result in cache misses and processing image will be a bottleneck during inferencing.
- Solution: The image needs to be cloned, ensuring the continuity and better cache performance during inferencing (One time upfront clone cost).
*/

int main() {
    std::string directory_path = "../";
    // Step 1: Read an image
    cv::Mat original = cv::imread(directory_path + "sample_image.jpg");
    // Step 2: Crop
    cv::Rect crop_region(10,10,100,100);
    cv::Rect safe_region = crop_region & cv::Rect(0,0,original.cols,original.rows);
    cv::Mat cropped_image = original(safe_region);
    
    // Step 3: Print analysis.
    std::cout << "------------------" << std::endl;
    std::cout << "Original: " << std::endl;
    std::cout << "Height: " << original.rows << ", Width: " << original.cols << std::endl;
    std::cout << "Channels: " << original.channels() << std::endl;
    std::cout << "Continuous: " << original.isContinuous() << std::endl;
    std::cout << "Step: " << original.step << std::endl;
    std::cout << "------------------" << std::endl;
    std::cout << "Cropped: " << std::endl;
    std::cout << "Height: " << cropped_image.rows << ", Width: " << cropped_image.cols << std::endl;
    std::cout << "Channels: " << cropped_image.channels() << std::endl;
    std::cout << "Continuous: " << cropped_image.isContinuous() << std::endl;
    std::cout << "Step: " << cropped_image.step << std::endl;
    
    // Step 4: Modify
    cropped_image.at<cv::Vec3b>(0, 0) = cv::Vec3b(0, 255, 0); 
    cv::Vec3b original_pixel = original.at<cv::Vec3b>(10, 10);
    std::cout << "Original pixel at (Row 10, Col 10) after crop change: " 
            << "B=" << (int)original_pixel[0] 
            << ", G=" << (int)original_pixel[1] 
            << ", R=" << (int)original_pixel[2] << std::endl;

    // Step 5: Clone and Verify continuity 
    cv::Mat cloned_crop = cropped_image.clone()
    std::cout << "------------------" << std::endl;
    std::cout << "Cropped After Cloning: " << std::endl;
    std::cout << "Height: " << cloned_crop.rows << ", Width: " << cloned_crop.cols << std::endl;
    std::cout << "Channels: " << cloned_crop.channels() << std::endl;
    std::cout << "Continuous: " << cloned_crop.isContinuous() << std::endl;
    std::cout << "Step: " << cloned_crop.step << std::endl;

    // Step 6: Write
    cv::imwrite(directory_path + "cropped_image.jpg", cropped_image);
    return 0;
}