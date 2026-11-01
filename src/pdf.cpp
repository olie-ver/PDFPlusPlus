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
        size_t startXrefPos = 0; //the begin of where we see "startxref"
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
                startXrefPos = chunkStart + findPos;

                file.seekg(startXrefPos, std::ios::beg);
                size_t finalRead = std::min(fileSize - startXrefPos, BUFFERSIZE);
                buffer.resize(finalRead);
                file.read(buffer.data(), static_cast<std::streamsize>(finalRead));
            }
        }

        size_t crt_start = 0;
        //Now that we have startxref, the CRT start, and %%EOF, we can now tokenize it
        const std::vector<std::string>& tokens = util::split(buffer);
        for (size_t i = 0; i < tokens.size(); i++) {
            if (tokens[i] == STARTXREF && tokens[i + 2] == PDF_EOF) {
                crt_start = std::stoi(tokens[i + 1]);
                break;
            }
        }

        parseCRT(crt_start, startXrefPos);
    }

    void pdf::parseCRT(size_t crt_start, size_t startxrefPos) {
        file.seekg(crt_start, std::ios::beg); //move the file to the start of the crt
        size_t curPos = crt_start;
        while (curPos < startxrefPos) { //while we haven't gone past or met "startxref"
            size_t bytesToRead = std::min(startxrefPos - curPos, BUFFERSIZE); //read in the maximum number of bytes without going past the file
            buffer.resize(bytesToRead); //resize the buffer
            file.read(buffer.data(), bytesToRead); //read in that many bytes
            curPos += bytesToRead; //move the curPos by the amount of bytes we've read

            std::vector<std::string> tokens = util::split(buffer);

            size_t firstEntryNum;
            size_t numEntries;
            bool parsingEntries = false;
            for (size_t i = 0; i < tokens.size(); i++) {
                if (tokens[i] == XREF) {
                    firstEntryNum = std::stoi(tokens[++i]);
                    numEntries = std::stoi(tokens[++i]);
                    parsingEntries = true;
                }

                if (parsingEntries) {
                    //if there are less than three tokens
                    if (i >= tokens.size() - 3) {
                        //move the file backwards by the amount of characters in 1 line
                        //read in as many bytes as allowed
                        //adjust curPos
                        //continue 
                    }
                }

                //Each entry is: 10 digit number, 5 digit number, f or n
            }
        }
    }
}