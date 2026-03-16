//
// Created by guilhermecunha on 15/03/26.
//

#include "InfoMenu.h"

#include <iostream>
#include <limits>

InfoMenu::InfoMenu(Data &data) : data_(data) {}

void InfoMenu::displayControl() const {
    std::cout << "\n=== Control Settings ===" << std::endl;

    const Control& c = data_.control;
    std::cout << "  GenerateAssignments: " << c.GenerateAssignments << std::endl;
    std::cout << "  RiskAnalysis:        " << c.RiskAnalysis << std::endl;
    std::cout << "  OutputFileName:      " << c.OutputFileName << std::endl;
}


void InfoMenu::displayParameters() const {
    std::cout << "\n=== Parameter Settings ===" << std::endl;

    const Parameters& p = data_.parameters;
    std::cout << "  MinReviewsPerSubmission:    " << p.MinReviewsPerSubmission << std::endl;
    std::cout << "  MaxReviewsPerReviewer:      " << p.MaxReviewsPerReviewer << std::endl;
    std::cout << "  PrimaryReviewerExpertise:   " << p.PrimaryReviewerExpertise << std::endl;
    std::cout << "  SecondaryReviewerExpertise: " << p.SecondaryReviewerExpertise << std::endl;
    std::cout << "  PrimarySubmissionDomain:    " << p.PrimarySubmissionDomain << std::endl;
    std::cout << "  SecondarySubmissionDomain:  " << p.SecondarySubmissionDomain << std::endl;
}


void InfoMenu::displayReviewers() const {
    std::cout << "\n=== Reviewers List ===" << std::endl;
    std::cout << "Id | Name | E-mail | Primary | Secondary" << std::endl;

    for (const Reviewer& reviewer : data_.reviewers) {
        std::cout << reviewer.getId() << " | " << reviewer.getName() << " | " << reviewer.getEmail() << " | " <<
            reviewer.getPrimaryField() << " | " << reviewer.getSecondaryField() << std::endl;
    }
}


void InfoMenu::displaySubmissions() const {
    std::cout << "\n=== Submissions List ===" << std::endl;
    std::cout << "Id | Title | Authors | E-mail | Primary | Secondary" << std::endl;

    for (const Submission& submission : data_.submissions) {
        std::cout << submission.getId() << " | " << submission.getTitle() << " | " << submission.getAuthor() << " | " <<
            submission.getEmail() << " | " << submission.getPrimaryField() << " | " << submission.getSecondaryField() << std::endl;
    }
}

void InfoMenu::displayOptions() {
    std::cout << "\n========================================\n"
              << "      CONFERENCE ORGANIZATION TOOL      \n"
              << "========================================\n"
              << "  [1] View Submissions\n"
              << "  [2] View Reviewers\n"
              << "  [3] Parameter Settings\n"
              << "  [4] Control Settings\n"
              << "  [5] Run Max Flow Algorithm\n"
              << "  [6] Exit\n"
              << "========================================\n"
              << "  Select an option (1-6): ";
}

int InfoMenu::display() const {
    int retV = 0;
    unsigned opt = 0;

    while (opt != 5 && opt != 6) {
        displayOptions();

        while(!(std::cin >> opt)){
             std::cin.clear();
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
             std::cout << "  Invalid input. Please enter a number.\n"
                       << "  Select an option (1-6): ";
        }

        while (opt < 1 || opt > 6) {
            std::cout << "  Invalid option.\n" << "  Select an option (1-6): ";
            while(!(std::cin >> opt)){
                 std::cin.clear();
                 std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                 std::cout << "  Invalid input. Please enter a number.\n"
                           << "  Select an option (1-6): ";
            }
        }

        switch (opt) {
            case 1: displaySubmissions(); break;
            case 2: displayReviewers(); break;
            case 3: displayParameters(); break;
            case 4: displayControl(); break;
            case 5: retV = 1; break;
            default: break;
        }
    }
    return retV; // 1 -> run max flow algorithm; 0 -> exit
}
