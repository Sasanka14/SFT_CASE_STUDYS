#include "Application.h"
#include <QStringList>

Application::Application() 
    : id(0), studentId(0), scholarshipId(0), status(ApplicationStatus::Pending),
      appliedDate(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm")) {}

Application::Application(int id, int studentId, int scholarshipId, ApplicationStatus status)
    : id(id), studentId(studentId), scholarshipId(scholarshipId), status(status),
      appliedDate(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm")) {}

QString Application::getStatusString() const {
    switch (status) {
        case ApplicationStatus::Pending:
            return "Pending";
        case ApplicationStatus::Approved:
            return "Approved";
        case ApplicationStatus::Rejected:
            return "Rejected";
        default:
            return "Unknown";
    }
}

ApplicationStatus Application::statusFromString(const QString& str) {
    if (str == "Approved") return ApplicationStatus::Approved;
    if (str == "Rejected") return ApplicationStatus::Rejected;
    return ApplicationStatus::Pending;
}

QString Application::serialize() const {
    return QString("%1|%2|%3|%4|%5")
        .arg(id)
        .arg(studentId)
        .arg(scholarshipId)
        .arg(getStatusString())
        .arg(appliedDate);
}

Application Application::deserialize(const QString& line) {
    QStringList parts = line.split("|");
    if (parts.size() < 4) {
        return Application();
    }
    
    Application app(
        parts[0].toInt(),
        parts[1].toInt(),
        parts[2].toInt(),
        statusFromString(parts[3])
    );
    
    if (parts.size() > 4) {
        app.setAppliedDate(parts[4]);
    }
    
    return app;
}

QString Application::toString() const {
    return QString("ID: %1 | Student: %2 | Scholarship: %3 | Status: %4 | Applied: %5")
        .arg(id)
        .arg(studentId)
        .arg(scholarshipId)
        .arg(getStatusString())
        .arg(appliedDate);
}

bool Application::isValid() const {
    return id > 0 && studentId > 0 && scholarshipId > 0;
}
