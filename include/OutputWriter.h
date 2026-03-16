//
// Created by guilhermecunha on 16/03/26.
//

#ifndef ORGANIZATIONALTOOL_OUTPUTWRITER_H
#define ORGANIZATIONALTOOL_OUTPUTWRITER_H

#include "Result.h"
#include <string>

class OutputWriter {
public:

    OutputWriter(std::string outputFileName, std::string outputRisk);
    void writeOutput(const Result& result, unsigned riskAnalysis) const;

private:
    static void writeMatches(std::ofstream& ofs, std::vector<Match> matches);
    static void writeMisses(std::ofstream& ofs, std::vector<Miss> misses);
    void writeRiskAnalysis(std::ofstream& ofs, std::vector<unsigned> riskyReviewers, unsigned riskAnalysis) const;

    std::string outputFileName_;
    std::string outputRisk_;
};
#endif //ORGANIZATIONALTOOL_OUTPUTWRITER_H