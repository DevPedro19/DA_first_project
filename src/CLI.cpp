//
// Created by guilhermecunha on 15/03/26.
//

#include "Graph.h"
#include "CLI.h"
#include "CsvParser.h"
#include "Data.h"
#include "InfoMenu.h"
#include "Result.h"
#include "MaxFlowSolver.h"
#include "OutputWriter.h"

#include <iostream>
#include <vector>
#include <filesystem>



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
    std::cout << "\n========================================\n"
              << "      CONFERENCE ORGANIZATION TOOL      \n"
              << "========================================\n" << std::endl;
}


void CLI::processArgs(const std::vector<std::string> &args) {
    if (args.size() >= 2 && args[1] == "-b") { // batch mode
        checkValidInputFile(args[2]);
        setInputFileName(args[2]);
        if (args.size() == 4) setOutputRisk(args[3]);
        // otherwise, the risk output will be written in the same output file as the rest

    } else {
        std::string inputFileName = askInputFilePath();
        checkValidInputFile(inputFileName);
        setInputFileName(inputFileName); // interactive mode
    }
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
    char sep = std::filesystem::path::preferred_separator;
    std::cout << "Enter input path to input file (.csv):\n(" << std::filesystem::current_path().string() << sep << ")";
    std::string inputFilepath;
    std::cin >> inputFilepath;
    return inputFilepath;
}


void CLI::readInput(const std::string &inputFileName, Data &data) {
    CSVParser csvParser(inputFileName);
    csvParser.parseDocument(data);
}

void CLI::writeOutput(const Result& result, unsigned riskAnalysis) const {
    OutputWriter output_writer = OutputWriter(outputFileName_, outputRisk_);
    output_writer.writeOutput(result, riskAnalysis);
}


void CLI::execute(const std::vector<std::string>& args) {
    printTitle();
    processArgs(args); // also sets the input file path

    Data data;
    readInput(this->inputFileName_, data);

    InfoMenu infoMenu(data);
    if (infoMenu.display()) {
        Graph flowNetwork(data);

        MaxFlowSolver solver(&flowNetwork);
        solver.execute();
        /*
        if (data.control.GenerateAssignments) {
            writeOutput(result, data.control.RiskAnalysis);
        }*/
        for (auto v : flowNetwork.getVertexSet()) {
            for (auto e : v->getAdj()) {
                std::cout << v->getName() << " -> " << e->getDest()->getName() << " (" << e->getFlow() << "," << e->getCapacity() << ")" << std::endl;
            }
        }
    }
}