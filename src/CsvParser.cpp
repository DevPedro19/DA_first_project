#include "../headers/CsvParser.h"
#include "../headers/Data.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>

CSVParser::CSVParser(std::string filename) : filename(std::move(filename)) {}


void CSVParser::removeTrailingCharacter(std::string &str, char character) {
    const size_t start = str.find_first_not_of(character);
    const size_t end = str.find_last_not_of(character);
    if (start == std::string::npos || end == std::string::npos) {
        str = "";
    } else {
        str = str.substr(start, end - start + 1);
    }
}

void CSVParser::parseDocument(Data &data) {
    std::ifstream file(filename, std::ios::binary);
    std::string line;
    while (std::getline(file, line)) {
        // No genericObject parser precisamos de passar this nos capture arguments uma vez que
        // funções internas necessitam de aceder ao set de ID's para verificar se estes são únicos.
        if (line.find("#Submissions") != std::string::npos) {
            genericObjectParser(file, data.submissions, [this](const std::string& l, Submission& s) {
                parseIndividualSubmission(l, s);
            });
            printf("Finished parsing submissions.\n");
        }
        else if (line.find("#Reviewers") != std::string::npos) {
            genericObjectParser(file, data.reviewers, [this](const std::string& l, Reviewer& r) {
                parseIndividualReviewer(l, r);
            });
            printf("Finished parsing reviewers.\n");
        }
        // Nos parsers dos parâmetros já não é necessário fazê-lo uma vez que estes não necessitam de aceder a
        // nenhum field do CSV parser: as funções são static
        else if (line.find("#Parameters") != std::string::npos) {
            genericParameterParser(file, data, [](const std::string& l, Data& d) {
                parseIndividualParameter(l, d);
            });
            printf("Finished parsing parameters.\n");
        }
        else if (line.find("#Control") != std::string::npos) {
            genericParameterParser(file, data, [](const std::string& l, Data& d) {
                parseIndividualControlParameter(l, d);
            });
            printf("Finished parsing control parameters.\n");
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
            case 1: removeTrailingCharacter(data, ' '); s.setTitle(data); break;
            case 2: removeTrailingCharacter(data, ' '); s.setAuthor(data); break;
            case 3: removeTrailingCharacter(data, ' '); s.setEmail(data); break;
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
            case 1: removeTrailingCharacter(data, ' '); r.setName(data); break;
            case 2: removeTrailingCharacter(data, ' '); r.setEmail(data); break;
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

bool CSVParser::isRepeatedId(std::set<int> &ids,const int &newId) {
    if (ids.find(newId) != ids.end()) {
        return true; // ID is repeated
    }
    ids.insert(newId); // Add new ID to the set
    return false; // ID is not repeated
}


template<typename T, typename ParseFunction>
void CSVParser::genericObjectParser(std::ifstream &file, std::vector<T>& items, ParseFunction parseLine) {
    std::string line;
    // Skip header line
    std::string dummyLine;
    std::getline(file, dummyLine);
    while (true) {
        const std::streampos currentPos = file.tellg();
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

template<typename ParseFunction>
void CSVParser::genericParameterParser(std::ifstream &file, Data &data, ParseFunction parseLine) {
    std::string line;
    while (true) {
        const std::streampos currentPos = file.tellg();
        if (!std::getline(file, line)) {
            break; // End of file
        }
        if (line[0] == '#') {
            file.seekg(currentPos);
            break;
        }
        parseLine(line, data);
    }
}

int CSVParser::getInteger(std::string &str) {
    removeTrailingCharacter(str, ' ');
    const int id = std::stoi(str);
    return id;
}

void CSVParser::isValidIntField(const int fieldValue, const std::string& fieldName) {
    // Check if the field value is a positive integer
    if (fieldValue <= 0) {
        const std::string errorMessage = fieldName + "must be a positive integer.";
        throw std::domain_error(errorMessage);
    }
}


void CSVParser::isUniqueId(const int id, const std::string &fieldName, std::set<int> &existingIds) {
    // Check if the field value is unique
    if (existingIds.find(id) != existingIds.end()) {
        const std::string errorMessage = fieldName + " must be unique. Duplicate value: " + std::to_string(id);
        throw std::invalid_argument(errorMessage);
    }
    // Update the existing id's with a new unique id since a duplicate was not found
    existingIds.insert(id);
}

void CSVParser::parseIndividualParameter(const std::string& line, Data &data) {
    std::istringstream ss(line);
    std::string parameterName;
    while (std::getline(ss, parameterName, ',')) {
        std::string dataStr;
        // parameter name
        if (parameterName.find("MinReviewsPerSubmission") != std::string::npos) {
            std::getline(ss, dataStr);
            if (dataStr.back() == '\r') dataStr.pop_back();
            const int minReviewsPerSubmission = getInteger(dataStr);
            isValidIntField(minReviewsPerSubmission, "Minimum reviews per submission ");
            data.parameters.MinReviewsPerSubmission = minReviewsPerSubmission;
        }
        else if (parameterName.find("MaxReviewsPerReviewer") != std::string::npos) {
            std::getline(ss, dataStr);
            if (dataStr.back() == '\r') dataStr.pop_back();
            const int maxReviewsPerReviewer = getInteger(dataStr);
            isValidIntField(maxReviewsPerReviewer, "Maximum reviews per reviewer ");
            data.parameters.MaxReviewsPerReviewer = maxReviewsPerReviewer;
        }
    }
}

void CSVParser::parseIndividualControlParameter(const std::string& line, Data &data) {
    std::istringstream ss(line);
    std::string parameterName;
    while (std::getline(ss, parameterName, ',')) {
        std::string dataStr;
        if (parameterName.find("GenerateAssignments") != std::string::npos) {
            std::getline(ss, dataStr);
            if (dataStr.back() == '\r') dataStr.pop_back();
            int generateAssignments = getInteger(dataStr);
            validateGenerateAssignments(generateAssignments);
            data.control.GenerateAssignments = generateAssignments;
        }
        else if (parameterName.find("RiskAnalysis") != std::string::npos) {
            std::getline(ss, dataStr);
            if (dataStr.back() == '\r') dataStr.pop_back();
            int riskAnalysis = getInteger(dataStr);
            validateRiskAnalysis(riskAnalysis);
            data.control.RiskAnalysis = riskAnalysis;
        }
        else if (parameterName.find("OutputFileName") != std::string::npos) {
            std::getline(ss, dataStr);
            if (dataStr.back() == '\r') dataStr.pop_back();
            removeTrailingCharacter(dataStr, ' ');
            removeTrailingCharacter(dataStr, '"');
            data.control.OutputFileName = dataStr;
        }
    }
}

void CSVParser::validateGenerateAssignments(const int generateAssignments) {
    if (generateAssignments < 0 || generateAssignments > 3) {
        const std::string errorMessage = "GenerateAssignments must be a non-negative integer. Invalid value: " + std::to_string(generateAssignments);
        throw std::domain_error(errorMessage);
    }
}

void CSVParser::validateRiskAnalysis(const int riskAnalysis) {
    if (riskAnalysis < 0 || riskAnalysis > 2) {
        const std::string errorMessage = "RiskAnalysis must be a non-negative integer. Invalid value: " + std::to_string(riskAnalysis);
        throw std::domain_error(errorMessage);
    }
}


