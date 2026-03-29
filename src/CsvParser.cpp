#include "CsvParser.h"
#include "Data.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <utility>
#include <fstream>

CSVParser::CSVParser(std::string filename) : filename(std::move(filename)) {}


void CSVParser::removeTrailingSequence(std::string &str, const std::string& s) {
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
        removeTrailingSequence(line, " ");
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
        std::cout << "Started parsing submissions." << std::endl;
        parseSubmissions(fileSections["#Submissions"], data.submissions);
        std::cout << "Finished parsing submissions." << std::endl;
    }

    if (fileSections.find("#Reviewers") != fileSections.end()) {
        std::cout << "Started parsing reviewers." << std::endl;
        parseReviewers(fileSections["#Reviewers"], data.reviewers);
        std::cout << "Finished parsing reviewers." << std::endl;
    }

    if (fileSections.find("#Parameters") != fileSections.end()) {
        std::cout << "Started parsing parameters" << std::endl;
        parseParameters(fileSections["#Parameters"], data);
        std::cout << "Finished parsing parameters." << std::endl;
    }

    if (fileSections.find("#Control") != fileSections.end()) {
        std::cout << "Started parsing control parameters" << std::endl;
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

void CSVParser::processTitle(std::istringstream& iss, std::string& data) {
    size_t openingQuotes = data.find('\"');
    if (data.find('\"', openingQuotes + 1) == std::string::npos) { // did not find closing quotes, so the title has commas
        const std::string prev = data;
        std::getline(iss, data, '\"'); // reads up until the end of the title/until finding closing quotes, however these are not included in the data variable
        data = prev + "," + data + '\"'; // build up the entire title with the first comma, read initially and the closing quotes

        std::string temp;
        std::getline(iss, temp, ','); // move the input string stream forward to the next comma after the closing quotes
    }
    removeTrailingSequence(data, " "); // applies to titles with or without commas
}

void CSVParser::parseIndividualSubmission(const std::string& line, Submission& s) {
    std::istringstream iss(line);
    std::string data;
    int counter = 0;
    int id = 0;
    int primaryField = 0;
    int secondaryField = 0;
    while (std::getline(iss, data, ',')) {
        switch (counter) {
            case 0:
                id = getInteger(data);
                isValidIntField(id, "Submission ID ", 1);
                isUniqueId(id, "Submission ID ", submissionIds);
                s.setId(id);
                break;
            case 1: processTitle(iss, data); s.setTitle(data); break;
            case 2: removeTrailingSequence(data, " "); s.setAuthor(data); break;
            case 3: removeTrailingSequence(data, " "); s.setEmail(data); break;
            case 4:
                primaryField = getInteger(data);
                isValidIntField(primaryField, "Primary field ", 0);
                s.setPrimaryField(primaryField);
                std::getline(iss, data);
                // Remove carriage return char (if present - for example if the code is run on Windows)
                removeCarriageReturn(data);
                removeTrailingSequence(data, " ");
                if (!data.empty()) {
                    secondaryField = getInteger(data);
                    isValidIntField(secondaryField, "Secondary field ", 0);
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
    int id = 0;
    int primaryField = 0;
    int secondaryField = 0;
    while (std::getline(iss, data, ',')) {
        switch (counter) {
            case 0:
                id = getInteger(data);
                isValidIntField(id, "Reviewer ID ", 0);
                isUniqueId(id, "Reviewer ID ", reviewerIds);
                r.setId(id);
                break;
            case 1: removeTrailingSequence(data, " "); r.setName(data); break;
            case 2: removeTrailingSequence(data, " "); r.setEmail(data); break;
            case 3:
                primaryField = getInteger(data);
                isValidIntField(primaryField, "Primary field ", 0);
                r.setPrimaryField(primaryField);
                std::getline(iss, data);
                // Remove carriage return char (if present - for example if the code is run on Windows)
                removeCarriageReturn(data);
                removeTrailingSequence(data, " ");
                if (!data.empty()) {
                    secondaryField = getInteger(data);
                    isValidIntField(secondaryField, "Secondary field ", 0);
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
    removeTrailingSequence(str, " ");
    const int id = std::stoi(str);
    return id;
}

void CSVParser::isValidIntField(const int fieldValue, const std::string& fieldName, const int min) {
    // Check if the field value is a positive integer
    if (fieldValue < min) {
        const std::string errorMessage = fieldName + " must be greater or equal than " + std::to_string(min);
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
    std::getline(ss, parameterName, ','); // put the ss stream after the only comma of the line, so the next getline will read the value of the parameter
    std::string dataStr;
    // parameter name
    if (parameterName.find("MinReviewsPerSubmission") != std::string::npos) {
        std::getline(ss, dataStr);
        removeCarriageReturn(dataStr);
        const int minReviewsPerSubmission = getInteger(dataStr);
        isValidIntField(minReviewsPerSubmission, "Minimum reviews per submission", 1);
        data.parameters.MinReviewsPerSubmission = minReviewsPerSubmission;
    }
    else if (parameterName.find("MaxReviewsPerReviewer") != std::string::npos) {
        std::getline(ss, dataStr);
        removeCarriageReturn(dataStr);
        const int maxReviewsPerReviewer = getInteger(dataStr);
        isValidIntField(maxReviewsPerReviewer, "Maximum reviews per reviewer", 1);
        data.parameters.MaxReviewsPerReviewer = maxReviewsPerReviewer;
    }
    else if (parameterName.find("PrimaryReviewerExpertise") != std::string::npos) {
        std::getline(ss, dataStr);
        removeCarriageReturn(dataStr);
        const int primaryReviewerExpertise = getInteger(dataStr);
        isValidIntField(primaryReviewerExpertise, "Primary Reviewer Expertise", 0);
        data.parameters.PrimaryReviewerExpertise = primaryReviewerExpertise;
    }
    else if (parameterName.find("SecondaryReviewerExpertise") != std::string::npos) {
        std::getline(ss, dataStr);
        removeCarriageReturn(dataStr);
        const int secondaryReviewerExpertise = getInteger(dataStr);
        isValidIntField(secondaryReviewerExpertise, "Secondary Reviewer Expertise", 0);
        data.parameters.SecondaryReviewerExpertise = secondaryReviewerExpertise;
    }
    else if (parameterName.find("PrimarySubmissionDomain") != std::string::npos) {
        std::getline(ss, dataStr);
        removeCarriageReturn(dataStr);
        const int primarySubmissionDomain = getInteger(dataStr);
        isValidIntField(primarySubmissionDomain, "Primary Submission Domain", 0);
        data.parameters.PrimarySubmissionDomain = primarySubmissionDomain;
    }
    else if (parameterName.find("SecondarySubmissionDomain") != std::string::npos) {
        std::getline(ss, dataStr);
        removeCarriageReturn(dataStr);
        const int secondarySubmissionDomain = getInteger(dataStr);
        isValidIntField(secondarySubmissionDomain, "Secondary Submission Domain", 0);
        data.parameters.SecondarySubmissionDomain = secondarySubmissionDomain;
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
            const int generateAssignments = getInteger(dataStr);
            validateGenerateAssignments(generateAssignments);
            data.control.GenerateAssignments = generateAssignments;
        }
        else if (parameterName.find("RiskAnalysis") != std::string::npos) {
            std::getline(ss, dataStr);
            removeCarriageReturn(dataStr);
            const int riskAnalysis = getInteger(dataStr);
            validateRiskAnalysis(riskAnalysis);
            data.control.RiskAnalysis = riskAnalysis;
        }
        else if (parameterName.find("OutputFileName") != std::string::npos) {
            std::getline(ss, dataStr);
            removeCarriageReturn(dataStr);
            removeTrailingSequence(dataStr, " ");
            removeTrailingSequence(dataStr, "\"");
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
