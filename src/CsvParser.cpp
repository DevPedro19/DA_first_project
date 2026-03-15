#include "../headers/CsvParser.h"

#include <algorithm>
#include <iostream>
#include <sstream>

CSVParser::CSVParser(const std::string &filename) : filename(filename) {}


void CSVParser::removeTrailingSpaces(std::string &str) {
    const size_t start = str.find_first_not_of(' ');
    const size_t end = str.find_last_not_of(' ');
    if (start == std::string::npos || end == std::string::npos) {
        str = "";
    } else {
        str = str.substr(start, end - start + 1);
    }
}

void CSVParser::parseDocument() {
    std::ifstream file(filename, std::ios::binary);
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("#Submissions") != std::string::npos) {
            genericParser(file, submissions, [this](const std::string& l, Submission& s) {
                parseIndividualSubmission(l, s);
            });
            printf("Finished parsing submissions.\n");
        }
        else if (line.find("#Reviewers") != std::string::npos) {
            genericParser(file, reviewers, [this](const std::string& l, Reviewer& r) {
                parseIndividualReviewer(l, r);
            });
            printf("Finished parsing reviewers.\n");
        }
    }
}

void CSVParser::parseIndividualSubmission(const std::string& line, Submission& s) {
    std::istringstream iss(line);
    std::string data;
    int counter = 0, id = 0, primaryField = 0, secondaryField = 0;
    while (std::getline(iss, data, ',')) {
        switch (counter) {
            case 0:
                id = getInteger(data);
                isValidIntField(id, "Submission ID ");
                isUniqueId(id, "Submission ID ", submissionIds);
                s.setId(id);
                break;
            case 1: removeTrailingSpaces(data); s.setTitle(data); break;
            case 2: removeTrailingSpaces(data); s.setAuthor(data); break;
            case 3: removeTrailingSpaces(data); s.setEmail(data); break;
            case 4:
                primaryField = getInteger(data);
                isValidIntField(primaryField, "Primary field ");
                s.setPrimaryField(primaryField);
                std::getline(iss, data);
                // Remove carriage return char (if present - for example if the code is run on Windows)
                if (data.back() == '\r') data.pop_back();
                if (!data.empty()) {
                    secondaryField = getInteger(data);
                    isValidIntField(secondaryField, "Secondary field ");
                    s.setSecondaryField(secondaryField);
                }
                break;
            default: ;
        }
        counter++;
    }
}

void CSVParser::parseIndividualReviewer(const std::string& line, Reviewer& r) {
    std::istringstream iss(line);
    std::string data;
    int counter = 0;
    int id = 0, primaryField = 0, secondaryField = 0;
    while (std::getline(iss, data, ',')) {
        switch (counter) {
            case 0:
                id = getInteger(data);
                isValidIntField(id, "Reviewer ID ");
                isUniqueId(id, "Reviewer ID ", reviewerIds);
                r.setId(id);
                break;
            case 1: removeTrailingSpaces(data); r.setName(data); break;
            case 2: removeTrailingSpaces(data); r.setEmail(data); break;
            case 3:
                primaryField = getInteger(data);
                isValidIntField(primaryField, "Primary field ");
                r.setPrimaryField(primaryField);
                std::getline(iss, data);
                // Remove carriage return char (if present - for example if the code is run on Windows)
                if (data.back() == '\r') data.pop_back();
                if (!data.empty()) {
                    secondaryField = getInteger(data);
                    isValidIntField(secondaryField, "Secondary field ");
                    r.setSecondaryField(secondaryField);
                }
                break;
            default: ;
        }
        counter++;
    }
}

bool CSVParser::isRepeatedIds(std::set<int> &ids,const int &newId) {
    if (ids.find(newId) != ids.end()) {
        return true; // ID is repeated
    }
    ids.insert(newId); // Add new ID to the set
    return false; // ID is not repeated
}


template<typename T, typename ParseFunction>
void CSVParser::genericParser(std::ifstream &file, std::vector<T>& items, ParseFunction parseLine) {
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
        T item;
        parseLine(line, item);
        items.push_back(item);
    }
}

int CSVParser::getInteger(std::string &str) {
    removeTrailingSpaces(str);
    int id = std::stoi(str);
    return id;
}

void CSVParser::isValidIntField(const int fieldValue, const std::string& fieldName) {
    // Check if the field value is a positive integer
    if (fieldValue <= 0) {
        const std::string errorMessage = fieldName + "must be a positive integer.";
        throw std::domain_error(errorMessage);
    }
}


void CSVParser::isUniqueId(int id, const std::string &fieldName, std::set<int> &existingIds) {
    // Check if the field value is unique
    if (existingIds.find(id) != existingIds.end()) {
        const std::string errorMessage = fieldName + " must be unique. Duplicate value: " + std::to_string(id);
        throw std::invalid_argument(errorMessage);
    }
    existingIds.insert(id);
}