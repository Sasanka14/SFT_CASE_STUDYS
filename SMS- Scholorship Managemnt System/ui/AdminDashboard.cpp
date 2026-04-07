#include "AdminDashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QTabWidget>
#include <QGroupBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>

AdminDashboard::AdminDashboard(QWidget* parent)
    : QMainWindow(parent) {
    
    fileManager = new FileManager("./data");
    setupUI();
    applyStyles();
    connectSignals();
    loadApplications();
    loadStudents();
    loadScholarships();
}

AdminDashboard::~AdminDashboard() {
    delete fileManager;
}

void AdminDashboard::setupUI() {
    setWindowTitle("Scholarship Management System - Admin Dashboard");
    setGeometry(50, 50, 1400, 900);

    // Central Widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Admin Title
    adminTitleLabel = new QLabel("Admin Control Panel", this);
    QFont titleFont("Arial", 18, QFont::Bold);
    adminTitleLabel->setFont(titleFont);
    mainLayout->addWidget(adminTitleLabel);

    // Create Tab Widget
    QTabWidget* tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    // ===== APPLICATIONS TAB =====
    QWidget* applicationsTab = new QWidget();
    QVBoxLayout* applicationsLayout = new QVBoxLayout(applicationsTab);

    QLabel* applicationsLabel = new QLabel("Scholarship Applications", this);
    applicationsLabel->setFont(titleFont);
    applicationsLayout->addWidget(applicationsLabel);

    // Applications Table
    applicationsTable = new QTableWidget(this);
    applicationsTable->setColumnCount(7);
    applicationsTable->setHorizontalHeaderLabels({"App ID", "Student ID", "Student Name", "Scholarship", "Status", "Income", "Marks"});
    applicationsTable->horizontalHeader()->setStretchLastSection(true);
    applicationsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    applicationsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    applicationsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    applicationsLayout->addWidget(applicationsTable);

    // Action Buttons
    QHBoxLayout* actionLayout = new QHBoxLayout();
    actionLayout->setSpacing(10);

    approveButton = new QPushButton("Approve", this);
    approveButton->setMinimumHeight(40);
    approveButton->setStyleSheet("QPushButton { background-color: #27ae60; }");
    actionLayout->addWidget(approveButton);

    rejectButton = new QPushButton("Reject", this);
    rejectButton->setMinimumHeight(40);
    rejectButton->setStyleSheet("QPushButton { background-color: #e74c3c; }");
    actionLayout->addWidget(rejectButton);

    deleteButton = new QPushButton("Delete", this);
    deleteButton->setMinimumHeight(40);
    deleteButton->setStyleSheet("QPushButton { background-color: #c0392b; }");
    actionLayout->addWidget(deleteButton);

    applicationsLayout->addLayout(actionLayout);
    tabWidget->addTab(applicationsTab, "Applications");

    // ===== STUDENTS TAB =====
    QWidget* studentsTab = new QWidget();
    QVBoxLayout* studentsLayout = new QVBoxLayout(studentsTab);

    QLabel* studentsLabel = new QLabel("Manage Students", this);
    studentsLabel->setFont(titleFont);
    studentsLayout->addWidget(studentsLabel);

    // Students Table
    studentsTable = new QTableWidget(this);
    studentsTable->setColumnCount(4);
    studentsTable->setHorizontalHeaderLabels({"ID", "Name", "Income", "Marks"});
    studentsTable->horizontalHeader()->setStretchLastSection(true);
    studentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    studentsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    studentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    studentsLayout->addWidget(studentsTable);

    addStudentButton = new QPushButton("Add New Student", this);
    addStudentButton->setMinimumHeight(40);
    studentsLayout->addWidget(addStudentButton);

    tabWidget->addTab(studentsTab, "Students");

    // ===== SCHOLARSHIPS TAB =====
    QWidget* scholarshipsTab = new QWidget();
    QVBoxLayout* scholarshipsLayout = new QVBoxLayout(scholarshipsTab);

    QLabel* scholarshipsLabel = new QLabel("Manage Scholarships", this);
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

    addScholarshipButton = new QPushButton("Add New Scholarship", this);
    addScholarshipButton->setMinimumHeight(40);
    scholarshipsLayout->addWidget(addScholarshipButton);

    tabWidget->addTab(scholarshipsTab, "Scholarships");

    // ===== BUTTONS SECTION =====
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    refreshButton = new QPushButton("Refresh All", this);
    refreshButton->setMinimumHeight(40);
    buttonLayout->addWidget(refreshButton);

    logoutButton = new QPushButton("Logout", this);
    logoutButton->setMinimumHeight(40);
    logoutButton->setStyleSheet("QPushButton { background-color: #95a5a6; }");
    buttonLayout->addWidget(logoutButton);

    mainLayout->addLayout(buttonLayout);
}

