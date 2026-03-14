#include "../headers/CsvParser.h"

#include <algorithm>
#include <iostream>
#include <sstream>

CSVParser::CSVParser(const std::string &filename) : filename(filename) {}


void CSVParser::parseDocument() {
    std::ifstream file(filename, std::ios::binary);
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("#Submissions") != std::string::npos) {
            parseSubmissions(file, submissions);
        }
        if (line.find("#Reviewers") != std::string::npos) {
            parseReviewers(file, reviewers);
        }
    }
}

void CSVParser::parseSubmissions(std::ifstream& file, std::vector<Submission>& submissions) {
    std::string line;
    // Skip header line
    std::string dummyLine;
    std::getline(file, dummyLine);
    std::streampos currentPos;
    while (true) {
        currentPos = file.tellg();
        if (!std::getline(file, line)) {
            break; // End of file
        }
        if (line[0] == '#') {
            file.seekg(currentPos);
            break;
        }
        auto submission = Submission();
        parseIndividualSubmission(line, submission);
        submissions.push_back(submission);
    }
}

void CSVParser::parseIndividualSubmission(const std::string& line, Submission& s) {
    std::istringstream iss(line);
    std::string data;
    int counter = 0;
    int secondaryField = 0;
    while (std::getline(iss, data, ',')) {
        switch (counter) {
            case 0: removeTrailingSpaces(data); s.setId(std::stoi(data)); break;
            case 1: removeTrailingSpaces(data); s.setTitle(data); break;
            case 2: removeTrailingSpaces(data); s.setAuthor(data); break;
            case 3: removeTrailingSpaces(data); s.setEmail(data); break;
            case 4:
                s.setPrimaryField(std::stoi(data));
                std::getline(iss, data);
                // Remove carriage return char (if present - for example if the code is run on Windows)
                if (data.back() == '\r') data.pop_back();
                if (!data.empty()) {
                    removeTrailingSpaces(data);
                    secondaryField = std::stoi(data);
                    s.setSecondaryField(secondaryField);
                }
                break;
            default: ;
        }
        counter++;
    }
}

void CSVParser::parseReviewers(std::ifstream& file, std::vector<Reviewer>& reviewers) {
    std::string line;
    // Skip header line
    std::string dummyLine;
    std::getline(file, dummyLine);
    std::streampos currentPos;
    while (true) {
        currentPos = file.tellg();
        if (!std::getline(file, line)) {
            break; // End of file
        }
        if (line.empty()) continue;
        if (line[0] == '#') {
            file.seekg(currentPos);
            break;
        }
        auto reviewer = Reviewer();
        parseIndividualReviewer(line, reviewer);
        reviewers.push_back(reviewer);
    }
}

void CSVParser::parseIndividualReviewer(const std::string& line, Reviewer& r) {
    std::istringstream iss(line);
    std::string data;
    int counter = 0;
    int secondaryField = 0;
    while (std::getline(iss, data, ',')) {
        switch (counter) {
            case 0: removeTrailingSpaces(data); r.setId(std::stoi(data)); break;
            case 1: removeTrailingSpaces(data); r.setName(data); break;
            case 2: removeTrailingSpaces(data); r.setEmail(data); break;
            case 3:
                r.setPrimaryField(std::stoi(data));
                std::getline(iss, data);
                // Remove carriage return char (if present - for example if the code is run on Windows)
                if (data.back() == '\r') data.pop_back();
                if (!data.empty()) {
                    removeTrailingSpaces(data);
                    secondaryField = std::stoi(data);
                    r.setSecondaryField(secondaryField);
                }
                break;
            default: ;
        }
        counter++;
    }
}


void CSVParser::removeTrailingSpaces(std::string &str) {
    const size_t start = str.find_first_not_of(' ');
    const size_t end = str.find_last_not_of(' ');
    if (start == std::string::npos || end == std::string::npos) {
        str = "";
    } else {
        str = str.substr(start, end - start + 1);
    }
}

std::vector<Submission> CSVParser::getSubmissions() const {
    return submissions;
}

std::vector<Reviewer> CSVParser::getReviewers() const {
    return reviewers;
}