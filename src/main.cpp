//
// Created by guilhermecunha on 15/03/26.
//

#include <iostream>

#include "../headers/CsvParser.h"

#include "../headers/CLI.h"

int main(int argc, char* argv[]) {
    const std::vector<std::string> args(argv, argv + argc);

    try {
        CLI cli;
        cli.execute(args);
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}