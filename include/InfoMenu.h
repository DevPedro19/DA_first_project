//
// Created by guilhermecunha on 15/03/26.
//

#ifndef ORGANIZATIONALTOOL_INFOMENU_H
#define ORGANIZATIONALTOOL_INFOMENU_H

#include "Data.h"

class InfoMenu {
public:

    InfoMenu(Data& data);
    int display() const;

private:
    static void displayOptions();
    static void printNonDefinedField(const int& field);
    void displaySubmissions() const;
    void displayReviewers() const;
    void displayParameters() const;
    void displayControl() const;
    Data& data_;
};

#endif //ORGANIZATIONALTOOL_INFOMENU_H