void AdminDashboard::applyStyles() {
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

void AdminDashboard::loadApplications() {
    applications = fileManager->loadApplications();
    applicationsTable->setRowCount(0);

    QVector<Student> allStudents = fileManager->loadStudents();
    QVector<Scholarship> allScholarships = fileManager->loadScholarships();

    for (const Application& app : applications) {
        int row = applicationsTable->rowCount();
        applicationsTable->insertRow(row);

        // Find student and scholarship names
        QString studentName = "Unknown";
        float studentIncome = 0, studentMarks = 0;
        for (const Student& s : allStudents) {
            if (s.getId() == app.getStudentId()) {
                studentName = s.getName();
                studentIncome = s.getIncome();
                studentMarks = s.getMarks();
                break;
            }
        }

        QString scholarshipName = "Unknown";
        for (const Scholarship& sch : allScholarships) {
            if (sch.getId() == app.getScholarshipId()) {
                scholarshipName = sch.getName();
                break;
            }
        }

        applicationsTable->setItem(row, 0, new QTableWidgetItem(QString::number(app.getId())));
        applicationsTable->setItem(row, 1, new QTableWidgetItem(QString::number(app.getStudentId())));
        applicationsTable->setItem(row, 2, new QTableWidgetItem(studentName));
        applicationsTable->setItem(row, 3, new QTableWidgetItem(scholarshipName));
        
        QTableWidgetItem* statusItem = new QTableWidgetItem(app.getStatusString());
        if (app.getStatusString() == "Approved") {
            statusItem->setBackground(QColor("#27ae60"));
        } else if (app.getStatusString() == "Rejected") {
            statusItem->setBackground(QColor("#e74c3c"));
        } else {
            statusItem->setBackground(QColor("#f39c12"));
        }
        applicationsTable->setItem(row, 4, statusItem);
        
        applicationsTable->setItem(row, 5, new QTableWidgetItem(QString::number(studentIncome, 'f', 2)));
        applicationsTable->setItem(row, 6, new QTableWidgetItem(QString::number(studentMarks, 'f', 2)));
    }
}

void AdminDashboard::loadStudents() {
    students = fileManager->loadStudents();
    studentsTable->setRowCount(0);

    for (const Student& student : students) {
        int row = studentsTable->rowCount();
        studentsTable->insertRow(row);

        studentsTable->setItem(row, 0, new QTableWidgetItem(QString::number(student.getId())));
        studentsTable->setItem(row, 1, new QTableWidgetItem(student.getName()));
        studentsTable->setItem(row, 2, new QTableWidgetItem(QString::number(student.getIncome(), 'f', 2)));
        studentsTable->setItem(row, 3, new QTableWidgetItem(QString::number(student.getMarks(), 'f', 2)));
    }
}

void AdminDashboard::loadScholarships() {
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

void AdminDashboard::connectSignals() {
    connect(addScholarshipButton, &QPushButton::clicked, this, &AdminDashboard::onAddScholarshipClicked);
    connect(addStudentButton, &QPushButton::clicked, this, &AdminDashboard::onAddStudentClicked);
    connect(approveButton, &QPushButton::clicked, this, &AdminDashboard::onApproveApplicationClicked);
    connect(rejectButton, &QPushButton::clicked, this, &AdminDashboard::onRejectApplicationClicked);
    connect(deleteButton, &QPushButton::clicked, this, &AdminDashboard::onDeleteApplicationClicked);
    connect(refreshButton, &QPushButton::clicked, this, &AdminDashboard::onRefreshData);
    connect(logoutButton, &QPushButton::clicked, this, &AdminDashboard::onLogout);
}

void AdminDashboard::onAddScholarshipClicked() {
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Add New Scholarship");
    dialog->setGeometry(400, 300, 400, 250);

    QFormLayout* layout = new QFormLayout(dialog);

    QSpinBox* idBox = new QSpinBox();
    idBox->setMinimum(1);
    idBox->setValue(scholarships.isEmpty() ? 1 : scholarships.last().getId() + 1);
    layout->addRow("Scholarship ID:", idBox);

    QLineEdit* nameEdit = new QLineEdit();
    layout->addRow("Name:", nameEdit);

    QDoubleSpinBox* minMarksBox = new QDoubleSpinBox();
    minMarksBox->setRange(0, 100);
    minMarksBox->setValue(60);
    layout->addRow("Minimum Marks:", minMarksBox);

    QDoubleSpinBox* maxIncomeBox = new QDoubleSpinBox();
    maxIncomeBox->setRange(0, 1000000);
    maxIncomeBox->setValue(500000);
    layout->addRow("Maximum Income:", maxIncomeBox);

    QDoubleSpinBox* amountBox = new QDoubleSpinBox();
    amountBox->setRange(0, 1000000);
    amountBox->setValue(50000);
    layout->addRow("Scholarship Amount:", amountBox);

    QPushButton* saveButton = new QPushButton("Add Scholarship");
    QPushButton* cancelButton = new QPushButton("Cancel");

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    layout->addRow(buttonLayout);

    connect(saveButton, &QPushButton::clicked, [this, dialog, idBox, nameEdit, minMarksBox, maxIncomeBox, amountBox]() {
        if (nameEdit->text().isEmpty()) {
            QMessageBox::warning(dialog, "Validation", "Please enter scholarship name!");
            return;
        }

        Scholarship newScholarship(
            idBox->value(),
            nameEdit->text(),
            minMarksBox->value(),
            maxIncomeBox->value(),
            amountBox->value()
        );

        if (fileManager->addScholarship(newScholarship)) {
            QMessageBox::information(this, "Success", "Scholarship added successfully!");
            loadScholarships();
            dialog->close();
        } else {
            QMessageBox::critical(this, "Error", "Failed to add scholarship!");
        }
    });

    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::close);
    dialog->exec();
}

void AdminDashboard::onAddStudentClicked() {
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle("Add New Student");
    dialog->setGeometry(400, 300, 400, 250);

    QFormLayout* layout = new QFormLayout(dialog);

    QSpinBox* idBox = new QSpinBox();
    idBox->setMinimum(1);
    idBox->setValue(students.isEmpty() ? 1 : students.last().getId() + 1);
    layout->addRow("Student ID:", idBox);

    QLineEdit* nameEdit = new QLineEdit();
    layout->addRow("Name:", nameEdit);

    QDoubleSpinBox* incomeBox = new QDoubleSpinBox();
    incomeBox->setRange(0, 10000000);
    incomeBox->setValue(400000);
    layout->addRow("Annual Income:", incomeBox);

    QDoubleSpinBox* marksBox = new QDoubleSpinBox();
    marksBox->setRange(0, 100);
    marksBox->setValue(75);
    layout->addRow("Marks (0-100):", marksBox);

    QPushButton* saveButton = new QPushButton("Add Student");
    QPushButton* cancelButton = new QPushButton("Cancel");

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    layout->addRow(buttonLayout);

    connect(saveButton, &QPushButton::clicked, [this, dialog, idBox, nameEdit, incomeBox, marksBox]() {
        if (nameEdit->text().isEmpty()) {
            QMessageBox::warning(dialog, "Validation", "Please enter student name!");
            return;
        }

        Student newStudent(
            idBox->value(),
            nameEdit->text(),
            "", // email - admin can leave empty for now
            incomeBox->value(),
            marksBox->value(),
            "password123"
        );

        if (fileManager->addStudent(newStudent)) {
            QMessageBox::information(this, "Success", "Student added successfully!");
            loadStudents();
            loadApplications(); // Refresh applications view
            dialog->close();
        } else {
            QMessageBox::critical(this, "Error", "Failed to add student!");
        }
    });

    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::close);
    dialog->exec();
}

