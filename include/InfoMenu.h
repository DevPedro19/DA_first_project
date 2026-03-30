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
    explicit InfoMenu(const Data& data);

    /**
     * @brief Displays the information menu and handles user interaction for displaying different sections of the data.
     * The user can choose to view submissions, reviewers, parameters, control settings, or run the max flow solver.
     * The method continues to prompt the user until they choose to exit or run the solver.
     * @return Either 0, if the user chooses to exit, or 1, if the user chooses to run the max flow solver.
     */
    [[nodiscard]] int display() const;

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

    /**
     * @brief Reference to the Data object containing all the information about submissions, reviewers, parameters and control settings. This reference is used to access the data and display it in the menu options.
     */
    const Data& data_;
};

#endif //ORGANIZATIONALTOOL_INFOMENU_H