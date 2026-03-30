//
// Created by guilhermecunha on 16/03/26.
//

#ifndef ORGANIZATIONALTOOL_OUTPUTWRITER_H
#define ORGANIZATIONALTOOL_OUTPUTWRITER_H

#include "Result.h"
#include <string>

/**
 * @brief Responsible for writing the output of the algorithm to a CSV file.
 */
class OutputWriter {
public:
    /**
     * @brief Constructs an OutputWriter with specified output filename.
     * Creates the "output/" directory if it does not exist.
     * @param outputFileName Name for the output CSV file.
     */
    explicit OutputWriter(const std::string& outputFileName);

    /**
     * @brief Writes the output of the algorithm to a CSV file, including matches, misses, and risk analysis if applicable.
     * @param result the result of the algorithm, after running the max flow solver and possibly risk analysis, containing all
     * the resulting data.
     * @param riskAnalysis the risk analysis value, if not 0 should be included in the file.
     */
    void writeOutput(const Result& result, int riskAnalysis) const;

private:
    /**
     * @brief Writes the matches data in a structured way to the specified .csv output file.
     * @param ofs output file stream to write the matches data as a string.
     * @param matches matches populated with the results of the algorithm.
     */
    static void writeMatches(std::ofstream& ofs, std::vector<Match> matches);

    /**
     * @brief Writes the misses data in a structured way to the specified .csv output file.
     * @param ofs output file stream to write the misses data as a string.
     * @param misses misses populated with the results of the algorithm.
     */
    static void writeMisses(std::ofstream& ofs, std::vector<Miss> misses);

    /**
     * @brief Writes the risk analysis data in a structured way to the specified .csv output file.
     * @param ofs output file stream to write the risk analysis data as a string.
     * @param riskyReviewers vector of reviewer IDs that are considered risky based on the risk analysis.
     * @param riskAnalysis the risk analysis parameter value, which is used for output purposes in this case.
     */
    static void writeRiskAnalysis(std::ofstream& ofs, std::vector<int> riskyReviewers, int riskAnalysis);

    /**
     * @brief The name of the output file to be written.
     */
    std::string outputFileName_;
};
#endif //ORGANIZATIONALTOOL_OUTPUTWRITER_H