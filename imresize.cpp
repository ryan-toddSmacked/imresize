
// OpenCV includes
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

// Boost argument parser includes
#include "boost/program_options.hpp"

#include <cstdlib>
#include <cstdint>
#include <cinttypes>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <chrono>

namespace po = boost::program_options;

// Desctiption string for help program option
static const std::string help_desc =
"Image Resizing application.\n"
"Uses OpenCV " CV_VERSION " to resize the image.\n"
"Only resizes the rows and columns of the image, not the channels.\n";

/// @brief This is a image resize program. It uses opencv to resize the image.
int main(int argc, char** argv)
{

    // Start global timer, measure time as a double in seconds
    auto start_global = std::chrono::high_resolution_clock::now();

    // Declare the supported options.
    po::options_description desc(help_desc + "Allowed options");

    desc.add_options()
    ("help", "produce help message")
    ("input,i", po::value<std::string>(), "input image")
    ("output,o", po::value<std::string>(), "output image")
    ("width,w", po::value<uint64_t>(), "requested width of the output image in pixels")
    ("height,h", po::value<uint64_t>(), "requested height of the output image in pixels")
    ("scale,s", po::value<double>(), "scale of the output image, {0.0 - Inf}")
    ("scale-width,x", po::value<double>(), "scale of the output image in width direction, {0.0 - Inf}")
    ("scale-height,y", po::value<double>(), "scale of the output image in height direction, {0.0 - Inf}")
    ("method,m", po::value<std::string>(), "interpolation method, {nearest, linear, cubic, area, lanczos4}")
    ("verbose,v", "verbose output");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    std::vector<std::string> interpolation_methods = {"nearest", "linear", "cubic", "area", "lanczos4"};
    std::string method = "cubic";
    bool antialiasing = true;
    bool dithering = true;
    bool verbose = false;
    double read_time = 0.0;
    double resize_time = 0.0;
    double write_time = 0.0;

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return EXIT_SUCCESS;
    }

    if (vm.count("input") == 0)
    {
        std::cout << "Error: input image is required" << std::endl;
        return EXIT_FAILURE;
    }
    if (vm.count("input") > 1)
    {
        std::cout << "Error: only one input image is allowed" << std::endl;
        return EXIT_FAILURE;
    }

    if (vm.count("output") == 0)
    {
        std::cout << "Error: output image is required" << std::endl;
        return EXIT_FAILURE;
    }
    if (vm.count("output") > 1)
    {
        std::cout << "Error: only one output image is allowed" << std::endl;
        return EXIT_FAILURE;
    }

    if (vm.count("width") == 0 && vm.count("height") == 0 && vm.count("scale") == 0 && vm.count("scale-width") == 0 && vm.count("scale-height") == 0)
    {
        std::cout << "Error: width, height, scale, scale-width or scale-height is required" << std::endl;
        return EXIT_FAILURE;
    }
    if (vm.count("width") > 1)
    {
        std::cout << "Error: only one width is allowed" << std::endl;
        return EXIT_FAILURE;
    }
    if (vm.count("height") > 1)
    {
        std::cout << "Error: only one height is allowed" << std::endl;
        return EXIT_FAILURE;
    }
    if (vm.count("scale") > 1)
    {
        std::cout << "Error: only one scale is allowed" << std::endl;
        return EXIT_FAILURE;
    }
    if (vm.count("scale-width") > 1)
    {
        std::cout << "Error: only one scale-width is allowed" << std::endl;
        return EXIT_FAILURE;
    }
    if (vm.count("scale-height") > 1)
    {
        std::cout << "Error: only one scale-height is allowed" << std::endl;
        return EXIT_FAILURE;
    }

    if (vm.count("method") > 1)
    {
        std::cout << "Error: only one interpolation method is allowed" << std::endl;
        return EXIT_FAILURE;
    }
    if (vm.count("method") == 1)
    {
        method = vm["method"].as<std::string>();
        if (std::find(interpolation_methods.begin(), interpolation_methods.end(), method) == interpolation_methods.end())
        {
            std::cout << "Error: interpolation method is not valid" << std::endl;
            return EXIT_FAILURE;
        }
    }

    if (vm.count("verbose"))
    {
        verbose = true;
    }

    if (verbose)
    {
        // Print opencv version string
        fprintf(stdout, "OpenCV version: %s\n", CV_VERSION);
        fprintf(stdout, "Image Resize:\n");
        fprintf(stdout, " Input image: %s\n", vm["input"].as<std::string>().c_str());
        fprintf(stdout, " Output image: %s\n", vm["output"].as<std::string>().c_str());
        fprintf(stdout, "Options:\n");
        if (vm.count("width") == 1)
        {
            fprintf(stdout, " Width: %lu\n", vm["width"].as<uint64_t>());
        }
        if (vm.count("height") == 1)
        {
            fprintf(stdout, " Height: %lu\n", vm["height"].as<uint64_t>());
        }
        if (vm.count("scale") == 1)
        {
            fprintf(stdout, " Scale: %f\n", vm["scale"].as<double>());
        }
        if (vm.count("scale-width") == 1)
        {
            fprintf(stdout, " Scale width: %f\n", vm["scale-width"].as<double>());
        }
        if (vm.count("scale-height") == 1)
        {
            fprintf(stdout, " Scale height: %f\n", vm["scale-height"].as<double>());
        }
        fprintf(stdout, " Interpolation method: %s\n", method.c_str());
    }

    std::string input_image = vm["input"].as<std::string>();
    // Read the input image, keeping default color mode
    cv::Mat image;
    if (verbose)
    {
        fprintf(stdout, "Loading input image...\n");
        // Start timer, measure time as a double in milliseconds
        auto start = std::chrono::high_resolution_clock::now();
        image = cv::imread(input_image, cv::IMREAD_UNCHANGED);
        // Stop timer
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        fprintf(stdout, "Loading elapsed time: %lf ms\n", elapsed.count() * 1000.0);
        read_time = elapsed.count();
    }
    else
        image = cv::imread(input_image, cv::IMREAD_UNCHANGED);
    if (image.empty())
    {
        std::cout << "Error: input image is not valid" << std::endl;
        return EXIT_FAILURE;
    }
    std::string output_image = vm["output"].as<std::string>();
    bool found_width = false;
    bool found_height = false;
    uint64_t width = 0;
    uint64_t height = 0;

    // Check if width or height is provided
    if (vm.count("width") == 1)
    {
        width = vm["width"].as<uint64_t>();
        found_width = true;
    }
    if (vm.count("height") == 1)
    {
        height = vm["height"].as<uint64_t>();
        found_height = true;
    }

    if (!found_height && !found_width)
    {
        // Search for scale
        if (vm.count("scale") == 1)
        {
            double scale = vm["scale"].as<double>();
            if (scale <= 0.0)
            {
                std::cout << "Error: scale is not valid" << std::endl;
                return EXIT_FAILURE;
            }
            width = static_cast<uint64_t>(image.cols * scale);
            height = static_cast<uint64_t>(image.rows * scale);
            found_width = true;
            found_height = true;
        }
        else
        {
            // Calculate width and height from scale-width and scale-height
            if (vm.count("scale-width") == 1)
            {
                double scale_width = vm["scale-width"].as<double>();
                if (scale_width <= 0.0)
                {
                    std::cout << "Error: scale-width is not valid" << std::endl;
                    return EXIT_FAILURE;
                }
                width = static_cast<uint64_t>(image.cols * scale_width);
                found_width = true;
            }
            if (vm.count("scale-height") == 1)
            {
                double scale_height = vm["scale-height"].as<double>();
                if (scale_height <= 0.0)
                {
                    std::cout << "Error: scale-height is not valid" << std::endl;
                    return EXIT_FAILURE;
                }
                height = static_cast<uint64_t>(image.rows * scale_height);
                found_height = true;
            }
        }
    }
    

    if (!found_width)
    {
        width = static_cast<uint64_t>(image.cols * (static_cast<double>(height) / static_cast<double>(image.rows)));
        found_width = true;
    }
    if (!found_height)
    {
        height = static_cast<uint64_t>(image.rows * (static_cast<double>(width) / static_cast<double>(image.cols)));
        found_height = true;
    }

    if (verbose)
    {
        fprintf(stdout, " Old Dims: %d x %d\n", image.cols, image.rows);
        fprintf(stdout, " New Dims: %" PRIu64 " x %" PRIu64 "\n", width, height);
    }
    // Call resize
    cv::Mat resized_image;
    enum cv::InterpolationFlags interpolation_method;
    if (method == "nearest")
        interpolation_method = cv::INTER_NEAREST;
    else if (method == "linear")
        interpolation_method = cv::INTER_LINEAR;
    else if (method == "cubic")
        interpolation_method = cv::INTER_CUBIC;
    else if (method == "area")
        interpolation_method = cv::INTER_AREA;
    else if (method == "lanczos4")
        interpolation_method = cv::INTER_LANCZOS4;
    else
    {
        std::cout << "Error: interpolation method is not valid" << std::endl;
        return EXIT_FAILURE;
    }

    if (verbose)
    {
        fprintf(stdout, "Resizing image...\n");
        // Start timer, measure time as a double in milliseconds
        auto start = std::chrono::high_resolution_clock::now();
        cv::resize(image, resized_image, cv::Size(width, height), 0, 0, interpolation_method);
        // Stop timer
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        fprintf(stdout, "Resizing elapsed time: %lf ms\n", elapsed.count() * 1000.0);
        resize_time = elapsed.count();
    }

    // Write the output image
    bool write_success = false;
    if (verbose)
    {
        fprintf(stdout, "Writing output image...\n");
        // Start timer, measure time as a double in milliseconds
        auto start = std::chrono::high_resolution_clock::now();
        write_success = cv::imwrite(output_image, resized_image);
        // Stop timer
        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        if (write_success)
            fprintf(stdout, "Writing elapsed time: %lf ms\n", elapsed.count() * 1000.0);

        write_time = elapsed.count();
    }
    else
        write_success = cv::imwrite(output_image, resized_image);

    if (!write_success)
    {
        std::cout << "Error: output image could not be written" << std::endl;
        return EXIT_FAILURE;
    }

    // Stop global timer
    auto finish_global = std::chrono::high_resolution_clock::now();

    if (verbose)
    {
        std::chrono::duration<double> elapsed_global = finish_global - start_global;

        // Print timing information. take ratios of times
        fprintf(stdout, "Timing information:\n");
        fprintf(stdout, " Read time: %lf%%\n", 100 * read_time / elapsed_global.count());
        fprintf(stdout, " Resize time: %lf%%\n", 100 * resize_time / elapsed_global.count());
        fprintf(stdout, " Write time: %lf%%\n", 100 * write_time / elapsed_global.count());
        fprintf(stdout, "Total elapsed time: %lf s\n", elapsed_global.count());

    }
    return EXIT_SUCCESS;
}




