#include "../include/pdf.hpp"
#include "../include/util.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>

namespace pdfpp {
    pdf::pdf(std::filesystem::path path) : file(path, std::ios::binary | std::ios::ate), fileSize(static_cast<size_t>(file.tellg())),
        path(path)
    {
        //set up
        if (!file.is_open()) {
            std::cerr << "Couln't open file\n" << std::endl;
            throw "Couldn't open file\n";
        }
        buffer.reserve(BUFFERSIZE);

        constexpr size_t overlap = kw_startxref.size() - 1;

        size_t xrefOffset = 0;
        bool found = false;

        while (!found) {
            //The number of bytes to read is the minimum between the fileSize - xrefOffset and BUFFERSIZE
            size_t seekBack = std::min(fileSize - xrefOffset, BUFFERSIZE);
            buffer.resize(seekBack);
            file.seekg(-seekBack, std::ios::cur);
            file.read(buffer.data(), seekBack);

            size_t findPos = 0;
            if ((findPos = buffer.rfind(STARTXREF)) == std::string::npos) {
                //xrefOffset = BUFFERSIZE - overlap so that if we partially read in "startxref", 
                //  we can read it fully on the next read
                xrefOffset += BUFFERSIZE - overlap;
            } else {
                found = true;
                size_t chunkStart = fileSize - xrefOffset - seekBack;
                size_t startXrefPos = chunkStart + findPos;

                file.seekg(startXrefPos, std::ios::beg);
                size_t finalRead = std::min(fileSize - startXrefPos, BUFFERSIZE);
                buffer.resize(finalRead);
                file.read(buffer.data(), static_cast<std::streamsize>(finalRead));
            }
        }

        //Now that we have startxref, the CRT start, and %%EOF, we can now tokenize it
        const std::vector<std::string>& tokens = util::split(buffer, " ");
        for (size_t i = 0; i < tokens.size(); i++) {
            if (tokens[i] == STARTXREF && tokens[i + 2] == PDF_EOF) {
                size_t crt_start = std::stoi(tokens[i + 1]);
                break;
            }
        }
    }
}