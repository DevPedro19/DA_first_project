//
// Created by guilhermecunha on 15/03/26.
//

#ifndef ORGANIZATIONAL_TOOL_DATA_H
#define ORGANIZATIONAL_TOOL_DATA_H

#include "Reviewer.h"
#include "Submission.h"
#include <vector>

/**
 * @brief Input parameters from the CSV configuration, related to the constraints of the problem instance.
 */
struct Parameters {
    int MinReviewsPerSubmission    = 0;
    int MaxReviewsPerReviewer      = 0;
    int PrimaryReviewerExpertise   = 0;
    int SecondaryReviewerExpertise = 0;
    int PrimarySubmissionDomain    = 0;
    int SecondarySubmissionDomain  = 0;
};

/**
 * @brief Control parameters from the CSV configuration, related to the execution and output of the algorithm.
 */
struct Control {
    int GenerateAssignments = 0;
    int RiskAnalysis        = 0;
    std::string OutputFileName = "output.csv";
};

/**
 * @brief Aggregates all data including submissions, reviewers, parameters, and control settings.
 */
struct Data {
    std::vector<Submission> submissions;
    std::vector<Reviewer> reviewers;
    Parameters parameters;
    Control control;
};


#endif //ORGANIZATIONAL_TOOL_DATA_H