#include <testpp/testpp.hpp>

#include "../include/pdf.hpp"

TEST(PDF, readEOF) {
    pdfpp::pdf pdf("/Users/oliverlie/Downloads/Stardew totes.pdf");
    // ASSERT_EQ(pdf.crt, 4061253);

    pdfpp::pdf pdf2("/Users/oliverlie/Downloads/test.txt");
    // ASSERT_EQ(pdf2.crt, 478);

    pdfpp::pdf pdf3("/Users/oliverlie/Downloads/SATHandbook-CDCL.pdf");
    // ASSERT_EQ(pdf3.crt, 268202);

    pdfpp::pdf pdf4("/Users/oliverlie/Downloads/pseudocode-algorithms.pdf");
    // ASSERT_EQ(pdf4.crt, 160270);

    // pdfpp::pdf pdf5("/Users/oliverlie/Downloads/Algorithms Textbook.txt");
    // ASSERT_EQ(pdf5.crt, 116);
}