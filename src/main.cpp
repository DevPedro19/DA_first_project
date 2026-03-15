//
// Created by guilhermecunha on 15/03/26.
//

#include "../headers/CsvParser.h"

#include "../headers/CLI.h"

int main(int argc, char* argv[]) {
    CLI cli;
    std::vector<std::string> args(argv, argv + argc);
    cli.execute(args);
}