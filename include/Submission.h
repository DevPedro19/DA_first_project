//
// Created by pedro on 3/11/2026.
//

#ifndef ORGANIZATIONAL_TOOL_SUBMISSION_H
#define ORGANIZATIONAL_TOOL_SUBMISSION_H

#include <string>

/**
 * @brief Represents a submission that can be assigned to reviewers.
 */
class Submission {
public:
    /**
     * @brief Default constructs an empty submission.
     */
    Submission();

    /**
     * @brief Constructs a submission with the provided metadata.
     * @param id Unique identifier for the submission.
     * @param title Title of the submission.
     * @param author Author name for the submission.
     * @param email Contact email for the author.
     * @param primaryField Primary field of study the submission belongs to.
     */
    Submission(int id, const std::string& title, const std::string& author, const std::string& email, int primaryField);

    /**
     * @brief Returns the submission identifier.
     * @return Submission id.
     */
    [[nodiscard]] int getId() const;

    /**
     * @brief Returns the submission title.
     * @return Title string.
     */
    [[nodiscard]] std::string getTitle() const;

    /**
     * @brief Returns the author name.
     * @return Author name string.
     */
    [[nodiscard]] std::string getAuthor() const;

    /**
     * @brief Returns the author contact email.
     * @return Email string.
     */
    [[nodiscard]] std::string getEmail() const;

    /**
     * @brief Returns the primary field identifier for the submission.
     * @return Primary field as an integer.
     */
    [[nodiscard]] int getPrimaryField() const;

    /**
     * @brief Updates the submission identifier.
     * @param newId New id value.
     */
    void setId(int newId);

    /**
     * @brief Updates the submission title.
     * @param newTitle New title string.
     */
    void setTitle(const std::string& newTitle);

    /**
     * @brief Updates the author name.
     * @param newAuthor New author name.
     */
    void setAuthor(const std::string& newAuthor);

    /**
     * @brief Updates the author contact email.
     * @param newEmail New email string.
     */
    void setEmail(const std::string& newEmail);

    /**
     * @brief Updates the primary field.
     * @param newPrimaryField New primary field id.
     */
    void setPrimaryField(int newPrimaryField);

    /**
     * @brief Returns the secondary field identifier for the submission.
     * @return Secondary field as an integer.
     */
    [[nodiscard]] int getSecondaryField() const;

    /**
     * @brief Updates the secondary field identifier.
     * @param newSecondaryField New secondary field id.
     */
    void setSecondaryField(int newSecondaryField);

    /**
     * @brief Formats the submission data as a string.
     * @return Human-readable submission summary.
     */
    [[nodiscard]] std::string toString() const;
private:
    int id;
    std::string title;
    std::string author;
    std::string email;
    int primaryField;
    int secondaryField;
};

#endif //ORGANIZATIONAL_TOOL_SUBMISSION_H