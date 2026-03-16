//
// Created by guilhermecunha on 15/03/26.
//

#include "InfoMenu.h"

#include <iostream>

InfoMenu::InfoMenu(Data &data) : data_(data) {}

void InfoMenu::displayControl() const {
    std::cout << "#Control" << std::endl;

    const Control& c = data_.control;
    std::cout << "GenerateAssignments, " << c.GenerateAssignments << std::endl;
    std::cout << "RiskAnalysis, " << c.RiskAnalysis << std::endl;
    std::cout << "OutputFileName, " << c.OutputFileName << std::endl;
}


void InfoMenu::displayParameters() const {
    std::cout << "#Parameters" << std::endl;

    const Parameters& p = data_.parameters;
    std::cout << "MinReviewsPerSubmission, " << p.MinReviewsPerSubmission << std::endl;
    std::cout << "MaxReviewsPerReviewer, " << p.MaxReviewsPerReviewer << std::endl;
    std::cout << "PrimaryReviewerExpertise, " << p.PrimaryReviewerExpertise << std::endl;
    std::cout << "SecondaryReviewerExpertise, " << p.SecondaryReviewerExpertise << std::endl;
    std::cout << "PrimarySubmissionDomain, " << p.PrimarySubmissionDomain << std::endl;
    std::cout << "SecondarySubmissionDomain, " << p.SecondarySubmissionDomain << std::endl;
}


void InfoMenu::displayReviewers() const {
    std::cout << "#Reviewers" << std::endl;
    std::cout << "#Id, Name, E-mail,Primary,Secondary" << std::endl;

    for (const Reviewer& reviewer : data_.reviewers) {
        std::cout << reviewer.getId() << ", " << reviewer.getName() << ", " << reviewer.getEmail() << ", " <<
            reviewer.getPrimaryField() << ", " << reviewer.getSecondaryField() << std::endl;
    }
}


void InfoMenu::displaySubmissions() const {
    std::cout << "#Submissions" << std::endl;
    std::cout << "#Id, Title, Authors,E-mail,Primary,Secondary" << std::endl;

    for (const Submission& submission : data_.submissions) {
        std::cout << submission.getId() << ", " << submission.getTitle() << ", " << submission.getAuthor() << ", " <<
            submission.getEmail() << ", " << submission.getPrimaryField() << ", " << submission.getSecondaryField() << std::endl;
    }
}

void InfoMenu::displayOptions() {
    std::cout <<   "\n## Info Menu ##\n" <<
                   "1. See submissions\n" <<
                   "2. See reviewers\n" <<
                   "3. See parameter settings\n" <<
                   "4. See control settings\n" <<
                   "5. Run Max FLow Algorithm\n" <<
                   "6. Exit" << std::endl;

    std::cout << "Insert option (1-6): ";
}

int InfoMenu::display() const {
    int retV = 0;
    unsigned opt = 0;

    while (opt != 5 && opt != 6) {
        displayOptions();
        std::cin >> opt;

        while (opt < 1 || opt > 6) {
            std::cout << "\nInvalid option.\n" << "Insert option (1-4): ";
            std::cin >> opt;
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
