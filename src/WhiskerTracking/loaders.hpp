
#include "Geometry/lines.hpp"

#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

namespace whisker {

inline std::vector<float> parse_string_to_float_vector(const std::string& str) {
    std::vector<float> result;

    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, ',')) {
        result.push_back(std::stof(item));
    }
    return result;
}

inline std::map<int, std::vector<Line2D>> load_line_csv(const std::string& filepath)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    std::map<int, std::vector<Line2D>> data_map;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }

    std::string line;

    int loaded_lines = 0;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string frame_num_str, x_str, y_str;

        std::getline(ss, frame_num_str, ',');

        if (frame_num_str == "Frame") {
            continue;
            // Skip the header line
        }

        std::getline(ss, x_str, '"');
        //std::vector<float> x_values = parse_stream_to_float_vector(ss);
        std::getline(ss, x_str, '"');


        std::getline(ss, y_str, '"');
        std::getline(ss, y_str, '"');

        int frame_num = std::stoi(frame_num_str);

        std::vector<float> x_values = parse_string_to_float_vector(x_str);
        std::vector<float> y_values = parse_string_to_float_vector(y_str);

        if (x_values.size() != y_values.size()) {
            std::cerr << "Mismatched x and y values at frame: " << frame_num << std::endl;
            continue;
        }

        if (data_map.find(frame_num) == data_map.end()) {
            data_map[frame_num] = std::vector<Line2D>();
        }

        data_map[frame_num].emplace_back(create_line(x_values, y_values));
        loaded_lines += 1;
       
    }

    file.close();
    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration<double>( t2 - t1 ).count();
    std::cout << "Loaded " << loaded_lines << " lines from " << filepath << " in " << duration << "ms" << std::endl;

    return data_map;
}
} // namespace whisker