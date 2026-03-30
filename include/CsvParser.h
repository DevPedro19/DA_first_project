//
// Created by pedro on 3/11/2026.
//

#ifndef ORGANIZATIONAL_TOOL_CSVPARSER_H
#define ORGANIZATIONAL_TOOL_CSVPARSER_H

#include "Reviewer.h"
#include "Submission.h"


#include <set>
#include <string>
#include <vector>
#include <map>

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
    /**
     * @brief The filename of the .csv file to be parsed.
     */
    std::string filename;

    /**
     * @brief Set of submission IDs that have already been processed, used to ensure uniqueness of submission IDs and to validate the data in the .csv file.
     */
    std::set<int> submissionIds;

    /**
     * @brief Set of reviewer IDs that have already been processed, used to ensure uniqueness of reviewer IDs and to validate the data in the .csv file.
     */
    std::set<int> reviewerIds;

    /**
     * @brief Map that associates each section header of the .csv file ("#Submissions", "#Reviewers", "#Parameters", "#Control") with a vector of strings containing the lines associated with that section.
     */
    std::map<std::string, std::vector<std::string>> fileSections;


    /**
     * @brief This function loads the .csv file and splits it into different sections (submissions, reviewers, parameters, control)
     * based on each section header, storing the lines associated with each section in a map of line vectors
     * with the section header as the key.
     */
    void loadAndSplitFile();

    /** @brief This function is responsible for parsing the lines associated with the submissions section of the .csv file
     * and populating a vector of Submission objects with the data from the lines.
     * @param lines Vector of strings containing the lines associated with the submissions section of the .csv file.
     * @param submissions Vector of Submission objects that will be populated with the data from the lines.
     */
    void parseSubmissions(const std::vector<std::string>& lines, std::vector<Submission>& submissions);

    /** @brief This function is responsible for parsing the lines associated with the reviewers section of the .csv file
     * and populating a vector of Reviewer objects with the data from the lines.
     * @param lines Vector of strings containing the lines associated with the reviewers section of the .csv file.
     * @param reviewers Vector of Reviewer objects that will be populated with the data from the lines.
     */
    void parseReviewers(const std::vector<std::string>& lines, std::vector<Reviewer>& reviewers);


    /** @brief This function is responsible for parsing the lines associated with the parameters section of the .csv file
     * and populating the parameters field of the Data object with the data from the lines.
     * @param lines Vector of strings containing the lines associated with the parameters section of the .csv file.
     * @param data Data struct that will have its parameters field populated with the data from these lines.
     */
    static void parseParameters(const std::vector<std::string>& lines, Data& data);


    /** @brief This function is responsible for parsing the lines associated with the control section of the .csv file
    * and populating the control field of the Data object with the data from the lines.
    * @param lines Vector of strings containing the lines associated with the control section of the .csv file.
    * @param data Data object that will have its control field populated with the data from these lines.
    */
    static void parseControl(const std::vector<std::string>& lines, Data& data);

    /**
     * @brief Helper function to remove leading and trailing spaces from a string.
     * @param str String from which to remove leading and trailing spaces.
     * @param s Sequence to remove at the beginning or at the end of the string str.
     */
    static void removeTrailingSequence(std::string& str, const std::string& s);

    /**
     * @brief Ensures that the title is correctly parsed, even if it contains commas, preventing it from interfering with the parsing of an individual submission.
     * @param iss The input string stream that is being used to parse the line of the CSV file.
     * @param data The string variable that will hold the entire title of the submission.
     */
    static void processTitle(std::istringstream& iss, std::string& data);

    /**
     * @brief This function is responsible for parsing a single submission line of the CSV file
     * and populating a Submission object
     * @param line The line from the CSV file that contains the data for a single submission.
     * @param s The submission object itself that will be populated with the data from the line.
     */
    void parseIndividualSubmission(const std::string& line, Submission& s);


    /**
     * @brief This function is responsible for parsing a single reviewer line of the CSV file
     * and populating a Reviewer object
     * @param line The line from the CSV file that contains the data for a single reviewer.
     * @param r The reviewer object itself that will be populated with the data from the line.
     */
    void parseIndividualReviewer(const std::string& line, Reviewer& r);


    /**
     * @brief This function is responsible for parsing a single parameter line of the CSV file
     * and populating the parameters field of the data object
     * @param line The line from the CSV file that contains the data for a single parameter.
     * @param data The data object that will have its parameters field populated with the parameter from the line.
     */
    static void parseIndividualParameter(const std::string& line, Data& data);


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
     * @param min The minimum valid value to fieldValue
     */
    static void isValidIntField(int fieldValue, const std::string& fieldName, int min);

    /**
     * @brief Checks if the given ID is unique among the existing IDs and, if so, adds it to the set of existing IDs. Otherwise, throws an invalid_argument if the ID is not unique.
     * @param id ID to be checked for uniqueness.
     * @param fieldName Name of the field where the violation occurs.
     * @param existingIds Set of existing IDs to check against for uniqueness.
     */
    static void isUniqueId(int id, const std::string& fieldName, std::set<int>& existingIds);

    static void parseIndividualControlParameter(const std::string& line, Data& data);

    /**
     * @brief Validates the value of the GenerateAssignments parameter, ensuring it is either 0, 1 or 2.
     * Throws a domain_error if the value is not valid.
     * @param generateAssignments Value of the GenerateAssignments control parameter to be validated.
     */
    static void validateGenerateAssignments(int generateAssignments);

    /**
     * @brief Validates the value of the RiskAnalysis parameter, ensuring it is either 0 or 1.
     * Throws a domain_error if the value is not valid.
     * @param riskAnalysis Value of the RiskAnalysis control parameter to be validated.
     */
    static void validateRiskAnalysis(int riskAnalysis);


    /** @brief Helper function to remove carriage return characters from a string,
     * which may be present in the data read from the CSV file and can interfere with parsing (for example if the code is
     * executed on Windows)
     * @param str String from which to remove carriage return characters.
     */
    static void removeCarriageReturn(std::string& str);

};

#endif //ORGANIZATIONAL_TOOL_CSVPARSER_H
