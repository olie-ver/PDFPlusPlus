#include "../include/pdf.hpp"
#include "../include/util.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>

namespace pdfpp {
    pdf::pdf(std::filesystem::path path) : file(path, std::ios::binary | std::ios::ate), fileSize(file.tellg()) {
        //set up
        if (!file.is_open()) {
            std::cerr << "Couln't open file\n" << std::endl;
            throw "Couldn't open file\n";
        }
        buffer.reserve(BUFFERSIZE);
        std::cout << fileSize << std::endl;

        long long tempFileSize = fileSize;

        //read in initial chunk
        size_t seekBack = std::min(static_cast<size_t>(tempFileSize), BUFFERSIZE);
        buffer.resize(seekBack);
        file.seekg(-seekBack, std::ios::cur);
        file.read(buffer.data(), seekBack);
        tempFileSize = std::max(0LL, static_cast<long long>(tempFileSize - BUFFERSIZE));
        
        //If the trailer can't be found, read backwards until it's found
        size_t trailerPos = 0;
        while ((trailerPos = buffer.rfind("trailer")) == std::string::npos) {
            if (fileSize <= 0) {
                std::cerr << "Missing trailer\n" << std::endl;
                throw ("PDF file is missing its trailer\n");
            }

            //to prevent skipping over the trailer (suppose it's partially read) between buffers,
            //  we will have an overlap by the size of "trailer" (8) between buffers
            seekBack = std::min(static_cast<size_t>(fileSize), BUFFERSIZE - kw_trailer.size());
            buffer.resize(seekBack);
            file.seekg(-seekBack, std::ios::cur);
            file.read(buffer.data(), seekBack);
            tempFileSize = std::max(0LL, static_cast<long long>(fileSize - BUFFERSIZE - kw_trailer.size()));
            std::cout << tempFileSize << std::endl;

            std::cout << "trailerPos: " << std::endl;
        }

        //I should really have the file seek to the trailer's position in the buffer

        parseTrailer(trailerPos);
    }


    void pdf::parseTrailer(size_t start) {
        size_t trailerEnd = buffer.find(TRAILER_END);

        //I highly doubt that the trailer is going to be larger than 1KB
        if (trailerEnd == std::string::npos) {
            std::cout << "Couldn't find trailer's end\n" << std::endl;
            throw "Couldn't find the end of the trailer\n";
        }

        std::string trailerBuf(buffer.begin() + start, buffer.end() + trailerEnd);

        const std::vector<std::string>& tokens = util::split(trailerBuf, " ");

        trailer trailer{0, static_cast<size_t>(-1), {0, 0}, {0, 0}};

        for (size_t i = 0; i < tokens.size(); i++) {
            if (tokens[i] == SIZE) {
                //get the next token
                trailer.size = std::stoi(tokens[++i]);
                i++;
            } else if (tokens[i] == ROOT) {
                //get the next two tokens
                trailer.root.object_num = std::stoi(tokens[++i]);
                trailer.root.generation_num = std::stoi(tokens[++i]);
                if (tokens[++i] != "R") {
                    std::cerr << "Root reference does not contain required 'R' key\n";
                }
            } else if (tokens[i] == PREV) {
                //get the next token
                trailer.prev = std::stoi(tokens[++i]);
            } else if (tokens[i] == INFO) {
                //get the next token
                trailer.info.object_num = std::stoi(tokens[++i]);
                trailer.info.generation_num = std::stoi(tokens[++i]);
                if (tokens[++i] != "R") {
                    std::cerr << "Root reference does not contain required 'R' key\n";
                }
            }
        }

        entries.push_back(trailer);

        if (trailer.prev != -1) {
            file.seekg(trailer.prev, std::ios::beg);
            size_t curPos = file.tellg();
            //Read either nothing or the rest of the file after the current position
            size_t maxRead = std::max(0LL, static_cast<long long>(fileSize - curPos));

            //Clamp that number between 0 and the size of the buffer
            size_t numRead = std::clamp(maxRead, 0UL, BUFFERSIZE);
            file.read(buffer.data(), numRead);
            buffer.resize(numRead);

            //Now search from the start to the end just in case the buffer contains two trailers (unlikely but still)
            size_t trailerPos = buffer.find(TRAILER);
            
            //parse the trailer at the new trailer position, which exists because there was a /Prev
            parseTrailer(trailerPos);
        }
    }
}