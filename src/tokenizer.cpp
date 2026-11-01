#include "../include/tokenizer.hpp"

#include "../include/util.hpp"

#include <iostream>

namespace pdfpp {
    tokenizer::tokenizer(const std::filesystem::path path, const std::ios_base::openmode mode, const size_t size) 
        : file(path, mode), bufferSize(size)
    {
        if (!file.is_open()) {
            throw "Couldn't open file\n";
        }

        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        buffer.reserve(size);
    }

    std::string tokenizer::getFirst() {
        if (tokenPos == -1) {
            size_t curPos = static_cast<size_t>(file.tellg());
            size_t bytesLeft = fileSize - curPos;
            size_t bytesToRead = std::min(bytesLeft, bufferSize);
            buffer.resize(bytesToRead);
            file.read(buffer.data(), bytesToRead);

            tokens = util::split(buffer);

            if (!std::iswspace(buffer[tokens[0].size()])) {            
                //read in the next chunks until EOF is reached or until a whitespace character is encountered
                completeNextToken();
            }

            tokenPos = 0;
        }

        // std::cout << "getFirst tokenPos: " << tokenPos << std::endl;

        return tokens[tokenPos];
    }

    std::string tokenizer::getLast() {
        if (tokenPos >= tokens.size() || tokenPos == -1) {
            size_t curPos = static_cast<size_t>(file.tellg());
            size_t bytesLeft = fileSize - curPos;
            size_t bytesToRead = std::min(bytesLeft, bufferSize);
            buffer.resize(bytesToRead);
            file.read(buffer.data(), bytesToRead);

            tokens = util::split(buffer);
            tokenPos = tokens.size() - 1;

            size_t lastTokenPos = buffer.rfind(tokens.back());

            if (!std::iswspace(buffer.back())) {            
                //read in the next chunks until EOF is reached or until a whitespace character is encountered
                completeNextToken();
            }
        }

        // std::cout << "getLast tokenPos: " << tokenPos << std::endl;

        return tokens[tokenPos];
    }

    std::string tokenizer::getNext() {
        std::cout << "TOKENPOS: " << tokenPos << std::endl;
        std::cout << "tokens.size(): " << tokens.size() << std::endl;

        if (file.tellg() == fileSize && tokenPos >= tokens.size() - 1) {
            std::cout << "EOF tokenPos: " << tokenPos << std::endl;
            return "";
        }


        if (tokenPos >= tokens.size() || tokenPos == -1) {
            std::cout << "gettingFirst\n";
            return getFirst();
        }

        //In order to make sure that the next token is valid, is to check that
        //the last character is some form of whitespace

        if (tokenPos + 1 == tokens.size() - 1) { //if the next token is the last token IN THE BUFFER
            std::cout << "Last token" << std::endl;
            if (!std::iswspace(buffer[buffer.size() - 1]) && file.tellg() != fileSize) {
                //read in the next chunks until EOF is reached or until a whitespace character is encountered
                completeNextToken();
            }
        }

        tokenPos++;

        std::cout << "getNext tokenPos: " << tokenPos << std::endl;

        return tokens[tokenPos];
    }

    std::string tokenizer::getPrev() {
        if (file.tellg() == 0 && tokenPos - 1 == -1) {
            std::cout << "getPrev returning empty string\n";
            return "";
        }


        if (tokenPos >= tokens.size() || tokenPos == -1) {
            return getLast();
        }

        if (tokenPos - 1 == 0) { //if the previous token is the last token IN THE BUFFER
            //if the first character isn't whitespace and the file position isn't the end of the first chunk
            if (!std::iswspace(buffer[0]) && file.tellg() != buffer.size()) {
                //go back by however much I can (either BOF or BUFFERSIZE)
                //read that much
                //then seek backwards by that much so that the current position of the tellg matches the buffer

                completePrevToken();
            }
        }

        tokenPos--;

        // std::cout << "getPrev tokenPos: " << tokenPos << std::endl;

        return tokens[tokenPos];
    }

    void tokenizer::completeNextToken() {
        std::cout << "completingNextToken\n";
        accumulator += tokens.back();
        tokenPos = -1;

        std::cout << "accumulator: " << accumulator << std::endl;

        bool tokenIncomplete = true;
        while (tokenIncomplete) {
            size_t curPos = static_cast<size_t>(file.tellg());
            size_t bytesLeft = fileSize - curPos;
            size_t bytesToRead = std::min(bytesLeft, bufferSize);

            if (bytesToRead == 0) {
                std::cout << "No more bytes to read\n";
                tokens[0] = std::move(accumulator);
                accumulator.clear();
                break;
            }

            buffer.resize(bytesToRead);
            file.read(buffer.data(), bytesToRead);
            tokens = util::split(buffer);
            accumulator += tokens[0];
            std::cout << "accumulator after appending: " << accumulator << std::endl;

            if (std::iswspace(buffer[tokens[0].size()]) || file.tellg() == fileSize) {
                tokens[0] = std::move(accumulator);
                accumulator.clear();
                tokenIncomplete = false;
            } 
            // else {
            //     std::cout << "buffer[tokens[0].size()]: " << buffer[tokens[0].size()] << std::endl;
            // }
        }
    }

    void tokenizer::completePrevToken() {
        std::cout << "completePrevToken\n";

        accumulator.insert(0, tokens[0]);
        tokenPos = -1;

        std::cout << "accumulator: " << accumulator << std::endl;

        bool tokenIncomplete = true;
        while (tokenIncomplete) {
            size_t curPos = static_cast<size_t>(file.tellg());
            size_t bytesToRead = std::min(curPos, BUFFERSIZE);

            if (bytesToRead == 0) {
                std::cout << "No more bytes to read\n";
                tokens[0] = std::move(accumulator);
                accumulator.clear();
                break;
            }

            buffer.resize(bytesToRead);

            std::cout << "file.tellg(): " << file.tellg() << std::endl;

            file.seekg(-bytesToRead, std::ios::cur);

            std::cout << "file.tellg(): " << file.tellg() << std::endl;

            file.read(buffer.data(), bytesToRead);

            std::cout << "file.tellg(): " << file.tellg() << std::endl;

            file.seekg(-bytesToRead, std::ios::cur);

            tokens = util::split(buffer);
            accumulator.insert(0, tokens.back());
            std::cout << "accumulator after prepending: " << accumulator << std::endl;

            //Need to find the character before the start of the last token and see if it's whitespace
            size_t lastTokenPos = buffer.rfind(tokens.back());
            std::cout << "file.tellg(): " << file.tellg() << std::endl;
            std::cout << "lastTokenPos: " << lastTokenPos << std::endl;
            
            if (file.tellg() == 0 || std::iswspace(buffer[lastTokenPos - 1])) {
                tokens.back() = std::move(accumulator);
                accumulator.clear();
                tokenIncomplete = false;
            } 
        }

        tokenPos = tokens.size();
    }
}