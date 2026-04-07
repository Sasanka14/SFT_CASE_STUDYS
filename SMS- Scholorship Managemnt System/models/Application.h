#ifndef APPLICATION_H
#define APPLICATION_H

#include <QString>
#include <QDateTime>
#include <QStringList>

enum class ApplicationStatus {
    Pending,
    Approved,
    Rejected
};

class Application {
private:
    int id;
    int studentId;
    int scholarshipId;
    ApplicationStatus status;
    QString appliedDate;

public:
    // Constructors
    Application();
    Application(int id, int studentId, int scholarshipId, ApplicationStatus status = ApplicationStatus::Pending);

    // Getters
    int getId() const { return id; }
    int getStudentId() const { return studentId; }
    int getScholarshipId() const { return scholarshipId; }
    ApplicationStatus getStatus() const { return status; }
    QString getStatusString() const;
    QString getAppliedDate() const { return appliedDate; }

    // Setters
    void setId(int id) { this->id = id; }
    void setStudentId(int studentId) { this->studentId = studentId; }
    void setScholarshipId(int scholarshipId) { this->scholarshipId = scholarshipId; }
    void setStatus(ApplicationStatus status) { this->status = status; }
    void setAppliedDate(const QString& date) { this->appliedDate = date; }

    // Utility
    static ApplicationStatus statusFromString(const QString& str);

    // Serialization
    QString serialize() const;
    static Application deserialize(const QString& line);

    // Display
    QString toString() const;
    bool isValid() const;
};

#endif // APPLICATION_H
