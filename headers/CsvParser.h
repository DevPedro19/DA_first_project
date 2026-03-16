//
// Created by pedro on 3/11/2026.
//

#ifndef ORGANIZATIONAL_TOOL_CSVPARSER_H
#define ORGANIZATIONAL_TOOL_CSVPARSER_H

#include "Reviewer.h"
#include "Submission.h"

#include <fstream>
#include <set>
#include <string>
#include <vector>

#include "Data.h"

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
    /**
     * @brief Constructor for the CSVParser class.
     * @param filename Filename of the .csv file.
     */
    CSVParser(std::string filename);

    /**
     * @brief This function essentially is the main method for parsing the .csv file.
     */
    void parseDocument(Data& data);
private:
    // Private fields
    std::string filename;
    std::set<int> submissionIds;
    std::set<int> reviewerIds;

    /**
     * @brief Helper function to remove leading and trailing spaces from a string.
     * @param str String from which to remove leading and trailing spaces.
     */
    static void removeTrailingCharacter(std::string& str, char character);

    /**
     * @brief This function serves as a generic parser for both submissions and reviewers.
     * It reads the file line by line, skipping the header and any lines starting with '#', and calls the provided parsing function for each line.
     * @tparam T The type of the items being parsed.
     * @tparam ParseFunction Serves as a generic type for the parsing function.
     * @param file The input file stream from which to read the data.
     * @param items The vector where the parsed items will be stored.
     * @param parseLine The function to be called for each specific type of object.
     */
    template<typename T, typename ParseFunction>
    void genericObjectParser(std::ifstream&file, std::vector<T>& items, ParseFunction parseLine);


    template <typename ParseFunction>
    static void genericParameterParser(std::ifstream& file, Data& data, ParseFunction parseLine);

    /**
     * @brief This function is responsible for parsing a single line of the CSV file
     * and populating a Submission object
     * @param line The line from the CSV file that contains the data for a single submission.
     * @param s The submission object itself that will be populated with the data from the line.
     */
    void parseIndividualSubmission(const std::string& line, Submission& s);


    /**
     * @brief This function is responsible for parsing a single line of the CSV file
     * and populating a Reviewer object
     * @param line The line from the CSV file that contains the data for a single reviewer.
     * @param r The reviewer object itself that will be populated with the data from the line.
     */
    void parseIndividualReviewer(const std::string& line, Reviewer& r);


    /**
     * @brief This function checks if the given ID has already been processed
     * @param ids A set of IDs that have already been encountered.
     * @param newId New ID to be checked against the set of existing IDs.
     * @return boolean value indicating whether the ID is repeated (true) or not (false).
     */
    static bool isRepeatedId(std::set<int>& ids, const int& newId);

    /**
     * @brief Convertes a string to an integer
     * @param str Parse string to be converted to an integer.
     * @return integer value obtained from the string.
     */
    static int getInteger(std::string& str);

    /**
     * @brief Checks if the given integer field is valid (positive integer)
     * throws a domain_error if the field is not valid.
     * @param fieldValue Value of the field to be validated.
     * @param fieldName Name of the field where the violation occurs
     */
    static void isValidIntField(int fieldValue, const std::string& fieldName);

    /**
     * @brief Checks if the given ID is unique among the existing IDs
     * throws an invalid_argument if the ID is not unique.
     * @param id ID to be checked for uniqueness.
     * @param fieldName Name of the field where the violation occurs.
     * @param existingIds Set of existing IDs to check against for uniqueness.
     */
    static void isUniqueId(int id, const std::string& fieldName, std::set<int>& existingIds);

    static void parseIndividualParameter(const std::string& line, Data& data);

    static void parseIndividualControlParameter(const std::string& line, Data& data);

    static void validateGenerateAssignments(int generateAssignments);

    static void validateRiskAnalysis(int riskAnalysis);
};

#endif //ORGANIZATIONAL_TOOL_CSVPARSER_H