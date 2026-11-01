#pragma once

#ifndef PDFPP_H
#define PDFPP_H

//PDFs since version 1.5 do not need the trailer keyword: https://community.adobe.com/questions-9/check-the-presence-of-header-body-and-trailer-in-pdfs-1265955
//For now we will say that it needs a trailer because this obviously can't handle streams

#include "trailer.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <list>
#include <unordered_map>

#define PDF_EOF "%%EOF"
#define STARTXREF "startxref"
#define XREF "xref"
#define TRAILER "trailer"
#define DIGITS32 10
#define DIGITS64 20
#define DIGITS128 39

#define BUFFERSIZE 1024UL

namespace pdfpp {
    /// @brief Keywords
    inline static constinit std::array<char, 8> kw_trailer = std::to_array(TRAILER);
    inline static constinit std::array<char, 6> kw_eof = std::to_array(PDF_EOF);
    inline static constinit std::array<char, 10> kw_startxref = std::to_array(STARTXREF);
    inline static constinit std::array<char, 5> kw_xref = std::to_array(XREF);

    class pdf {
        public:
            pdf(std::filesystem::path path);

            size_t CRT;
        private:
            void parseCRT(size_t crt_start, size_t startxrefPos);

            /// @brief An istream the pdf file
            std::ifstream file;
            const size_t fileSize;
            std::filesystem::path path;

            std::string buffer;

            /// @brief a list of trailers in the pdf file with the first entry being the most recent and the 
            //  last entry being the least recent entry in the file
            std::list<trailer> entries;

            //I need an unordered_map from object IDs to actual objects
    };
}

#endif