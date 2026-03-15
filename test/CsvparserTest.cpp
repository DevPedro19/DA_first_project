//
// Created by pedro on 3/11/2026.
//
#include "../headers/CsvParser.h"
#include <gtest/gtest.h>
#include <stdexcept>

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

TEST(CsvParserTest, throwsOnInvalidSubmissionId) {
    CSVParser parser("../test/invalid_submission_id.csv");
    EXPECT_THROW(parser.parseDocument(), std::domain_error);
}

TEST(CsvParserTest, throwsOnInvalidSubmissionPrimary) {
    CSVParser parser("../test/invalid_submission_primary.csv");
    EXPECT_THROW(parser.parseDocument(), std::domain_error);
}

TEST(CsvParserTest, throwsOnInvalidSubmissionSecondary) {
    CSVParser parser("../test/invalid_submission_secondary.csv");
    EXPECT_THROW(parser.parseDocument(), std::domain_error);
}

TEST(CsvParserTest, throwsOnDuplicateSubmissionIds) {
    CSVParser parser("../test/duplicate_submission_ids.csv");
    EXPECT_THROW(parser.parseDocument(), std::invalid_argument);
}

TEST(CsvParserTest, throwsOnDuplicateReviewerIds) {
    CSVParser parser("../test/duplicate_reviewer_ids.csv");
    EXPECT_THROW(parser.parseDocument(), std::invalid_argument);
}
