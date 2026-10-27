#pragma once

#ifndef PDFPP_UTIL_H
#define PDFPP_UTIL_H

#include <vector>
#include <string>

namespace pdfpp::util {
    std::vector<std::string> split(std::string s, const std::string& delimiter);
}

#endif