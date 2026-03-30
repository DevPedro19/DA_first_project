//
// Created by pedro on 3/11/2026.
//

#ifndef ORGANIZATIONAL_TOOL_REVIEWER_H
#define ORGANIZATIONAL_TOOL_REVIEWER_H
#include <string>

/**
 * @brief Represents a reviewer who can assess submissions.
 */
class Reviewer {
public:
    /**
     * @brief Default constructs an empty reviewer.
     */
    Reviewer();

    /**
     * @brief Constructs a reviewer with the provided metadata.
     * @param id Unique identifier for the reviewer.
     * @param name Reviewer name.
     * @param email Contact email for the reviewer.
     * @param primaryField Primary field of expertise.
     */
    Reviewer(int id, std::string  name, std::string  email, int primaryField);

    /**
     * @brief Returns the reviewer identifier.
     * @return Reviewer id.
     */
    [[nodiscard]] int getId() const;

    /**
     * @brief Returns the reviewer name.
     * @return Name string.
     */
    [[nodiscard]] std::string getName() const;

    /**
     * @brief Returns the reviewer contact email.
     * @return Email string.
     */
    [[nodiscard]] std::string getEmail() const;

    /**
     * @brief Returns the primary field identifier for the reviewer.
     * @return Primary field as an integer.
     */
    [[nodiscard]] int getPrimaryField() const;

    /**
     * @brief Updates the reviewer identifier.
     * @param newId New id value.
     */
    void setId(int newId);

    /**
     * @brief Updates the reviewer name.
     * @param newName New name string.
     */
    void setName(const std::string& newName);

    /**
     * @brief Updates the reviewer contact email.
     * @param newEmail New email string.
     */
    void setEmail(const std::string& newEmail);

    /**
     * @brief Updates the primary field.
     * @param newPrimaryField New primary field id.
     */
    void setPrimaryField(int newPrimaryField);

    /**
     * @brief Returns the secondary field identifier for the reviewer.
     * @return Secondary field as an integer.
     */
    [[nodiscard]] int getSecondaryField() const;

    /**
     * @brief Updates the secondary field identifier.
     * @param newSecondaryField New secondary field id.
     */
    void setSecondaryField(int newSecondaryField);

    /**
     * @brief Formats the reviewer data as a string.
     * @return Human-readable reviewer summary.
     */
    [[nodiscard]] std::string toString() const;
private:
    int id;
    std::string name;
    std::string email;
    int primaryField;
    int secondaryField;
};

#endif //ORGANIZATIONAL_TOOL_REVIEWER_H

