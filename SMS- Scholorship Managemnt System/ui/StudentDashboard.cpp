#include "StudentDashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QTabWidget>

StudentDashboard::StudentDashboard(const Student& currentStudent, QWidget* parent)
    : QMainWindow(parent), currentStudent(currentStudent) {
    
    fileManager = new FileManager("./data");
    setupUI();
    applyStyles();
    connectSignals();
    loadScholarships();
    loadApplicationStatus();
}

StudentDashboard::~StudentDashboard() {
    delete fileManager;
}

void StudentDashboard::setupUI() {
    setWindowTitle("Scholarship Management System - Student Dashboard");
    setGeometry(50, 50, 1200, 800);

    // Central Widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Welcome Section
    welcomeLabel = new QLabel(QString("Welcome, %1! (ID: %2)")
        .arg(currentStudent.getName())
        .arg(currentStudent.getId()), this);
    QFont titleFont("Arial", 16, QFont::Bold);
    welcomeLabel->setFont(titleFont);
    mainLayout->addWidget(welcomeLabel);

    // Eligibility Status
    eligibilityLabel = new QLabel("", this);
    QFont statusFont("Arial", 11);
    eligibilityLabel->setFont(statusFont);
    mainLayout->addWidget(eligibilityLabel);

    // Create Tab Widget
    QTabWidget* tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    // ===== SCHOLARSHIPS TAB =====
    QWidget* scholarshipsTab = new QWidget();
    QVBoxLayout* scholarshipsLayout = new QVBoxLayout(scholarshipsTab);

    QLabel* scholarshipsLabel = new QLabel("Available Scholarships", this);
    scholarshipsLabel->setFont(titleFont);
    scholarshipsLayout->addWidget(scholarshipsLabel);

    // Scholarships Table
    scholarshipsTable = new QTableWidget(this);
    scholarshipsTable->setColumnCount(5);
    scholarshipsTable->setHorizontalHeaderLabels({"ID", "Name", "Min Marks", "Max Income", "Amount"});
    scholarshipsTable->horizontalHeader()->setStretchLastSection(true);
    scholarshipsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    scholarshipsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    scholarshipsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    scholarshipsLayout->addWidget(scholarshipsTable);

    // Apply Button
    applyButton = new QPushButton("Apply for Selected Scholarship", this);
    applyButton->setMinimumHeight(40);
    scholarshipsLayout->addWidget(applyButton);

    tabWidget->addTab(scholarshipsTab, "Scholarships");

    // ===== APPLICATIONS TAB =====
    QWidget* applicationsTab = new QWidget();
    QVBoxLayout* applicationsLayout = new QVBoxLayout(applicationsTab);

    QLabel* applicationsLabel = new QLabel("Your Applications", this);
    applicationsLabel->setFont(titleFont);
    applicationsLayout->addWidget(applicationsLabel);

    // Applications Table
    applicationsTable = new QTableWidget(this);
    applicationsTable->setColumnCount(4);
    applicationsTable->setHorizontalHeaderLabels({"Application ID", "Scholarship", "Status", "Applied Date"});
    applicationsTable->horizontalHeader()->setStretchLastSection(true);
    applicationsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    applicationsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    applicationsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    applicationsLayout->addWidget(applicationsTable);

    tabWidget->addTab(applicationsTab, "My Applications");

    // ===== BUTTONS SECTION =====
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    refreshButton = new QPushButton("Refresh", this);
    refreshButton->setMinimumHeight(40);
    buttonLayout->addWidget(refreshButton);

    viewStatusButton = new QPushButton("View Student Details", this);
    viewStatusButton->setMinimumHeight(40);
    buttonLayout->addWidget(viewStatusButton);

    logoutButton = new QPushButton("Logout", this);
    logoutButton->setMinimumHeight(40);
    buttonLayout->addWidget(logoutButton);

    mainLayout->addLayout(buttonLayout);
}

void StudentDashboard::applyStyles() {
    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #1e1e2e;
        }
        
        QLabel {
            color: #ffffff;
        }
        
        QTableWidget {
            background-color: #2d2d44;
            alternate-background-color: #252535;
            gridline-color: #3d3d5c;
            color: #ffffff;
        }
        
        QTableWidget::item:selected {
            background-color: #00a86b;
        }
        
        QHeaderView::section {
            background-color: #00a86b;
            color: white;
            padding: 5px;
            border: none;
            font-weight: bold;
        }
        
        QPushButton {
            background-color: #00a86b;
            color: white;
            border: none;
            border-radius: 5px;
            font-weight: bold;
            padding: 8px;
        }
        
        QPushButton:hover {
            background-color: #00c77d;
        }
        
        QPushButton:pressed {
            background-color: #008552;
        }
        
        QTabWidget::pane {
            border: none;
        }
        
        QTabBar::tab {
            background-color: #2d2d44;
            color: #ffffff;
            padding: 8px 20px;
            border: none;
        }
        
        QTabBar::tab:selected {
            background-color: #00a86b;
        }
    )");
}

void StudentDashboard::loadScholarships() {
    scholarships = fileManager->loadScholarships();
    scholarshipsTable->setRowCount(0);

    for (const Scholarship& scholarship : scholarships) {
        int row = scholarshipsTable->rowCount();
        scholarshipsTable->insertRow(row);

        scholarshipsTable->setItem(row, 0, new QTableWidgetItem(QString::number(scholarship.getId())));
        scholarshipsTable->setItem(row, 1, new QTableWidgetItem(scholarship.getName()));
        scholarshipsTable->setItem(row, 2, new QTableWidgetItem(QString::number(scholarship.getMinMarks(), 'f', 2)));
        scholarshipsTable->setItem(row, 3, new QTableWidgetItem(QString::number(scholarship.getMaxIncome(), 'f', 2)));
        scholarshipsTable->setItem(row, 4, new QTableWidgetItem(QString::number(scholarship.getAmount(), 'f', 2)));
    }
}

