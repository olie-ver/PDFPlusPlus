#pragma once

#ifndef PDFPP_TOKENIZER_H
#define PDFPP_TOKENIZER_H

#include <cctype>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#define BUFFERSIZE 1024UL

namespace pdfpp {
    //So far it does not return the empty string when there are no more tokens to consume
    class tokenizer {
        public:
            tokenizer(const std::filesystem::path path, const std::ios_base::openmode mode = std::ios_base::in, const size_t size = BUFFERSIZE);
            //Gets the first token in the buffer
            std::string getFirst();
            //Gets the last token in the buffer
            std::string getLast();
            //Gets the next token in the buffer, handles rereading and moving the file if needed
            //  Returns the empty string if it reaches the EOF
            std::string getNext();
            //Gets the previous token in the buffer, handles rereading and moving the file if needed,
            //  Returns the empty string if it reaches the beginning of file
            std::string getPrev();

        private:
            //called when there's a token split across file chunks
            void completeNextToken();

            //called when there's a token split across file chunks
            void completePrevToken();

            //The file
            std::ifstream file;
            //The size of the buffer
            const size_t bufferSize;
            //The file size
            size_t fileSize;
            //The position of which index the most recently grabbed token came from
            //  a tokenPos of -1 or OOB (out of bounds) is obviously invalid 
            //  and will need to be made valid
            size_t tokenPos = -1;
            //The buffer containing the contents of the file
            std::string buffer;
            //A buffer for when a token spans across multiple reads
            std::string accumulator;
            //The tokens of the buffer
            std::vector<std::string> tokens;
    };
}

#endif