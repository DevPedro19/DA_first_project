//
// Created by pedro on 3/23/2026.
//

#include <filesystem>

#include "CsvParser.h"
#include "Graph.h"
#include "MaxFlowSolver.h"
#include "OutputWriter.h"
#include "EdmondsKarp.h"
#include "gtest/gtest.h"
#include <fstream>
#include <string>


void verifyCsvLinesMatch(const std::string &actualPath, const std::string &expectedPath) {
    std::ifstream actual(actualPath);
    std::ifstream expected(expectedPath);

    std::string actualLine;
    std::string expectedLine;
    int lineNumber = 1;
    while (true) {
        std::getline(actual, actualLine);
        std::getline(expected, expectedLine);
        if (actualLine.empty() && expectedLine.empty()) {
            break;
        }
        ASSERT_EQ(actualLine, expectedLine)
            << "Mismatch at line " << lineNumber << " between " << actualPath << " and " << expectedPath;
        lineNumber++;
    }
} // namespace
TEST(test_datasets, test_all_dataset) {
    std::string outputDirectoryName = "output";
    // Create directory
    std::filesystem::create_directory(outputDirectoryName);
    for (int i = 1; i <= 14; i++) {
        std::string inputFilepath = "input/dataset" + std::to_string(i) + ".csv";
        std::string outputFilepath = "output/output_dataset" + std::to_string(i) + ".csv";
        std::string expectedOutputFilepath = "expected/output_dataset" + std::to_string(i) + ".csv";
        Data data;
        CSVParser parser(inputFilepath);
        parser.parseDocument(data);
        Graph<nodeInfo> graph(data);
        EdmondsKarp edmondsKarp(&graph);
        MaxFlowSolver solver(&graph, &edmondsKarp);
        solver.execute();
        Result result;
        solver.checkResults(result, data.control.RiskAnalysis, data.reviewers);
        OutputWriter output_writer(data.control.OutputFileName);
        output_writer.writeOutput(result, data.control.RiskAnalysis);
        verifyCsvLinesMatch(outputFilepath, expectedOutputFilepath);
    }
}