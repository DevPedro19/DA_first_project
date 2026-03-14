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
    // TODO: These two functions should be private since they are only used internally by the parser
    void parseSubmissions(std::ifstream& file, std::vector<Submission>& submissions);
    void parseIndividualSubmission(const std::string& line, Submission& s);
    void parseReviewers(std::ifstream& file, std::vector<Reviewer>& reviewers);
    void parseIndividualReviewer(const std::string& line, Reviewer& r);
    std::vector<Submission> getSubmissions() const;
    std::vector<Reviewer> getReviewers() const;
private:
    static void removeTrailingSpaces(std::string& str);
    std::string filename;
    /*
     * TODO: Estes dois vetores seria boa ideia passar para uma nova classe
     * já que o parser não deve ser responsável por guardar os dados, apenas por os ler e processa
     */
    std::vector<Reviewer> reviewers; // Using vector to be able to sort and since reviewers in the csv have unique IDs
    std::vector<Submission> submissions;
};

#endif //ORGANIZATIONAL_TOOL_CSVPARSER_H