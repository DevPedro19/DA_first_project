//
// Created by guilhermecunha on 15/03/26.
//

#ifndef ORGANIZATIONAL_TOOL_DATA_H
#define ORGANIZATIONAL_TOOL_DATA_H

#include "Reviewer.h"
#include "Submission.h"
#include <vector>

struct Parameters {
    unsigned MinReviewsPerSubmission    = 0,
    MaxReviewsPerReviewer               = 0,
    PrimaryReviewerExpertise            = 0,
    SecondaryReviewerExpertise          = 0,
    PrimarySubmissionDomain             = 0,
    SecondarySubmissionDomain           = 0;
};

struct Control {
    unsigned GenerateAssignments    = 0,
    RiskAnalysis                    = 0;

    std::string OutputFileName      = "output.csv";
};

struct Data {
    std::vector<Submission> submissions;
    std::vector<Reviewer> reviewers;
    Parameters parameters;
    Control control;
};


#endif //ORGANIZATIONAL_TOOL_DATA_H