#pragma once

#ifndef PDFPP_OBJECTS_H
#define PDFPP_OBJECTS_H

#include <optional>
#include <string>
#include <vector>

namespace pdfpp {
    struct Boolean {
        std::optional<std::string> label;
        bool value;
    };

    struct Number {
        std::optional<std::string> label;
        long long integer;
        long double real;
    };

    struct String {
        std::optional<std::string> label;
        std::string content;
    };

    struct Name {
        std::optional<std::string> label;
        std::string name;
    };

    struct Array {
        std::optional<std::string> label;
        //Each element is going to be a string, but they are distinct types
        //  and it is up to this library to parse and deduce what type it is
        std::vector<std::string> elements;
    };

    struct Dictionary {
        std::optional<std::string> label;
        //This one needs a lot more thought because the example is confusing me
    };

    struct Null {};

    struct Stream {
        std::optional<std::string> label;
        unsigned long long length;
        std::vector<std::string> filters;
        std::vector<std::string> decodeParams; //Probably shouldn't be represented as a vector of strings for now
    };

    enum Filter {
        ASCIIHexDecode,     //No param
        ASCII85Decode,      //No param
        LZWDecode,          //Optional Dictionary
        RunLengthDecode,    //No param
        CCITTFaxDecode,     //Optional Dictionary
        DCTDecode           //Optional Dictionary
    };

    struct ID {
        uint32_t objectNum;
        uint32_t generationNum;
    };
}

#endif