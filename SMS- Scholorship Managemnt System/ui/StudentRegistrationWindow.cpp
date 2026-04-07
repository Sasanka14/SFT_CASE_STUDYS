#include "StudentRegistrationWindow.h"
#include "../models/Student.h"
#include "../services/FileManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QRegularExpression>

StudentRegistrationWindow::StudentRegistrationWindow(QWidget* parent)
    : QMainWindow(parent) {
    setWindowTitle("Student Registration");
    setGeometry(100, 100, 500, 450);
    setupUI();
}

void StudentRegistrationWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel* titleLabel = new QLabel("Student Registration", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    // Full Name
    mainLayout->addWidget(new QLabel("Full Name:", this));
    fullNameInput = new QLineEdit(this);
    fullNameInput->setPlaceholderText("Enter your full name");
    mainLayout->addWidget(fullNameInput);

    // Email (auto-generated from name, can be edited)
    mainLayout->addWidget(new QLabel("Email:", this));
    emailInput = new QLineEdit(this);
    emailInput->setPlaceholderText("e.g., john.doe@email.com");
    connect(fullNameInput, &QLineEdit::textChanged, this, &StudentRegistrationWindow::validateEmail);
    mainLayout->addWidget(emailInput);

    // Income
    mainLayout->addWidget(new QLabel("Annual Income:", this));
    incomeInput = new QDoubleSpinBox(this);
    incomeInput->setRange(0, 10000000);
    incomeInput->setValue(0);
    incomeInput->setSuffix(" PKR");
    mainLayout->addWidget(incomeInput);

    // Marks
    mainLayout->addWidget(new QLabel("Marks (0-100):", this));
    marksInput = new QSpinBox(this);
    marksInput->setRange(0, 100);
    marksInput->setValue(0);
    mainLayout->addWidget(marksInput);

    // Password
    mainLayout->addWidget(new QLabel("Password:", this));
    passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setPlaceholderText("Enter a secure password");
    mainLayout->addWidget(passwordInput);

    // Confirm Password
    mainLayout->addWidget(new QLabel("Confirm Password:", this));
    confirmPasswordInput = new QLineEdit(this);
    confirmPasswordInput->setEchoMode(QLineEdit::Password);
    confirmPasswordInput->setPlaceholderText("Confirm your password");
    mainLayout->addWidget(confirmPasswordInput);

    // Status Label
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: red;");
    mainLayout->addWidget(statusLabel);

    // Buttons
    mainLayout->addSpacing(10);
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    
    registerButton = new QPushButton("Register", this);
    registerButton->setMinimumHeight(35);
    connect(registerButton, &QPushButton::clicked, this, &StudentRegistrationWindow::onRegisterButtonClicked);
    buttonLayout->addWidget(registerButton);

    cancelButton = new QPushButton("Cancel", this);
    cancelButton->setMinimumHeight(35);
    connect(cancelButton, &QPushButton::clicked, this, &StudentRegistrationWindow::onCancelButtonClicked);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
}

void StudentRegistrationWindow::validateEmail(const QString& fullName) {
    if (!fullName.isEmpty()) {
        QString suggested = fullName.toLower();
        suggested = suggested.replace(" ", ".");
        suggested += "@email.com";
        
        if (emailInput->text().isEmpty()) {
            emailInput->setText(suggested);
        }
    }
}

bool StudentRegistrationWindow::validateInputs() {
    // Check full name
    if (fullNameInput->text().trimmed().isEmpty()) {
        statusLabel->setText("Please enter your full name.");
        return false;
    }

    // Check email
    QString email = emailInput->text().trimmed();
    if (email.isEmpty()) {
        statusLabel->setText("Please enter an email address.");
        return false;
    }

    QRegularExpression emailRegex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!emailRegex.match(email).hasMatch()) {
        statusLabel->setText("Please enter a valid email address.");
        return false;
    }

    // Check income
    if (incomeInput->value() < 0) {
        statusLabel->setText("Income cannot be negative.");
        return false;
    }

    // Check marks
    if (marksInput->value() < 0 || marksInput->value() > 100) {
        statusLabel->setText("Marks must be between 0 and 100.");
        return false;
    }

    // Check password
    if (passwordInput->text().isEmpty()) {
        statusLabel->setText("Please enter a password.");
        return false;
    }

    if (passwordInput->text().length() < 6) {
        statusLabel->setText("Password must be at least 6 characters long.");
        return false;
    }

    // Check password confirmation
    if (passwordInput->text() != confirmPasswordInput->text()) {
        statusLabel->setText("Passwords do not match.");
        return false;
    }

    // Check if email already exists
    FileManager fm;
    QVector<Student> students = fm.loadStudents();
    for (const Student& student : students) {
        if (student.getEmail() == email) {
            statusLabel->setText("This email is already registered.");
            return false;
        }
    }

    return true;
}

void StudentRegistrationWindow::onRegisterButtonClicked() {
    statusLabel->setText("");

    if (!validateInputs()) {
        return;
    }

    try {
        FileManager fm;
        QVector<Student> students = fm.loadStudents();

        // Generate new ID
        int newId = students.isEmpty() ? 1 : students.last().getId() + 1;

        // Create new student
        Student newStudent(
            newId,
            fullNameInput->text().trimmed(),
            emailInput->text().trimmed(),
            incomeInput->value(),
            marksInput->value(),
            passwordInput->text()
        );

        // Add student
        students.append(newStudent);
        fm.saveStudents(students);

        statusLabel->setStyleSheet("color: green;");
        statusLabel->setText("Registration successful! You can now log in.");

        // Clear inputs
        fullNameInput->clear();
        emailInput->clear();
        incomeInput->setValue(0);
        marksInput->setValue(0);
        passwordInput->clear();
        confirmPasswordInput->clear();

        // Emit signal
        emit registrationCompleted();

    } catch (const std::exception& e) {
        statusLabel->setText(QString("Error: %1").arg(e.what()));
    }
}

void StudentRegistrationWindow::onCancelButtonClicked() {
    emit registrationCancelled();
    close();
}
