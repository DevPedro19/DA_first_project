//
// Created by pedro on 3/11/2026.
//

#ifndef ORGANIZATIONAL_TOOL_REVIEWER_H
#define ORGANIZATIONAL_TOOL_REVIEWER_H
#include <string>
#include <sstream>

class Reviewer {
public:
    Reviewer();
    Reviewer(int id, const std::string& name, const std::string& email, int primaryField);
    int getId() const;
    std::string getName() const;
    std::string getEmail() const;
    int getPrimaryField() const;
    void setId(int newId);
    void setName(const std::string& newName);
    void setEmail(const std::string& newEmail);
    void setPrimaryField(int newPrimaryField);
    int getSecondaryField() const;
    void setSecondaryField(int newSecondaryField);
    std::string toString() const;
private:
    int id;
    std::string name;
    std::string email;
    int primaryField;
    int secondaryField;
};

#endif //ORGANIZATIONAL_TOOL_REVIEWER_H

