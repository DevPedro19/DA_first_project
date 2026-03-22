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

    /**
     * @brief Prints the title of the program.
     */
    static void printTitle();

    /**
     * @brief Checks if the given input file is valid (exists and has .csv extension)
     * @param inputFile Path of the input file to be checked.
     */
    static void checkValidInputFile(const std::string& inputFile);

    /**
     * @brief Asks the user for the relative path (from project's root directory) of the input file and returns it.
     * @return Relative path of the input file provided by the user.
     */
    static std::string askInputFilePath();

    /**
     * @brief Creates a CSVParser object and uses it to read the input file and populate the data object.
     * @param inputFileName The relative path of the input file to be read.
     * @param data The data object that will be populated with the information from the input file.
     */
    static void readInput(const std::string& inputFileName, Data& data);

    /**
     * @brief Creates an OutputWriter object and uses it to write the output file with the given result and risk analysis.
     * @param result Struct containing the information about the matches, misses and risky reviewers after running the max flow algorithm.
     * @param riskAnalysis Either 0 (no risk analysis), 1 (risk analysis for 1 missing reviewer) or k > 1 (risk analysis for k missing reviewers - not implemented).
     * @param outputFileName Name of the output file to be written.
     */
    void writeOutput(const Result& result, int riskAnalysis, std::string& outputFileName);

    /**
     * @brief Processes the command line arguments and determines if the execution is in batch or interactive modes and behaves accordingly to check the validity of the input file's extension and set the input and output file paths.
     * @param args Command line arguments passed to the program when executed.
     */
    void processArgs(const std::vector<std::string> & args);

    /**
     * @brief Main method of the CLI class that builds the high-level flow of the CLI execution, calling the necessary methods to read the input, run the max flow algorithm and write the output.
     * @param args Command line arguments passed to the program when executed.
     */
    void execute(const std::vector<std::string>& args);

    /**
     * @brief Sets the input file name.
     * @param inputFileName Name of the input file to be set.
     */
    void setInputFileName(const std::string& inputFileName);

    /**
     * @brief Sets the output file name.
     * @param outputFileName Name of the output file to be set.
     */
    void setOutputFileName(const std::string& outputFileName);
    void setIsValidOutputFileName(bool value);

private:
    /**
     * @brief Name of the input file to be read.
     */
    std::string inputFileName_;

    /**
     * @brief Name of the output file to be written.
     */
    std::string outputFileName_;
    
    /**
     * @brief Boolean value indicating whether the output file name comes from batch mode (true) or not (false). This is used to determine if the output file name provided in batch mode should be considered over the output file name specified in the input csv file.
     */
    bool isValidOutputFileName_ = false; // Initially false
};
#endif //FIRST_PROJECT_CLI_H