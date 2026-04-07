#ifndef ADMINDASHBOARD_H
#define ADMINDASHBOARD_H

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QDialog>
#include <QVector>
#include "../models/Student.h"
#include "../models/Scholarship.h"
#include "../models/Application.h"
#include "../services/FileManager.h"

class AdminDashboard : public QMainWindow {
    Q_OBJECT

public:
    AdminDashboard(QWidget* parent = nullptr);
    ~AdminDashboard();

signals:
    void logoutRequested();

private slots:
    void onAddScholarshipClicked();
    void onApproveApplicationClicked();
    void onRejectApplicationClicked();
    void onDeleteApplicationClicked();
    void onRefreshData();
    void onAddStudentClicked();
    void onViewStudentsClicked();
    void onLogout();

private:
    void setupUI();
    void applyStyles();
    void loadApplications();
    void loadStudents();
    void loadScholarships();
    void connectSignals();
    void updateApplicationStatus(const Application& updatedApp);

    // UI Components
    QLabel* adminTitleLabel;
    QTableWidget* applicationsTable;
    QTableWidget* studentsTable;
    QTableWidget* scholarshipsTable;
    QPushButton* addScholarshipButton;
    QPushButton* addStudentButton;
    QPushButton* approveButton;
    QPushButton* rejectButton;
    QPushButton* deleteButton;
    QPushButton* refreshButton;
    QPushButton* logoutButton;

    // Data
    QVector<Student> students;
    QVector<Scholarship> scholarships;
    QVector<Application> applications;
    FileManager* fileManager;
};

#endif // ADMINDASHBOARD_H
