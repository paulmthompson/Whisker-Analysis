
#include "Geometry/lines.hpp"

#include <string>
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


template<typename T>
std::vector<float> parse_string_to_float_vector(T const & str);

template<typename T>
Line2D parse_string_to_line(T const & xstr, T const & ystr);

std::vector<std::pair<int, std::vector<Line2D>>> load_line_csv(std::string const & filepath, bool verbose = false);

void save_lines_csv(
        std::vector<std::pair<int, std::vector<Line2D>>> const & data,
        std::string const & filename,
        std::string const & header = "Frame,X,Y");

extern template std::vector<float> parse_string_to_float_vector(std::string const & str);
extern template std::vector<float> parse_string_to_float_vector(std::string_view const & str);

extern template Line2D parse_string_to_line(std::string const & xstr, std::string const & ystr);
extern template Line2D parse_string_to_line(std::string_view const & xstr, std::string_view const & ystr);

}// namespace whisker