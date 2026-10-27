#pragma once

#ifndef PDFPP_TRAILER_H
#define PDFPP_TRAILER_H

#include <array>

#define SIZE "/Size"
#define ROOT "/Root"
#define PREV "/Prev"
#define INFO "/Info"

#define TRAILER_END ">>"

namespace pdfpp {
    /// @brief Keywords
    inline static constinit std::array<char, 6> kw_size = std::to_array(SIZE);
    inline static constinit std::array<char, 6> kw_root = std::to_array(ROOT);
    inline static constinit std::array<char, 6> kw_prev = std::to_array(PREV);
    inline static constinit std::array<char, 6> kw_info = std::to_array(INFO);

    struct ref {
        size_t object_num;

        size_t generation_num;
    };

    struct trailer {
        size_t size;

        /// @brief the byte offset of the previous CRT in the file
        /// a value of -1 means that there is not previous
        size_t prev;
        
        ref root;

        ref info;
    };
}

#endif