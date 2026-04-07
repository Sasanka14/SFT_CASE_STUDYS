#include "FileManager.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QStringList>

FileManager::FileManager(const QString& basePath)
    : dataPath(basePath) {
    ensureDataDirectoryExists();
}

// ===== AUTHENTICATION =====

Student FileManager::authenticateStudent(const QString& email, const QString& password) {
    QVector<Student> students = loadStudents();
    for (const Student& student : students) {
        if (student.getEmail() == email && student.getPassword() == password) {
            return student;
        }
    }
    return Student(); // Return empty/invalid student if auth fails
}

QString FileManager::getStudentsFilePath() const {
    return dataPath + "/students.txt";
}

QString FileManager::getScholarshipsFilePath() const {
    return dataPath + "/scholarships.txt";
}

QString FileManager::getApplicationsFilePath() const {
    return dataPath + "/applications.txt";
}

bool FileManager::ensureDataDirectoryExists() {
    QDir dir;
    if (!dir.exists(dataPath)) {
        if (!dir.mkpath(dataPath)) {
            qWarning() << "Failed to create data directory: " << dataPath;
            return false;
        }
    }
    return true;
}

// ===== STUDENT OPERATIONS =====

bool FileManager::saveStudents(const QVector<Student>& students) {
    QFile file(getStudentsFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open students file for writing";
        return false;
    }

    QTextStream out(&file);
    for (const Student& student : students) {
        out << student.serialize() << "\n";
    }

    file.close();
    return true;
}

QVector<Student> FileManager::loadStudents() {
    QVector<Student> students;
    QFile file(getStudentsFilePath());

    if (!file.exists()) {
        return students;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open students file for reading";
        return students;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            students.append(Student::deserialize(line));
        }
    }

    file.close();
    return students;
}

bool FileManager::addStudent(const Student& student) {
    if (!student.isValid()) {
        qWarning() << "Invalid student data";
        return false;
    }

    QVector<Student> students = loadStudents();
    students.append(student);
    return saveStudents(students);
}

// ===== SCHOLARSHIP OPERATIONS =====

bool FileManager::saveScholarships(const QVector<Scholarship>& scholarships) {
    QFile file(getScholarshipsFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open scholarships file for writing";
        return false;
    }

    QTextStream out(&file);
    for (const Scholarship& scholarship : scholarships) {
        out << scholarship.serialize() << "\n";
    }

    file.close();
    return true;
}

QVector<Scholarship> FileManager::loadScholarships() {
    QVector<Scholarship> scholarships;
    QFile file(getScholarshipsFilePath());

    if (!file.exists()) {
        return scholarships;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open scholarships file for reading";
        return scholarships;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            scholarships.append(Scholarship::deserialize(line));
        }
    }

    file.close();
    return scholarships;
}

bool FileManager::addScholarship(const Scholarship& scholarship) {
    if (!scholarship.isValid()) {
        qWarning() << "Invalid scholarship data";
        return false;
    }

    QVector<Scholarship> scholarships = loadScholarships();
    scholarships.append(scholarship);
    return saveScholarships(scholarships);
}

// ===== APPLICATION OPERATIONS =====

bool FileManager::saveApplications(const QVector<Application>& applications) {
    QFile file(getApplicationsFilePath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open applications file for writing";
        return false;
    }

    QTextStream out(&file);
    for (const Application& app : applications) {
        out << app.serialize() << "\n";
    }

    file.close();
    return true;
}

QVector<Application> FileManager::loadApplications() {
    QVector<Application> applications;
    QFile file(getApplicationsFilePath());

    if (!file.exists()) {
        return applications;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open applications file for reading";
        return applications;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            applications.append(Application::deserialize(line));
        }
    }

    file.close();
    return applications;
}

bool FileManager::addApplication(const Application& application) {
    if (!application.isValid()) {
        qWarning() << "Invalid application data";
        return false;
    }

    QVector<Application> applications = loadApplications();
    applications.append(application);
    return saveApplications(applications);
}

void FileManager::initializeSampleData() {
    // This can be used to create sample data if files don't exist
    if (loadStudents().isEmpty() || loadScholarships().isEmpty()) {
        qDebug() << "Data files appear empty. Consider adding sample data.";
    }
}
