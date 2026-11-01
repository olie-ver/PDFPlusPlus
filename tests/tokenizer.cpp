#include <testpp/testpp.hpp>

#include "../include/tokenizer.hpp"
#include "../include/util.hpp"

#include <array>
#include <fstream>

// TEST(tokenizer, getFirst) {
//     pdfpp::tokenizer tokenizer{"tests/tokenizer/500Bytes.txt"};

//     ASSERT_STR_EQ(tokenizer.getFirst(), "Lorem");
// }

// TEST(tokenizer, getLast) {
//     pdfpp::tokenizer tokenizer{"tests/tokenizer/500Bytes.txt"};

//     ASSERT_STR_EQ(tokenizer.getLast(), "dui.");
// }

// TEST(tokenizer, getNext) {
//     pdfpp::tokenizer tokenizer{"tests/tokenizer/500Bytes.txt"};

//     ASSERT_STR_EQ(tokenizer.getNext(), "Lorem");
//     ASSERT_STR_EQ(tokenizer.getNext(), "ipsum");
//     ASSERT_STR_EQ(tokenizer.getNext(), "dolor");
//     ASSERT_STR_EQ(tokenizer.getNext(), "sit");
//     ASSERT_STR_EQ(tokenizer.getNext(), "amet,");
// } 

// TEST(tokenizer, getPrev) {
//     pdfpp::tokenizer tokenizer{"tests/tokenizer/500Bytes.txt"};

//     ASSERT_STR_EQ(tokenizer.getPrev(), "dui.");
//     ASSERT_STR_EQ(tokenizer.getPrev(), "dui");
//     ASSERT_STR_EQ(tokenizer.getPrev(), "at");
//     ASSERT_STR_EQ(tokenizer.getPrev(), "sapien");
//     ASSERT_STR_EQ(tokenizer.getPrev(), "lacinia");
// } 

// TEST(tokenizer, getPrevNextMix) {
//     pdfpp::tokenizer tokenizer{"tests/tokenizer/500Bytes.txt"};

//     // std::cout << "getPrevNextMix:" << std::endl;
//     ASSERT_STR_EQ(tokenizer.getFirst(), "Lorem");
//     ASSERT_STR_EQ(tokenizer.getNext(), "ipsum");
//     ASSERT_STR_EQ(tokenizer.getPrev(), "Lorem");
//     // std::cout << "End" << std::endl;
// }

// TEST(tokenizer, splitNextToken) {
//     pdfpp::tokenizer tokenizer{"tests/tokenizer/500Bytes.txt", 8U, 200};

//     std::string buffer;

//     buffer.resize(200);
//     std::ifstream fileStream("tests/tokenizer/500Bytes.txt");
//     fileStream.read(buffer.data(), 200);
//     ASSERT_EQ(fileStream.gcount(), 200);

//     //For this particular test, the last token before a chunk split is "libero."

//     // std::cout << buffer.data() << std::endl;

//     std::string token;

//     while (token != "libero.") {
//         token = tokenizer.getNext();
//     }

//     //And the token after that is "Vivamus"
//     ASSERT_STR_EQ(tokenizer.getNext(), "Vivamus");
// }

// TEST(tokenizer, nextTokenLargerThanBuffer) {
//     pdfpp::tokenizer tokenizer{"tests/tokenizer/512ByteToken.txt", 8U, 100};
//     std::string result(512, 'A');
//     ASSERT_EQ(tokenizer.getNext(), result);

//     pdfpp::tokenizer tokenizer2{"tests/tokenizer/512ByteToken.txt", 8U, 100};
//     ASSERT_EQ(tokenizer2.getFirst(), result);
// }

// TEST(tokenizer, readPastEnd) {
//     pdfpp::tokenizer tokenizer{"tests/tokenizer/500Bytes.txt", 8U, 100};

//     std::string buffer;
//     std::ifstream fileStream("tests/tokenizer/500Bytes.txt");
//     buffer.resize(500);
//     fileStream.read(buffer.data(), 500);
//     ASSERT_EQ(fileStream.gcount(), 500);

//     const std::vector<std::string>& tokens = pdfpp::util::split(buffer);

//     for (const auto& token : tokens) {
//         // std::cout << tokenizer.getNext() << std::endl;
//         ASSERT_STR_EQ(tokenizer.getNext(), token);
//     }

//     ASSERT_STR_EMT(tokenizer.getNext());
// }

// TEST(tokenizer, readPastBeginning) {
//     pdfpp::tokenizer tokenizer{"tests/tokenizer/500Bytes.txt", 8U, 100};
    
//     ASSERT_STR_EQ(tokenizer.getFirst(), "Lorem");
//     ASSERT_STR_EMT(tokenizer.getPrev());

//     tokenizer.getLast();
//     tokenizer.getNext();
//     ASSERT_STR_NEMT(tokenizer.getPrev());
// }

// TEST(tokenizer, splitPrevToken) {
//     //Need to figure out how to write the test properly
//     pdfpp::tokenizer tokenizer{"tests/tokenizer/500Bytes.txt", 8U, 200};

//     std::string buffer;

//     buffer.resize(200);
//     std::ifstream fileStream("tests/tokenizer/500Bytes.txt");
//     fileStream.read(buffer.data(), 200);
//     ASSERT_EQ(fileStream.gcount(), 200);

//     //For this particular test, the last token before a chunk split is "et"

//     // std::cout << buffer.data() << std::endl;

//     std::string token;

//     while (token != "et") {
//         token = tokenizer.getPrev();
//     }

//     //And the token after that is "Vivamus"
//     ASSERT_STR_EQ(tokenizer.getPrev(), "lectus");
// }

TEST(tokenizer, prevTokenLargerThanBuffer) {
    pdfpp::tokenizer tokenizer{"tests/tokenizer/512ByteToken.txt", 8U, 100};
    std::string result(512, 'A');
    // ASSERT_EQ(tokenizer.getPrev(), result);

    pdfpp::tokenizer tokenizer2{"tests/tokenizer/512ByteToken.txt", 8U, 100};
    ASSERT_EQ(tokenizer2.getLast(), result);
}
