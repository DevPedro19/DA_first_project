#include "CsvParser.h"
#include "Data.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>

CSVParser::CSVParser(std::string filename) : filename(std::move(filename)) {}


void CSVParser::removeTrailingCharacter(std::string &str, const std::string& s) {
    const size_t start = str.find_first_not_of(s);
    const size_t end = str.find_last_not_of(s);
    if (start == std::string::npos || end == std::string::npos) {
        str = "";
    } else {
        str = str.substr(start, end - start + 1);
    }
}

void CSVParser::loadAndSplitFile() {
    fileSections.clear(); // Clear previous reads
    std::ifstream file(filename); // Read the CSV
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + filename);
    }
    std::string line; // Current line
    std::string currentSection; // String that identifies the section
    while (std::getline(file, line)) {
        removeCarriageReturn(line); // Remove carriage return if on Windows
        if (line.empty() || line == "#") continue; // Ignore weird # line with no fields
        // Create new section inside the map (maps each section with a vector with associated lines) for each new section in the file
        if (line.find("#Submissions") != std::string::npos) {
            currentSection = "#Submissions";
            continue;
        }
        if (line.find("#Reviewers") != std::string::npos) {
            currentSection = "#Reviewers";
            continue;
        }
        if (line.find("#Parameters") != std::string::npos) {
            currentSection = "#Parameters";
            continue;
        }
        if (line.find("#Control") != std::string::npos) {
            currentSection = "#Control";
            continue;
        }
        // Adds the line to the respective section in case of an already existing section
        if (!currentSection.empty()) {
            fileSections[currentSection].push_back(line);
        }
    }
}

void CSVParser::parseDocument(Data &data) {
    loadAndSplitFile();
    // Parses different existing sections (now part of the map of sections)
    // Only allows parsing of existing sections inside the csv
    if (fileSections.find("#Submissions") != fileSections.end()) {
        parseSubmissions(fileSections["#Submissions"], data.submissions);
        std::cout << "Finished parsing submissions." << std::endl;
    }

    if (fileSections.find("#Reviewers") != fileSections.end()) {
        parseReviewers(fileSections["#Reviewers"], data.reviewers);
        std::cout << "Finished parsing reviewers." << std::endl;
    }

    if (fileSections.find("#Parameters") != fileSections.end()) {
        parseParameters(fileSections["#Parameters"], data);
        std::cout << "Finished parsing parameters." << std::endl;
    }

    if (fileSections.find("#Control") != fileSections.end()) {
        parseControl(fileSections["#Control"], data);
        std::cout << "Finished parsing control parameters." << std::endl;
    }
}

void CSVParser::parseSubmissions(const std::vector<std::string>& lines, std::vector<Submission>& submissions) {
    if (lines.empty()) return;
    // Skips header line (starts in 1) and iterates over the lines of the submission section
    for (size_t i = 1; i < lines.size(); ++i) {
        const std::string& line = lines[i];
        if (line.empty()) continue;
        Submission s;
        parseIndividualSubmission(line, s);
        submissions.push_back(s);
    }
}

void CSVParser::parseReviewers(const std::vector<std::string>& lines, std::vector<Reviewer>& reviewers) {
    if (lines.empty()) return;
    for (size_t i = 1; i < lines.size(); ++i) {
        const std::string& line = lines[i];
        if (line.empty()) continue;
        Reviewer r;
        parseIndividualReviewer(line, r);
        reviewers.push_back(r);
    }
}

void CSVParser::parseParameters(const std::vector<std::string>& lines, Data& data) {
    for (const std::string& line : lines) {
        if (line.empty()) continue;
        parseIndividualParameter(line, data);
    }
}

void CSVParser::parseControl(const std::vector<std::string>& lines, Data& data) {
    for (const std::string& line : lines) {
        if (line.empty()) continue;
        parseIndividualControlParameter(line, data);
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
            case 1: removeTrailingCharacter(data, " "); s.setTitle(data); break;
            case 2: removeTrailingCharacter(data, " "); s.setAuthor(data); break;
            case 3: removeTrailingCharacter(data, " "); s.setEmail(data); break;
            case 4:
                primaryField = getInteger(data);
                isValidIntField(primaryField, "Primary field ");
                s.setPrimaryField(primaryField);
                std::getline(iss, data);
                // Remove carriage return char (if present - for example if the code is run on Windows)
                removeCarriageReturn(data);
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
            case 1: removeTrailingCharacter(data, " "); r.setName(data); break;
            case 2: removeTrailingCharacter(data, " "); r.setEmail(data); break;
            case 3:
                primaryField = getInteger(data);
                isValidIntField(primaryField, "Primary field ");
                r.setPrimaryField(primaryField);
                std::getline(iss, data);
                // Remove carriage return char (if present - for example if the code is run on Windows)
                removeCarriageReturn(data);
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



int CSVParser::getInteger(std::string &str) {
    removeTrailingCharacter(str, " ");
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
            removeCarriageReturn(dataStr);
            const int minReviewsPerSubmission = getInteger(dataStr);
            isValidIntField(minReviewsPerSubmission, "Minimum reviews per submission ");
            data.parameters.MinReviewsPerSubmission = minReviewsPerSubmission;
        }
        else if (parameterName.find("MaxReviewsPerReviewer") != std::string::npos) {
            std::getline(ss, dataStr);
            removeCarriageReturn(dataStr);
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
            removeCarriageReturn(dataStr);
            int generateAssignments = getInteger(dataStr);
            validateGenerateAssignments(generateAssignments);
            data.control.GenerateAssignments = generateAssignments;
        }
        else if (parameterName.find("RiskAnalysis") != std::string::npos) {
            std::getline(ss, dataStr);
            removeCarriageReturn(dataStr);
            int riskAnalysis = getInteger(dataStr);
            validateRiskAnalysis(riskAnalysis);
            data.control.RiskAnalysis = riskAnalysis;
        }
        else if (parameterName.find("OutputFileName") != std::string::npos) {
            std::getline(ss, dataStr);
            removeCarriageReturn(dataStr);
            removeTrailingCharacter(dataStr, " ");
            removeTrailingCharacter(dataStr, "\"");
            if (!dataStr.empty()) {
                data.control.OutputFileName = dataStr;
            }
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

void CSVParser::removeCarriageReturn(std::string &str) {
    if (!str.empty() && str.back() == '\r') {
        str.pop_back();
    }
}
