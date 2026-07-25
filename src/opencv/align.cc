#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

/**
 * @brief Problem 3 — Custom Hardware Stride Alignment & Memory Continuous Analysis
 * 
 * CORE INTENT:
 * This program isolates the underlying allocation constraints of computer vision 
 * structures, demonstrating how real-time operating systems and middleware force 
 * matrix rows to map to hard hardware boundaries.
 * 
 * THE ARCHITECTURAL REALITY:
 * - Stride Step vs. True Width: The physical row size (step/stride) in hardware memory 
 *   frequently exceeds the mathematical pixel width. Extra space is deliberately padded 
 *   to ensure the start address of every row aligns cleanly with the system memory bus.
 * 
 * - The Alignment Formula: Stride bytes are rounded up to the nearest multiple of the 
 *   hardware boundary (e.g., 32-byte or 64-byte chunks) using the universal mapping rule:
 *   Step = ceil((Width * Channels) / Alignment Threshold) * Alignment Threshold
 * 
 * THE CORE CONTINUOUS BARRIER:
 * - When an allocation includes dead padding bytes between rows, memory contiguity collapses. 
 *   The matrix flag drops to non-continuous (isContinuous = 0).
 * - General-purpose CPUs feature elastic out-of-order execution pipelines and wide cache line 
 *   pre-fetchers that handle unaligned memory with zero latency penalty.
 * - Specialized Edge-AI hardware (DSPs/NPUs) relies on strict Vector/SIMD parallel registers. 
 *   Feeding an unaligned row stride forces split memory fetches and internal bit-shifting 
 *   stalls, making strict stride alignment mandatory for real-time edge processing.
 * KEY TAKEAWAY:
 * Padding is a deliberate, structural trade-off: we waste cheap memory capacity to buy expensive execution speed.
 */

int main() {
    std::string directory_path = "../resources/";
    std::cout << "Using OpenCV Program" << std::endl;
    cv::Mat img(13, 101, CV_8UC3, cv::Scalar(255,0,0)); // All Blue.

    std::cout << "------------------" << std::endl;
    std::cout << "Image 1: " << std::endl;
    std::cout << "Height: " << img.rows << ", Width: " << img.cols << std::endl;
    std::cout << "Channels: " << img.channels() << std::endl;
    std::cout << "Continuous: " << img.isContinuous() << std::endl;
    std::cout << "Step: " << img.step << std::endl;

    cv::imwrite(directory_path + "image1.png", img);
    // Image with alignment

    int width = 376; // Plug in desired number
    int height = 580;
    int alignment = 64;

    int block_width = std::ceil(width * 3 / (double)alignment) * alignment;
    std::vector<uint8_t> buffer(block_width * height);
    cv::Mat img2(height, width, CV_8UC3, buffer.data(), block_width); // All Blue.
    
    std::cout << "------------------" << std::endl;
    std::cout << "Image 2: " << std::endl;
    std::cout << "Height: " << img2.rows << ", Width: " << img2.cols << std::endl;
    std::cout << "Channels: " << img2.channels() << std::endl;
    std::cout << "Continuous: " << img2.isContinuous() << std::endl;
    std::cout << "Step: " << img2.step << std::endl;
    cv::imwrite(directory_path + "image2.png", img2);
    return 0;
}