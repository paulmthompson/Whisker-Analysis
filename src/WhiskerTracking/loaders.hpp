
#include "Geometry/lines.hpp"

#include <charconv>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace whisker {

/*
inline std::vector<float> parse_string_to_float_vector(const std::string& str) {
    std::vector<float> result;

    std::stringstream ss(str);
    std::string item;

    while (std::getline(ss, item, ',')) {
        result.push_back(std::stof(item));
    }
    return result;
}
*/

// Tokenize function using std::string_view
std::string_view stringview_getline(
    std::string_view str, 
    char delimiter,
    size_t & start) 
{
    size_t end = str.find(delimiter, start);

    auto sub_str = str.substr(start, end - start);
    start = end + 1;
    return sub_str;
}

template <typename T>
inline std::vector<float> parse_string_to_float_vector(const T& str) {
    std::vector<float> result;
    const char* start = str.data();
    const char* end = str.data() + str.size();

    while (start < end) {
        float value;
        auto [ptr, ec] = std::from_chars(start, end, value);
        if (ec == std::errc()) {
            result.push_back(value);
            start = ptr;
            if (start < end && *start == ',') {
                ++start; // Skip the comma
            }
        } else {
            // Handle error if needed
            break;
        }
    }
    return result;
}

template <typename T>
inline Line2D parse_string_to_line(const T& xstr, const T& ystr) {
    Line2D line;

    const char* startx = xstr.data();
    const char* endx = xstr.data() + xstr.size();

    while (startx < endx) {
        float value;
        auto [ptr, ec] = std::from_chars(startx, endx, value);
        if (ec == std::errc()) {
            line.emplace_back(Point2D<float>{value,0.0});
            startx = ptr;
            if (startx < endx && *startx == ',') {
                ++startx; // Skip the comma
            }
        } else {
            // Handle error if needed
            break;
        }
    }

    const char* starty = ystr.data();
    const char* endy = ystr.data() + ystr.size();

    int i = 0;
    while (starty < endy) {
        float value;
        auto [ptr, ec] = std::from_chars(starty, endy, value);
        if (ec == std::errc()) {
            line[i].y = value;
            ++i;
            starty = ptr;
            if (starty < endy && *starty == ',') {
                ++starty; // Skip the comma
            }
        } else {
            // Handle error if needed
            break;
        }
    }
    
    return line;
}

inline std::vector<std::pair<int, std::vector<Line2D>>> load_line_csv(const std::string& filepath, bool verbose = false)
{
    auto t1 = std::chrono::high_resolution_clock::now();

    std::vector<std::pair<int, std::vector<Line2D>>> data;
    std::vector<int> frame_nums;

    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }

    std::string line;

    int loaded_lines = 0;
    int current_frame = -1;
    while (std::getline(file, line)) {
        
        std::string_view line_view(line);

        size_t pos = 0;

        auto frame_num_str = stringview_getline(line_view, ',', pos);

        if (frame_num_str.find_first_not_of("0123456789") != std::string::npos) {
            continue; // Skip the header line
        }

        auto x_str = stringview_getline(line_view, '"', pos);
        x_str = stringview_getline(line_view, '"', pos);
        auto y_str = stringview_getline(line_view, '"', pos);
        y_str = stringview_getline(line_view, '"', pos);

        int frame_num;
        auto result = std::from_chars(frame_num_str.data(), frame_num_str.data() + frame_num_str.size(), frame_num);

        auto line_float = parse_string_to_line(x_str, y_str);

        if (frame_num > current_frame) {
            data.push_back(std::make_pair(frame_num,std::vector<Line2D>()));
            current_frame = frame_num;
        } else if (frame_num < current_frame) {
            throw std::runtime_error("Frame numbers are not in increasing order");
        }

        frame_nums.push_back(frame_num);
        std::get<1>(data.back()).emplace_back(std::move(line_float));
        loaded_lines += 1;
       
    }

    file.close();
    auto t2 = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration<double>( t2 - t1 ).count();
    if (verbose) {
        std::cout << "Loaded " << loaded_lines << " lines from " << filepath << " in " << duration << "s" << std::endl;
    }

    return data;
}

inline void save_lines_csv(
        const std::vector<std::pair<int,std::vector<Line2D>>>& data,
        const std::string& filename,
        const std::string& header = "Frame,X,Y")
{

    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // Write the header
    file << header << "\n";

    char buffer[64];

    // Write the data
    for (const auto& [frame, lines] : data) {
        for (const auto& line : lines) {
            //std::ostringstream x_values;
            //std::ostringstream y_values;
            std::string x_values;
            std::string y_values;

            for (const auto& point : line) {
                //x_values << std::fixed << std::setprecision(1) << point.x << ",";
                //y_values << std::fixed << std::setprecision(1) << point.y << ",";
                int x_len = std::snprintf(buffer, sizeof(buffer), "%.1f,", point.x);
                x_values.append(buffer, x_len);

                int y_len = std::snprintf(buffer, sizeof(buffer), "%.1f,", point.y);
                y_values.append(buffer, y_len);
            }

            // Remove the trailing comma
            if (!x_values.empty()) x_values.pop_back();
            if (!y_values.empty()) y_values.pop_back();

            file << frame << ",\"" << x_values << "\",\"" << y_values << "\"\n";
        }
    }

    file.close();
}


} // namespace whisker