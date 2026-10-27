#include <testpp/testpp.hpp>

#include "../include/util.hpp"

#include <vector>
#include <string>

TEST(Util, Split) {
    std::string view = "The big brown dog jumps over the moon";

    const std::vector<std::string>& tokens = pdfpp::util::split(view, " ");

    ASSERT_STR_EQ(tokens[0], "The");
    ASSERT_STR_EQ(tokens[1], "big");
    ASSERT_STR_EQ(tokens[2], "brown");
    ASSERT_STR_EQ(tokens[3], "dog");
    ASSERT_STR_EQ(tokens[4], "jumps");
    ASSERT_STR_EQ(tokens[5], "over");
    ASSERT_STR_EQ(tokens[6], "the");
    ASSERT_STR_EQ(tokens[7], "moon");
}