void AdminDashboard::onViewStudentsClicked() {
    // This slot is for viewing students - table already shows them
    loadStudents();
    QMessageBox::information(this, "Students", "Students view refreshed!");
}

void AdminDashboard::onApproveApplicationClicked() {
    if (applicationsTable->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select an application to approve!");
        return;
    }

    int row = applicationsTable->currentRow();
    int appId = applicationsTable->item(row, 0)->text().toInt();

    for (Application& app : applications) {
        if (app.getId() == appId) {
            app.setStatus(ApplicationStatus::Approved);
            updateApplicationStatus(app);
            loadApplications();
            QMessageBox::information(this, "Success", "Application approved!");
            return;
        }
    }
}

void AdminDashboard::onRejectApplicationClicked() {
    if (applicationsTable->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select an application to reject!");
        return;
    }

    int row = applicationsTable->currentRow();
    int appId = applicationsTable->item(row, 0)->text().toInt();

    for (Application& app : applications) {
        if (app.getId() == appId) {
            app.setStatus(ApplicationStatus::Rejected);
            updateApplicationStatus(app);
            loadApplications();
            QMessageBox::information(this, "Success", "Application rejected!");
            return;
        }
    }
}

void AdminDashboard::onDeleteApplicationClicked() {
    if (applicationsTable->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "No Selection", "Please select an application to delete!");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Confirm Delete", 
        "Are you sure you want to delete this application?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        int row = applicationsTable->currentRow();
        int appId = applicationsTable->item(row, 0)->text().toInt();

        applications.erase(std::find_if(applications.begin(), applications.end(), 
            [appId](const Application& app) { return app.getId() == appId; }));

        fileManager->saveApplications(applications);
        loadApplications();
        QMessageBox::information(this, "Success", "Application deleted!");
    }
}

void AdminDashboard::updateApplicationStatus(const Application& updatedApp) {
    fileManager->saveApplications(applications);
}

void AdminDashboard::onRefreshData() {
    loadApplications();
    loadStudents();
    loadScholarships();
    QMessageBox::information(this, "Refreshed", "All data refreshed successfully!");
}

void AdminDashboard::onLogout() {
    emit logoutRequested();
}
