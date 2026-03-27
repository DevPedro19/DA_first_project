//
// Created by guilhermecunha on 16/03/26.
//

#include <fstream>
#include <algorithm>

#include "OutputWriter.h"

OutputWriter::OutputWriter(const std::string& outputFileName) :
outputFileName_("output/" + outputFileName) {}

void OutputWriter::writeMatches(std::ofstream& ofs, std::vector<Match> matches) {

    // "#SubmissionId,ReviewerId,Domain" Order
    std::sort(matches.begin(), matches.end(), [](const Match& m1, const Match& m2) {
        return m1.submissionId < m2.submissionId;
    });
    ofs << "#SubmissionId,ReviewerId,Match" << '\n';
    for (const Match& match : matches) {
        ofs << match.submissionId << ", " << match.reviewerId << ", " << match.domain << '\n';
    }

    // "#ReviewerId,SubmissionId,Domain" Order
    std::sort(matches.begin(), matches.end(), [](const Match& m1, const Match& m2) {
        return m1.reviewerId < m2.reviewerId;
    });
    ofs << "#ReviewerId,SubmissionId,Match" << '\n';
    for (const Match& match : matches) {
        ofs << match.reviewerId << ", " << match.submissionId << ", " << match.domain << '\n';
    }
    // #Total section
    if (!matches.empty()) {
        ofs << "#Total: " << matches.size() << "\n";
    }
}


void OutputWriter::writeMisses(std::ofstream& ofs, std::vector<Miss> misses) {
    std::sort(misses.begin(), misses.end(), [](const Miss& m1, const Miss& m2) {
        return m1.submissionId < m2.submissionId;
    });
    ofs << "#SubmissionId,Domain,MissingReviews" << '\n';
    for (const Miss& miss : misses) {
        ofs << miss.submissionId << ", " << miss.domain << ", " << miss.missingReviews << '\n';
    }
}


void OutputWriter::writeRiskAnalysis(std::ofstream& ofs, std::vector<int> riskyReviewers, int riskAnalysis) {
    std::sort(riskyReviewers.begin(), riskyReviewers.end());
    ofs << "#Risk Analysis: " << riskAnalysis << std::endl;

    if (!riskyReviewers.empty()) {
        ofs << riskyReviewers[0];
        for (unsigned i = 1; i < riskyReviewers.size(); i++) {
            ofs << ", " << riskyReviewers[i];
        }
        ofs << '\n';
    }
}


void OutputWriter::writeOutput(const Result &result, int riskAnalysis) const {
    std::ofstream ofs(this->outputFileName_);
    writeMatches(ofs, result.matches);
    if (!result.misses.empty()) {
        writeMisses(ofs, result.misses);
    }
    // Risk analysis is written directly in ofs
    if (riskAnalysis) writeRiskAnalysis(ofs, result.riskyReviewers, riskAnalysis);
}
