#include "Reviewer.h"
#include <sstream>

Reviewer::Reviewer() : Reviewer(0, "", "", 0) {}

Reviewer::Reviewer(int id, const std::string& name, const std::string& email, int primaryField)
    : id(id), name(name), email(email), primaryField(primaryField), secondaryField(0) {}

int Reviewer::getId() const {
    return id;
}

std::string Reviewer::getName() const {
    return name;
}

std::string Reviewer::getEmail() const {
    return email;
}

int Reviewer::getPrimaryField() const {
    return primaryField;
}

int Reviewer::getSecondaryField() const {
    return secondaryField;
}


void Reviewer::setId(int newId) {
    id = newId;
}

void Reviewer::setName(const std::string& newName) {
    name = newName;
}

void Reviewer::setEmail(const std::string& newEmail) {
    email = newEmail;
}

void Reviewer::setPrimaryField(int newPrimaryField) {
    primaryField = newPrimaryField;
}

void Reviewer::setSecondaryField(int newSecondaryField) {
    secondaryField = newSecondaryField;
}

std::string Reviewer::toString() const {
    std::ostringstream oss;
    oss << "Reviewer ID: " << id << ", Name: " << name << ", Email: " << email << ", Primary Field: " << primaryField;
    return oss.str();
}
