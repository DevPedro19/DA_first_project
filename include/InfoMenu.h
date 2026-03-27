//
// Created by guilhermecunha on 15/03/26.
//

#ifndef ORGANIZATIONALTOOL_INFOMENU_H
#define ORGANIZATIONALTOOL_INFOMENU_H

#include "Data.h"

/**
 * @brief Provides a user interface for viewing organizational data and running the solver.
 */
class InfoMenu {
public:
    /**
     * @brief Constructs an InfoMenu with the given data reference.
     * @param data Reference to the Data object containing submissions, reviewers, and configuration.
     */
    InfoMenu(Data& data);

    /**
     * @brief Displays the information menu and handles user interaction for displaying different sections of the data.
     * The user can choose to view submissions, reviewers, parameters, control settings, or run the solver.
     * The method continues to prompt the user until they choose to exit.
     * @return An integer status code (0 for normal exit, non-zero for errors).
     */
    int display() const;

private:
    /**
     * @brief Displays the main menu options.
     */
    static void displayOptions();

    /**
     * @brief Prints a field value or indicates if it is not defined.
     * @param field Field value to display.
     */
    static void printNonDefinedField(const int& field);

    /**
     * @brief Displays all submissions in the data.
     */
    void displaySubmissions() const;

    /**
     * @brief Displays all reviewers in the data.
     */
    void displayReviewers() const;

    /**
     * @brief Displays solver parameter settings.
     */
    void displayParameters() const;

    /**
     * @brief Displays control parameter settings.
     */
    void displayControl() const;
    Data& data_;
};

#endif //ORGANIZATIONALTOOL_INFOMENU_H