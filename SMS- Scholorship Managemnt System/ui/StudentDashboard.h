#ifndef STUDENTDASHBOARD_H
#define STUDENTDASHBOARD_H

#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QVector>
#include "../models/Student.h"
#include "../models/Scholarship.h"
#include "../models/Application.h"
#include "../services/FileManager.h"
#include "../services/EligibilityService.h"

class StudentDashboard : public QMainWindow {
    Q_OBJECT

public:
    StudentDashboard(const Student& currentStudent, QWidget* parent = nullptr);
    ~StudentDashboard();

signals:
    void logoutRequested();

private slots:
    void onApplyButtonClicked();
    void onScholarshipSelectionChanged(int row);
    void onRefreshData();
    void onViewStatus();
    void onLogout();

private:
    void setupUI();
    void applyStyles();
    void loadScholarships();
    void loadApplicationStatus();
    void updateEligibilityStatus();
    void connectSignals();

    // UI Components
    QLabel* welcomeLabel;
    QLabel* eligibilityLabel;
    QTableWidget* scholarshipsTable;
    QTableWidget* applicationsTable;
    QPushButton* applyButton;
    QPushButton* refreshButton;
    QPushButton* viewStatusButton;
    QPushButton* logoutButton;

    // Data
    Student currentStudent;
    QVector<Scholarship> scholarships;
    QVector<Application> applications;
    FileManager* fileManager;
    int selectedScholarshipId = -1;
};

#endif // STUDENTDASHBOARD_H
