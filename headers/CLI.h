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

    void printTitle();

    void readInput(const std::string& string, Data& data);

    void execute(const std::vector<std::string>&);

    std::string askInputFilePath();

    void setInputFileName(std::string inputFileName);
    std::string getInputFileName();
    void setOutputFileName(std::string outputFileName);
    std::string getOutputFileName();
    void setOutputRisk(std::string outputRisk);
    std::string getOutputRisk();

private:

    std::string inputFileName;
    std::string outputFileName;
    std::string outputRisk;




};
#endif //FIRST_PROJECT_CLI_H