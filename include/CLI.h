//
// Created by guilhermecunha on 15/03/26.
//

#ifndef FIRST_PROJECT_CLI_H
#define FIRST_PROJECT_CLI_H

#include "Data.h"

#include <string>
#include <vector>

#include "Result.h"

/**
 * @brief This class is the CLI of the program, that exposes all implemented functionalities in a user-friendly manner.
 */
class CLI {
public:

    CLI();

    static void printTitle();
    static void checkValidInputFile(const std::string& inputFile);
    static std::string askInputFilePath();
    static void readInput(const std::string& inputFileName, Data& data);

    void execute(const std::vector<std::string>& args);
    void writeOutput(const Result& result, unsigned riskAnalysis) const;
    void processArgs(const std::vector<std::string> & args);

    void setInputFileName(const std::string& inputFileName);
    void setOutputFileName(const std::string& outputFileName);
    void setOutputRisk(const std::string& outputRisk);

private:
    std::string inputFileName_;
    std::string outputFileName_;
    std::string outputRisk_;
};
#endif //FIRST_PROJECT_CLI_H