//
// Created by guilhermecunha on 15/03/26.
//

#include "../headers/CLI.h"

#include <iostream>
#include <vector>

#include "../headers/CsvParser.h"

void CLI::execute(const std::vector<std::string>& args) {
    printTitle();

    if (args.size() >= 2 && args[1] == "-b") { // batch mode
        setInputFileName(args[2]);
        if (args.size() == 4) {
            setOutputRisk(args[3]);
        } // otherwise, the risk output will be written in the same output file as the rest

    } else { // interactive mode
        setInputFileName(askInputFilePath());
    }

    Data data;// will have fields like submissions, reviewers, parameters and control
    try {
        readInput(this->inputFileName, data);

    } catch (const std::invalid_argument& e) {
        std::cerr << "Duplicate ID:" << e.what() << std::endl;

    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl; // wrong number of submission/reviewer arguments

    } catch (const std::domain_error& e) {
        std::cerr << "Expected positive value: " << e.what() << std::endl;
    }

    InfoMenu infoMenu(data);
    infoMenu.display();

    Graph flowNetwork;
    flowNetwork.build(data.submissions, data.reviewers, data.parameters);

    flowNetwork.runMaxFlowAlgorithm();

    Result result;
    flowNetwork.checkFlow(result);

    writeOutput();
}
