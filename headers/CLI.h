//
// Created by guilhermecunha on 15/03/26.
//

#ifndef FIRST_PROJECT_CLI_H
#define FIRST_PROJECT_CLI_H

#include <string>
#include <vector>



/**
 * @brief This class is the CLI of the program, that exposes all implemented functionalities in a user-friendly manner.
 */
class CLI {
public:

    static void printTitle();
    static void checkValidInputFile(const std::string& inputFile);
    static std::string askInputFilePath();

    void readInput(const std::string& string, Data& data);

    void execute(const std::vector<std::string>&);

    void setInputFileName(const std::string& inputFileName);
    std::string getInputFileName() const;
    void setOutputFileName(const std::string& outputFileName);
    std::string getOutputFileName() const;
    void setOutputRisk(const std::string& outputRisk);
    std::string getOutputRisk() const;

private:

    std::string inputFileName_;
    std::string outputFileName_;
    std::string outputRisk_;
};
#endif //FIRST_PROJECT_CLI_H