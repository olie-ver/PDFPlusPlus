#include "../include/util.hpp"
#include <sstream>

namespace pdfpp::util {
    // std::vector<std::string> split(std::string s, const std::string& delimiter) {
    //     std::vector<std::string> tokens;
    //     size_t pos = 0;
    //     std::string token;
    //     while ((pos = s.find(delimiter)) != std::string::npos) {
    //         token = s.substr(0, pos);
    //         tokens.push_back(token);
    //         s.erase(0, pos + delimiter.length());
    //     }

    //     if (!s.empty()) {
    //         tokens.push_back(s);
    //     }

    //     return tokens;
    // }

    std::vector<std::string> split(std::string s) {
        std::istringstream buffer(s);
        std::vector<std::string> ret((std::istream_iterator<std::string>(buffer)), std::istream_iterator<std::string>());
        return ret;
    }
}