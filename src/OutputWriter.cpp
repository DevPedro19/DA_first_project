//
// Created by guilhermecunha on 16/03/26.
//

#include <utility>
#include <fstream>
#include <algorithm>

#include "OutputWriter.h"

OutputWriter::OutputWriter(std::string outputFileName, std::string outputRisk) :
outputFileName_(std::move(outputFileName)), outputRisk_(std::move(outputRisk)) {}

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
    ofs << "#SubmissionId,ReviewerId,Match" << '\n';
    for (const Match& match : matches) {
        ofs << match.reviewerId << ", " << match.submissionId << ", " << match.domain << '\n';
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


void OutputWriter::writeRiskAnalysis(std::ofstream& ofs, std::vector<unsigned int> riskyReviewers, unsigned riskAnalysis) const {
    std::sort(riskyReviewers.begin(), riskyReviewers.end());
    std::ofstream riskFile;

    if (!this->outputRisk_.empty()) // if a risk analysis output file was specified
        riskFile.open(this->outputRisk_);

    // allows for redirecting the output either to riskFile, if a risk analysis
    // output file was specified, or to the previous and still the same outputFile
    std::ofstream& riskOut = riskFile.is_open()? riskFile : ofs;

    riskOut << "#Risk Analysis: " << riskAnalysis << std::endl;

    if (!riskyReviewers.empty()) {
        riskOut << riskyReviewers[0];
        for (unsigned i = 1; i < riskyReviewers.size(); i++) {
            ofs << ", " << riskyReviewers[i];
        }
        riskOut << '\n';

    } else riskOut << "No risky reviewers\n";
}


void OutputWriter::writeOutput(const Result &result, unsigned riskAnalysis) const {
    std::ofstream ofs(this->outputFileName_);
    writeMatches(ofs, result.matches);
    writeMisses(ofs, result.misses);
    if (riskAnalysis) writeRiskAnalysis(ofs, result.riskyReviewers, riskAnalysis);
}
