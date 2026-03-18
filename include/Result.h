//
// Created by guilhermecunha on 15/03/26.
//

#ifndef ORGANIZATIONALTOOL_RESULT_H
#define ORGANIZATIONALTOOL_RESULT_H

#include <vector>

struct Match {
    int submissionId, reviewerId, domain;
};

struct Miss {
    int submissionId, domain, missingReviews;
};

struct Result {
    std::vector<Match> matches;
    std::vector<Miss> misses;
    std::vector<int> riskyReviewers;
};

#endif //ORGANIZATIONALTOOL_RESULT_H