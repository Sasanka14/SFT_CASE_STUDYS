#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QVector>
#include "../models/Student.h"
#include "../models/Scholarship.h"
#include "../models/Application.h"

class FileManager {
private:
    QString dataPath;
    
    // File paths
    QString getStudentsFilePath() const;
    QString getScholarshipsFilePath() const;
    QString getApplicationsFilePath() const;

public:
    FileManager(const QString& basePath = "./data");

    // Authentication
    Student authenticateStudent(const QString& email, const QString& password);
    
    // Student operations
    bool saveStudents(const QVector<Student>& students);
    QVector<Student> loadStudents();
    bool addStudent(const Student& student);

    // Scholarship operations
    bool saveScholarships(const QVector<Scholarship>& scholarships);
    QVector<Scholarship> loadScholarships();
    bool addScholarship(const Scholarship& scholarship);

    // Application operations
    bool saveApplications(const QVector<Application>& applications);
    QVector<Application> loadApplications();
    bool addApplication(const Application& application);

    // Utility
    bool ensureDataDirectoryExists();
    void initializeSampleData();

    // Getters
    QString getDataPath() const { return dataPath; }
};

#endif // FILEMANAGER_H
