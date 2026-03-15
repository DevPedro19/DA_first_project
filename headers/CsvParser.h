//
// Created by pedro on 3/11/2026.
//

#ifndef ORGANIZATIONAL_TOOL_CSVPARSER_H
#define ORGANIZATIONAL_TOOL_CSVPARSER_H
#include <fstream>
#include <set>
#include <string>
#include <vector>

#include "Reviewer.h"
#include "Submission.h"


/**
 * @brief This class is responsible for parsing the .csv file and extracting the data from it.
 * @details The CSV must follow the specified format:
 *
 * #Submissions
 *
 * id, title, author, email, primary field, secondary field (optional)
 *
 * #Reviewers
 *
 * id, name, email, field of expertise
 */
class CSVParser {
public:
    CSVParser(const std::string &filename);
    void parseDocument();
    std::vector<Reviewer> getReviewers() const { return reviewers; }
    std::vector<Submission> getSubmissions() const { return submissions; }
private:
    static void removeTrailingSpaces(std::string& str);
    std::string filename;
    std::vector<Reviewer> reviewers;
    std::vector<Submission> submissions;
    static void parseSubmissions(std::ifstream& file, std::vector<Submission>& submissions);
    static void parseIndividualSubmission(const std::string& line, Submission& s);
    static void parseReviewers(std::ifstream& file, std::vector<Reviewer>& reviewers);
    static void parseIndividualReviewer(const std::string& line, Reviewer& r);
};

#endif //ORGANIZATIONAL_TOOL_CSVPARSER_H