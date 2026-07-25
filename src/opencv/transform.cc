#include <chrono>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <string>

/**
 * CORE INTENT:
 * This program implements a manual, pixel-level data transformation layer
 * to bridge the architectural gap between classical computer vision libraries
 * and edge AI inference frameworks.
 * 
 * THE ARCHITECTURAL TRANSITION:
 * - From (Input): OpenCV Interleaved Format (HWC)
 *   Pixels are grouped as [B,G,R][B,G,R] in memory. This layout optimizes localized
 *   pixel math (like grayscale conversion) by packing color components into a single
 *   CPU cache line.
 * 
 * - To (Output): Deep Learning Planar Format (CHW)
 *   Pixels are sorted into entirely separate spatial blocks: [All Blues][All Greens][All Reds].
 *   This layout optimizes vector math blocks (like 2D Convolutions) by allowing Matrix 
 *   Multiply Accelerators (NPUs) to stream continuous spatial feature maps without skipping bytes.
 * 
 * EDGE-AI & ACCELERATOR IMPACT:
 * - General-purpose CPU cores utilize large silicon caches, hardware write-buffers, and 
 *   stride pre-fetchers to absorb the performance penalties of layout transformations.
 * - Hardware accelerators (DSPs, NPUs) are highly streamlined and lack these scheduling safety nets.
 *   Feeding a poorly arranged layout directly to an NPU stalls its execution pipeline, making an 
 *   upfront, contiguous layout conversion layer mandatory for real-time edge processing.
 */

int main() {
    std::string directory_path = "../";
    // Read an image
    cv::Mat original = cv::imread(directory_path + "sample_image.jpg");
    auto ncols = original.cols;
    auto nrows = original.rows;
    uint8_t* input = original.data;
    int image_size = ncols * nrows;

    // Reserve memory for output transformed buffer.
    std::vector<uint8_t> output_buffer(image_size * 3);
    // ==========================================
    // PROFILE OPTION A: Linear Reads, Scattered Writes
    // ==========================================
    auto start = std::chrono::high_resolution_clock::now();

    // Transformation Logic.
    for(auto i = 0; i < nrows; ++i) {
        auto row_idx = ncols * i;
        for(auto j = 0; j < ncols; ++j) {
            auto input_idx = row_idx * 3 + j * 3; 
            output_buffer[row_idx + j] = input[input_idx]; // B 
            output_buffer[image_size + row_idx + j] = input[input_idx+ 1]; // G 
            output_buffer[image_size * 2 + row_idx + j] = input[input_idx + 2]; // R
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Profiling Option A: " << duration << std::endl;

    // ==========================================
    // Optional Exercise: PROFILE OPTION B: Scattered Reads, Linear Writes
    // ==========================================
    std::vector<uint8_t> output_buffer_B(image_size * 3);
    auto start_B = std::chrono::high_resolution_clock::now();

    for(auto channel = 0; channel < 3; ++channel) {
        for(auto i = 0; i < nrows; ++i) {
            auto row_idx = ncols * i;
            for(auto j = 0; j < ncols; ++j) {
                auto input_idx = row_idx * 3 + j * 3;
                output_buffer_B[image_size * channel + row_idx + j] = input[input_idx + channel];
            }
        }
    }

    auto end_B = std::chrono::high_resolution_clock::now();
    auto duration_B = std::chrono::duration_cast<std::chrono::microseconds>(end_B - start_B).count();
    std::cout << "Profiling Option B: " << duration_B << std::endl;
}