//
// Created by guilhermecunha on 15/03/26.
//

#include "../headers/CLI.h"
#include "../headers/CsvParser.h"
#include "../headers/Data.h"
#include "../headers/InfoMenu.h"
#include "../data_structures/Graph.h"

#include <iostream>
#include <vector>


CLI::CLI() = default;


void CLI::setInputFileName(const std::string &inputFileName) {
    this->inputFileName_ = inputFileName;
}


void CLI::setOutputFileName(const std::string &outputFileName) {
    this->outputFileName_ = outputFileName;
}


void CLI::setOutputRisk(const std::string &outputRisk) {
    this->outputRisk_ = outputRisk;
}


void CLI::printTitle() {
    std::cout << "## Scientific Conference Organization Tool ##" << std::endl;
}


void CLI::checkValidInputFile(const std::string& inputFile) {
    if (inputFile.empty()) {
        throw std::invalid_argument("Input file name cannot be empty.");
    }

    size_t dotPos = inputFile.rfind('.');

    if (dotPos == std::string::npos || inputFile.substr(dotPos) != ".csv") {
        throw std::invalid_argument("Input file extension should be .csv.");
    }
}


std::string CLI::askInputFilePath() {
    std::cout << "Enter input path to input file (.csv): ";
    std::string inputFilepath;
    std::cin >> inputFilepath;
    return inputFilepath;
}


void CLI::readInput(const std::string &inputFileName, Data &data) {
    CSVParser csvParser(inputFileName);
    csvParser.parseDocument(data);
}


void CLI::execute(const std::vector<std::string>& args) {
    printTitle();

    if (args.size() >= 2 && args[1] == "-b") { // batch mode
        checkValidInputFile(args[2]);
        setInputFileName(args[2]);
        if (args.size() == 4) setOutputRisk(args[3]);
        // otherwise, the risk output will be written in the same output file as the rest

    } else setInputFileName(askInputFilePath()); // interactive mode

    Data data;
    readInput(this->inputFileName_, data);

    InfoMenu infoMenu(data);
    if (infoMenu.display()) {
        Graph flowNetwork;
        flowNetwork.build(data);

        flowNetwork.runMaxFlowAlgorithm();

        Result result; // matches and misses
        flowNetwork.checkFlow(result);

        if (data.control.GenerateAssignments) {
            writeOutput();
        }
    }
}
