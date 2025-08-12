
#include "loaders.hpp"

#include "fast_float/fast_float.h"

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string_view>

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
        size_t & start) {
    size_t end = str.find(delimiter, start);

    auto sub_str = str.substr(start, end - start);
    start = end + 1;
    return sub_str;
}

template<typename T>
std::vector<float> parse_string_to_float_vector(T const & str) {
    std::vector<float> result;
    char const * start = str.data();
    char const * end = str.data() + str.size();

    while (start < end) {
        float value;
        auto [ptr, ec] = fast_float::from_chars(start, end, value);
        if (ec == std::errc()) {
            result.push_back(value);
            start = ptr;
            if (start < end && *start == ',') {
                ++start;// Skip the comma
            }
        } else {
            // Handle error if needed
            break;
        }
    }
    return result;
}

template<typename T>
Line2D parse_string_to_line(T const & xstr, T const & ystr) {
    Line2D line;

    char const * startx = xstr.data();
    char const * endx = xstr.data() + xstr.size();

    while (startx < endx) {
        float value;
        auto [ptr, ec] = fast_float::from_chars(startx, endx, value);
        if (ec == std::errc()) {
            line.emplace_back(Point2D<float>{value, 0.0});
            startx = ptr;
            if (startx < endx && *startx == ',') {
                ++startx;// Skip the comma
            }
        } else {
            // Handle error if needed
            break;
        }
    }

    char const * starty = ystr.data();
    char const * endy = ystr.data() + ystr.size();

    int i = 0;
    while (starty < endy) {
        float value;
        auto [ptr, ec] = fast_float::from_chars(starty, endy, value);
        if (ec == std::errc()) {
            line[i].y = value;
            ++i;
            starty = ptr;
            if (starty < endy && *starty == ',') {
                ++starty;// Skip the comma
            }
        } else {
            // Handle error if needed
            break;
        }
    }

    return line;
}

std::vector<std::pair<int, std::vector<Line2D>>> load_line_csv(std::string const & filepath, bool verbose) {
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
            continue;// Skip the header line
        }

        auto x_str = stringview_getline(line_view, '"', pos);
        x_str = stringview_getline(line_view, '"', pos);
        auto y_str = stringview_getline(line_view, '"', pos);
        y_str = stringview_getline(line_view, '"', pos);

        int frame_num;
        auto result = fast_float::from_chars(frame_num_str.data(), frame_num_str.data() + frame_num_str.size(), frame_num);

        auto line_float = parse_string_to_line(x_str, y_str);

        if (frame_num > current_frame) {
            data.push_back(std::make_pair(frame_num, std::vector<Line2D>()));
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

    auto duration = std::chrono::duration<double>(t2 - t1).count();
    if (verbose) {
        std::cout << "Loaded " << loaded_lines << " lines from " << filepath << " in " << duration << "s" << std::endl;
    }

    return data;
}

int count_digits(float input) {
    int count = 0;
    while (input > 1) {
        input = input / 10;
        count++;
    }
    return count;
}

void save_lines_csv(
        std::vector<std::pair<int, std::vector<Line2D>>> const & data,
        std::string const & filename,
        std::string const & header) {

    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }

    // Write the header
    file << header << "\n";

    char buffer[64];

    // Write the data
    for (auto const & [frame, lines]: data) {
        for (auto const & line: lines) {
            //std::ostringstream x_values;
            //std::ostringstream y_values;

            //I know the length of x_values and y_values by the number of digitals in each
            //float

            auto x_buffer_size = 0;
            auto y_buffer_size = 0;

            for (auto const & point: line) {
                x_buffer_size += count_digits(point.x);
                y_buffer_size += count_digits(point.y);

                x_buffer_size += 1;// decimal point
                y_buffer_size += 1;// decimal point

                x_buffer_size += 2;// decimal places
                y_buffer_size += 2;// decimal places

                x_buffer_size += 1;// comma
                y_buffer_size += 1;// comma
            }

            std::string x_values;
            x_values.reserve(x_buffer_size);
            std::string y_values;
            y_values.reserve(y_buffer_size);

            for (auto const & point: line) {

                int x_len = std::snprintf(buffer, sizeof(buffer), "%.1f,", static_cast<double>(point.x));
                x_values.append(buffer, x_len);

                int y_len = std::snprintf(buffer, sizeof(buffer), "%.1f,", static_cast<double>(point.y));
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

template std::vector<float> parse_string_to_float_vector(std::string const & str);
template std::vector<float> parse_string_to_float_vector(std::string_view const & str);

template Line2D parse_string_to_line(std::string const & xstr, std::string const & ystr);
template Line2D parse_string_to_line(std::string_view const & xstr, std::string_view const & ystr);

}// namespace whisker