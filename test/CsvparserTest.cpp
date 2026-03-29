//
// Created by pedro on 3/11/2026.
//
#include "../include/CsvParser.h"
#include <gtest/gtest.h>
#include <stdexcept>

TEST(CsvParserTest, test_full_parsing) {
    CSVParser parse("test/testing.csv");
    Data data;
    parse.parseDocument(data);

    // Verify we have the correct number of submissions and reviewers
    ASSERT_EQ(data.submissions.size(), 2);
    ASSERT_EQ(data.reviewers.size(), 2);

    // Check first submission
    EXPECT_EQ(data.submissions[0].getId(), 31);
    EXPECT_EQ(data.submissions[0].getTitle(), "\"The Eternal Wheel of Reincarnation\"");
    EXPECT_EQ(data.submissions[0].getAuthor(), "Ralph Kholer");
    EXPECT_EQ(data.submissions[0].getEmail(), "kholer@gmail.com");
    EXPECT_EQ(data.submissions[0].getPrimaryField(), 3);
    EXPECT_EQ(data.submissions[0].getSecondaryField(), 4);

    // Check second submission
    EXPECT_EQ(data.submissions[1].getId(), 87);
    EXPECT_EQ(data.submissions[1].getTitle(), "\"GoDiva: A PIM Architecture\"");
    EXPECT_EQ(data.submissions[1].getAuthor(), "Jeff Draper");
    EXPECT_EQ(data.submissions[1].getEmail(), "draper@usc.edu");
    EXPECT_EQ(data.submissions[1].getPrimaryField(), 1);
    EXPECT_EQ(data.submissions[1].getSecondaryField(), 0);

    // Check first reviewer
    EXPECT_EQ(data.reviewers[0].getId(), 1);
    EXPECT_EQ(data.reviewers[0].getName(), "Jaqueline N. Chame");
    EXPECT_EQ(data.reviewers[0].getEmail(), "jchame@yahoo.com");
    EXPECT_EQ(data.reviewers[0].getPrimaryField(), 1);
    EXPECT_EQ(data.reviewers[0].getSecondaryField(), 0);

    // Check second reviewer
    EXPECT_EQ(data.reviewers[1].getId(), 2);
    EXPECT_EQ(data.reviewers[1].getName(), "Mary W. Hall");
    EXPECT_EQ(data.reviewers[1].getEmail(), "mhall@hotmail.edu");
    EXPECT_EQ(data.reviewers[1].getPrimaryField(), 1);
    EXPECT_EQ(data.reviewers[1].getSecondaryField(), 4);
}

TEST(CsvParserTest, throwsOnInvalidSubmissionId) {
    CSVParser parser("test/invalid_submission_id.csv");
    Data data;
    EXPECT_THROW(parser.parseDocument(data), std::domain_error);
}

TEST(CsvParserTest, throwsOnDuplicateSubmissionIds) {
    CSVParser parser("test/duplicate_submission_ids.csv");
    Data data;
    EXPECT_THROW(parser.parseDocument(data), std::invalid_argument);
}

TEST(CsvParserTest, throwsOnDuplicateReviewerIds) {
    CSVParser parser("test/duplicate_reviewer_ids.csv");
    Data data;
    EXPECT_THROW(parser.parseDocument(data), std::invalid_argument);
}

// ===== Parameter Parsing Tests via parseIndividualParameter =====

TEST(CsvParserTest, parseValidParameters) {
    CSVParser parser("test/testing.csv");
    Data data;
    parser.parseDocument(data);

    EXPECT_EQ(data.parameters.MinReviewsPerSubmission, 5);
    EXPECT_EQ(data.parameters.MaxReviewsPerReviewer, 4);
}

TEST(CsvParserTest, throwsOnInvalidMinReviewsPerSubmission) {
    // MinReviewsPerSubmission = 0 should throw (must be positive)
    CSVParser parser("test/invalid_min_reviews.csv");
    Data data;
    EXPECT_THROW(parser.parseDocument(data), std::domain_error);
}

TEST(CsvParserTest, throwsOnInvalidMaxReviewsPerReviewer) {
    // MaxReviewsPerReviewer = -1 should throw (must be positive)
    CSVParser parser("test/invalid_max_reviews_param.csv");
    Data data;
    EXPECT_THROW(parser.parseDocument(data), std::domain_error);
}

// ===== Control Parameter Parsing Tests via parseIndividualControlParameter =====

TEST(CsvParserTest, parseValidControlParameters) {
    CSVParser parser("test/testing.csv");
    Data data;
    parser.parseDocument(data);

    EXPECT_EQ(data.control.GenerateAssignments, 1);
    EXPECT_EQ(data.control.RiskAnalysis, 0);
    EXPECT_EQ(data.control.OutputFileName, "assign.csv");
}

TEST(CsvParserTest, throwsOnInvalidGenerateAssignments) {
    // GenerateAssignments = 4 (invalid, must be 0-3)
    CSVParser parser("test/invalid_generate_assignments.csv");
    Data data;
    EXPECT_THROW(parser.parseDocument(data), std::domain_error);
}

TEST(CsvParserTest, throwsOnInvalidRiskAnalysis) {
    // RiskAnalysis = 3 (invalid, must be 0-2)
    CSVParser parser("test/invalid_risk_analysis.csv");
    Data data;
    EXPECT_THROW(parser.parseDocument(data), std::domain_error);
}

// ===== Boundary Value Tests =====

TEST(CsvParserTest, generateAssignmentsBoundaryZeroIsValid) {
    // GenerateAssignments = 0 is valid (lower boundary)
    CSVParser parser("test/testing.csv");
    Data data;
    parser.parseDocument(data);
    EXPECT_GE(data.control.GenerateAssignments, 0);
    EXPECT_LE(data.control.GenerateAssignments, 3);
}

TEST(CsvParserTest, generateAssignmentsBoundaryThreeIsValid) {
    // GenerateAssignments = 3 is valid (upper boundary)
    CSVParser parser("test/testing.csv");
    Data data;
    parser.parseDocument(data);
    EXPECT_LE(data.control.GenerateAssignments, 3);
}

TEST(CsvParserTest, riskAnalysisBoundaryZeroIsValid) {
    // RiskAnalysis = 0 is valid (lower boundary)
    CSVParser parser("test/testing.csv");
    Data data;
    parser.parseDocument(data);
    EXPECT_GE(data.control.RiskAnalysis, 0);
    EXPECT_LE(data.control.RiskAnalysis, 2);
}

TEST(CsvParserTest, riskAnalysisBoundaryTwoIsValid) {
    // RiskAnalysis = 2 is valid (upper boundary)
    CSVParser parser("test/testing.csv");
    Data data;
    parser.parseDocument(data);
    EXPECT_LE(data.control.RiskAnalysis, 2);
}

TEST(CsvParserTest, minReviewsPerSubmissionBoundaryOneIsValid) {
    // MinReviewsPerSubmission = 1 is valid (positive boundary)
    CSVParser parser("test/testing.csv");
    Data data;
    parser.parseDocument(data);
    EXPECT_GT(data.parameters.MinReviewsPerSubmission, 0);
}

TEST(CsvParserTest, maxReviewsPerReviewerBoundaryOneIsValid) {
    // MaxReviewsPerReviewer = 1 is valid (positive boundary)
    CSVParser parser("test/testing.csv");
    Data data;
    parser.parseDocument(data);
    EXPECT_GT(data.parameters.MaxReviewsPerReviewer, 0);
}

