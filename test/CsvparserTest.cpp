//
// Created by pedro on 3/11/2026.
//
#include <filesystem>
#include <gtest/gtest.h>
#include "../headers/CsvParser.h"

// The function parseIndividualSubmission is going to be private so it must be moved to public for testing purposes
/*
TEST(CsvParserTest, test_parseIndividualSubmissionNoSecondaryField) {
    // The last char is a carriage return because we want to simulate if the code is run on Windows
    // Or in linux
    std::string line = "31, The Eternal Wheel of Reincarnation, Ralph Kholer, kholer@gmail.com, 3,\r";
    Submission s = Submission();
    CSVParser parser("");
    parser.parseIndividualSubmission(line, s);

    EXPECT_EQ(s.getId(), 31);
    EXPECT_EQ(s.getTitle(), "The Eternal Wheel of Reincarnation");
    EXPECT_EQ(s.getAuthor(), "Ralph Kholer");
    EXPECT_EQ(s.getEmail(), "kholer@gmail.com");
    EXPECT_EQ(s.getPrimaryField(), 3);
    EXPECT_EQ(s.getSecondaryField(), 0);
}

TEST(CsvParserTest, test_parseIndividualSubmissionWithSecondaryField) {
    std::string line = "31, The Eternal Wheel of Reincarnation, Ralph Kholer, kholer@gmail.com, 3, 4\r";
    Submission s = Submission();
    CSVParser parser("");
    parser.parseIndividualSubmission(line, s);

    EXPECT_EQ(s.getId(), 31);
    EXPECT_EQ(s.getTitle(), "The Eternal Wheel of Reincarnation");
    EXPECT_EQ(s.getAuthor(), "Ralph Kholer");
    EXPECT_EQ(s.getEmail(), "kholer@gmail.com");
    EXPECT_EQ(s.getPrimaryField(), 3);
    EXPECT_EQ(s.getSecondaryField(), 4);
}

TEST(CsvParserTest, test_parseIndividualReviewerNoSecondaryField) {
    std::string line = "1, Jaqueline N. Chame, jchame@yahoo.com, 1,\r";
    Reviewer r = Reviewer();
    CSVParser parser("");
    parser.parseIndividualReviewer(line, r);
    EXPECT_EQ(r.getId(), 1);
    EXPECT_EQ(r.getName(), "Jaqueline N. Chame");
    EXPECT_EQ(r.getEmail(), "jchame@yahoo.com");
    EXPECT_EQ(r.getPrimaryField(), 1);
    EXPECT_EQ(r.getSecondaryField(), 0);
}

TEST(CsvParserTest, test_parseIndividualReviewerWithSecondaryField) {
    std::string line = "1, Jaqueline N. Chame, jchame@yahoo.com, 1, 4\r";
    Reviewer r = Reviewer();
    CSVParser parser("");
    parser.parseIndividualReviewer(line, r);
    EXPECT_EQ(r.getId(), 1);
    EXPECT_EQ(r.getName(), "Jaqueline N. Chame");
    EXPECT_EQ(r.getEmail(), "jchame@yahoo.com");
    EXPECT_EQ(r.getPrimaryField(), 1);
    EXPECT_EQ(r.getSecondaryField(), 4);
}
*/

TEST(CsvParserTest, test_full_parsing) {
    CSVParser parse("../test/testing.csv");
    parse.parseDocument();
    auto submissions = parse.getSubmissions();
    auto reviewers = parse.getReviewers();
    ASSERT_EQ(submissions.size(), 2);
    ASSERT_EQ(reviewers.size(), 2);

    // Check first submission
    EXPECT_EQ(submissions[0].getId(), 31);
    EXPECT_EQ(submissions[0].getTitle(), "“The Eternal Wheel of Reincarnation”");
    EXPECT_EQ(submissions[0].getAuthor(), "Ralph Kholer");
    EXPECT_EQ(submissions[0].getEmail(), "kholer@gmail.com");
    EXPECT_EQ(submissions[0].getPrimaryField(), 3);
    EXPECT_EQ(submissions[0].getSecondaryField(), 4);

    // Check second submission
    EXPECT_EQ(submissions[1].getId(), 87);
    EXPECT_EQ(submissions[1].getTitle(), "“GoDiva: A PIM Architecture”");
    EXPECT_EQ(submissions[1].getAuthor(), "Jeff Draper");
    EXPECT_EQ(submissions[1].getEmail(), "draper@usc.edu");
    EXPECT_EQ(submissions[1].getPrimaryField(), 1);
    EXPECT_EQ(submissions[1].getSecondaryField(), 0);

    // Check first reviewer
    EXPECT_EQ(reviewers[0].getId(), 1);
    EXPECT_EQ(reviewers[0].getName(), "Jaqueline N. Chame");
    EXPECT_EQ(reviewers[0].getEmail(), "jchame@yahoo.com");
    EXPECT_EQ(reviewers[0].getPrimaryField(), 1);
    EXPECT_EQ(reviewers[0].getSecondaryField(), 0);

    // Check second reviewer
    EXPECT_EQ(reviewers[1].getId(), 2);
    EXPECT_EQ(reviewers[1].getName(), "Mary W. Hall");
    EXPECT_EQ(reviewers[1].getEmail(), "mhall@hotmail.edu");
    EXPECT_EQ(reviewers[1].getPrimaryField(), 1);
    EXPECT_EQ(reviewers[1].getSecondaryField(), 4);
}