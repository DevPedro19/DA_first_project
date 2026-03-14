//
// Created by pedro on 3/11/2026.
//

#ifndef ORGANIZATIONAL_TOOL_SUBMISSION_H
#define ORGANIZATIONAL_TOOL_SUBMISSION_H

#include <string>
#include <sstream>

class Submission {
public:
    Submission();
    Submission(int id, const std::string& title, const std::string& author, const std::string& email, int primaryField);
    int getId() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getEmail() const;
    int getPrimaryField() const;
    void setId(int newId);
    void setTitle(const std::string& newTitle);
    void setAuthor(const std::string& newAuthor);
    void setEmail(const std::string& newEmail);
    void setPrimaryField(int newPrimaryField);
    int getSecondaryField() const;
    void setSecondaryField(int newSecondaryField);
    std::string toString() const;
private:
    int id;
    std::string title;
    std::string author;
    std::string email;
    int primaryField;
    int secondaryField;
};

#endif //ORGANIZATIONAL_TOOL_SUBMISSION_H