void StudentDashboard::loadApplicationStatus() {
    applications = fileManager->loadApplications();
    applicationsTable->setRowCount(0);

    QVector<Scholarship> allScholarships = fileManager->loadScholarships();

    for (const Application& app : applications) {
        if (app.getStudentId() == currentStudent.getId()) {
            int row = applicationsTable->rowCount();
            applicationsTable->insertRow(row);

            // Find scholarship name
            QString scholarshipName = "Unknown";
            for (const Scholarship& sch : allScholarships) {
                if (sch.getId() == app.getScholarshipId()) {
                    scholarshipName = sch.getName();
                    break;
                }
            }

            applicationsTable->setItem(row, 0, new QTableWidgetItem(QString::number(app.getId())));
            applicationsTable->setItem(row, 1, new QTableWidgetItem(scholarshipName));
            
            QTableWidgetItem* statusItem = new QTableWidgetItem(app.getStatusString());
            
            // Color-code status
            if (app.getStatusString() == "Approved") {
                statusItem->setBackground(QColor("#00a86b"));
            } else if (app.getStatusString() == "Rejected") {
                statusItem->setBackground(QColor("#e74c3c"));
            } else {
                statusItem->setBackground(QColor("#f39c12"));
            }
            
            applicationsTable->setItem(row, 2, statusItem);
            applicationsTable->setItem(row, 3, new QTableWidgetItem(app.getAppliedDate()));
        }
    }
}

void StudentDashboard::connectSignals() {
    connect(applyButton, &QPushButton::clicked, this, &StudentDashboard::onApplyButtonClicked);
    connect(scholarshipsTable, &QTableWidget::cellClicked, this, &StudentDashboard::onScholarshipSelectionChanged);
    connect(refreshButton, &QPushButton::clicked, this, &StudentDashboard::onRefreshData);
    connect(viewStatusButton, &QPushButton::clicked, this, &StudentDashboard::onViewStatus);
    connect(logoutButton, &QPushButton::clicked, this, &StudentDashboard::onLogout);
}

void StudentDashboard::onScholarshipSelectionChanged(int row) {
    if (row >= 0 && scholarshipsTable->columnCount() > 0) {
        selectedScholarshipId = scholarshipsTable->item(row, 0)->text().toInt();
        updateEligibilityStatus();
    }
}

void StudentDashboard::updateEligibilityStatus() {
    if (selectedScholarshipId == -1) {
        eligibilityLabel->setText("");
        return;
    }

    for (const Scholarship& sch : scholarships) {
        if (sch.getId() == selectedScholarshipId) {
            QString reason = EligibilityService::getEligibilityReason(currentStudent, sch);
            if (reason == "Eligible") {
                eligibilityLabel->setText("✓ Eligible for: " + sch.getName());
                eligibilityLabel->setStyleSheet("color: #00a86b;");
            } else {
                eligibilityLabel->setText("✗ " + reason);
                eligibilityLabel->setStyleSheet("color: #e74c3c;");
            }
            break;
        }
    }
}

void StudentDashboard::onApplyButtonClicked() {
    if (selectedScholarshipId == -1) {
        QMessageBox::warning(this, "No Selection", "Please select a scholarship first!");
        return;
    }

    // Find scholarship
    Scholarship selectedScholarship;
    for (const Scholarship& sch : scholarships) {
        if (sch.getId() == selectedScholarshipId) {
            selectedScholarship = sch;
            break;
        }
    }

    // Check eligibility
    if (!EligibilityService::checkEligibility(currentStudent, selectedScholarship)) {
        QMessageBox::warning(this, "Not Eligible", 
            "You are not eligible for this scholarship.\n" + 
            EligibilityService::getEligibilityReason(currentStudent, selectedScholarship));
        return;
    }

    // Check for duplicate application
    for (const Application& app : applications) {
        if (app.getStudentId() == currentStudent.getId() &&
            app.getScholarshipId() == selectedScholarshipId) {
            QMessageBox::warning(this, "Already Applied", 
                "You have already applied for this scholarship!");
            return;
        }
    }

    // Create application
    int newId = applications.isEmpty() ? 1 : applications.last().getId() + 1;
    Application newApp(newId, currentStudent.getId(), selectedScholarshipId, ApplicationStatus::Pending);
    
    if (fileManager->addApplication(newApp)) {
        QMessageBox::information(this, "Success", "Application submitted successfully!");
        loadApplicationStatus();
        selectedScholarshipId = -1;
    } else {
        QMessageBox::critical(this, "Error", "Failed to submit application!");
    }
}

void StudentDashboard::onRefreshData() {
    loadScholarships();
    loadApplicationStatus();
    QMessageBox::information(this, "Refreshed", "Data refreshed successfully!");
}

void StudentDashboard::onViewStatus() {
    QString details = QString("Student ID: %1\nName: %2\nIncome: ₹%3\nMarks: %4")
        .arg(currentStudent.getId())
        .arg(currentStudent.getName())
        .arg(currentStudent.getIncome(), 0, 'f', 0)
        .arg(currentStudent.getMarks(), 0, 'f', 1);
    
    QMessageBox::information(this, "Your Details", details);
}

void StudentDashboard::onLogout() {
    emit logoutRequested();
}
