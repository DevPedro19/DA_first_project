//
// Created by guilhermecunha on 15/03/26.
//

#ifndef ORGANIZATIONALTOOL_RESULT_H
#define ORGANIZATIONALTOOL_RESULT_H

#include <vector>

/**
 * @brief Represents a match between a submission and a reviewer for a specific domain.
 */
struct Match {
    int submissionId;
    int reviewerId;
    int domain;
};

/**
 * @brief Represents a miss where a certain submission was not reviewed enough by the required number of reviewers
 * and counts how many reviews are still missing for that submission.
 */
struct Miss {
    int submissionId;
    int domain;
    int missingReviews;
};

/**
 * @brief Stores the outputs of the MaxFlowSolver: matches, misses and riskyReviewers
 * in case of a risk analysis
 */
struct Result {
    std::vector<Match> matches;
    std::vector<Miss> misses;
    std::vector<int> riskyReviewers;
};

#endif //ORGANIZATIONALTOOL_RESULT_H