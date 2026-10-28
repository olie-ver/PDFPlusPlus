#include <testpp/testpp.hpp>

#include "../include/pdf.hpp"

TEST(PDF, readEOF) {
    pdfpp::pdf pdf("docs/Handbook of Satisfiability.txt");

    pdfpp::pdf pdf2("docs/test.txt");

    pdfpp::pdf pdf3("docs/Learning Core Audio.txt");

    pdfpp::pdf pdf4("docs/pdfreference1.0.txt");

    pdfpp::pdf pdf5("docs/Stardew totes.txt");
}