#include "../headers/Submission.h"
#include <sstream>

Submission::Submission() : Submission(0, "", "", "", 0) {}

Submission::Submission(int id, const std::string& title, const std::string& author, const std::string& email, int primaryField)
    : id(id), title(title), author(author), email(email), primaryField(primaryField), secondaryField(0) {}

int Submission::getId() const {
    return id;
}

std::string Submission::getTitle() const {
    return title;
}

std::string Submission::getAuthor() const {
    return author;
}

std::string Submission::getEmail() const {
    return email;
}

int Submission::getPrimaryField() const {
    return primaryField;
}

void Submission::setId(int newId) {
    id = newId;
}

void Submission::setTitle(const std::string& newTitle) {
    title = newTitle;
}

void Submission::setAuthor(const std::string& newAuthor) {
    author = newAuthor;
}

void Submission::setEmail(const std::string& newEmail) {
    email = newEmail;
}

void Submission::setPrimaryField(int newPrimaryField) {
    primaryField = newPrimaryField;
}

int Submission::getSecondaryField() const {
    return secondaryField;
}

void Submission::setSecondaryField(int newSecondaryField) {
    secondaryField = newSecondaryField;
}

std::string Submission::toString() const {
    std::ostringstream oss;
    oss << "Submission ID: " << id << ", Title: " << title << ", Author: " << author << ", Email: " << email << ", Primary Field: " << primaryField;
    return oss.str();
}
