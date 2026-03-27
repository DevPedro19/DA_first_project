//
// Created by guilhermecunha on 16/03/26.
//

#ifndef ORGANIZATIONALTOOL_OUTPUTWRITER_H
#define ORGANIZATIONALTOOL_OUTPUTWRITER_H

#include "Result.h"
#include <string>

class OutputWriter {
public:

    OutputWriter(const std::string& outputFileName);
    void writeOutput(const Result& result, int riskAnalysis) const;

private:
    static void writeMatches(std::ofstream& ofs, std::vector<Match> matches);
    static void writeMisses(std::ofstream& ofs, std::vector<Miss> misses);
    static void writeRiskAnalysis(std::ofstream& ofs, std::vector<int> riskyReviewers, int riskAnalysis);

    std::string outputFileName_;
};
#endif //ORGANIZATIONALTOOL_OUTPUTWRITER_H