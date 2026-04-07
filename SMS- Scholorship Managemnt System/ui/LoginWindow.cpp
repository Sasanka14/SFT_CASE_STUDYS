#include "LoginWindow.h"
#include "../services/FileManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QLineEdit>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget* parent)
    : QMainWindow(parent) {
    setupUI();
    applyStyles();
    connectSignals();
    showRoleSelection();
}

LoginWindow::~LoginWindow() {}

void LoginWindow::setupUI() {
    setWindowTitle("Scholarship Management System");
    setWindowIconText("SMS");
    setGeometry(100, 100, 600, 500);

    // Central widget
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(40, 40, 40, 40);

    // Title
    titleLabel = new QLabel("Scholarship Management System", this);
    QFont titleFont("Arial", 24, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Subtitle
    subtitleLabel = new QLabel("", this);
    QFont subtitleFont("Arial", 12);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(subtitleLabel);

    mainLayout->addSpacing(20);

    // Status Label (for error messages)
    statusLabel = new QLabel("", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("color: red; font-weight: bold;");
    mainLayout->addWidget(statusLabel);

    // Email Input
    mainLayout->addWidget(new QLabel("Email:", this));
    emailInput = new QLineEdit(this);
    emailInput->setPlaceholderText("Enter your email");
    emailInput->setMinimumHeight(35);
    mainLayout->addWidget(emailInput);

    // Password Input
    mainLayout->addWidget(new QLabel("Password:", this));
    passwordInput = new QLineEdit(this);
    passwordInput->setPlaceholderText("Enter your password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setMinimumHeight(35);
    mainLayout->addWidget(passwordInput);

    mainLayout->addSpacing(10);

    // Login Button
    loginButton = new QPushButton("Login", this);
    loginButton->setMinimumHeight(40);
    mainLayout->addWidget(loginButton);

    // Register Button
    registerButton = new QPushButton("Register as New Student", this);
    registerButton->setMinimumHeight(40);
    mainLayout->addWidget(registerButton);

    // Admin Button
    adminButton = new QPushButton("Admin Login", this);
    adminButton->setMinimumHeight(40);
    mainLayout->addWidget(adminButton);

    // Back Button
    backButton = new QPushButton("Back", this);
    backButton->setMinimumHeight(40);
    mainLayout->addWidget(backButton);

    // Role Selection Buttons (initially hidden, shown with role selection)
    studentRoleButton = new QPushButton("Student Login", this);
    studentRoleButton->setMinimumHeight(50);
    studentRoleButton->setVisible(false);
    QFont buttonFont("Arial", 12, QFont::Bold);
    studentRoleButton->setFont(buttonFont);
    mainLayout->addWidget(studentRoleButton);

    adminRoleButton = new QPushButton("Admin Login", this);
    adminRoleButton->setMinimumHeight(50);
    adminRoleButton->setFont(buttonFont);
    adminRoleButton->setVisible(false);
    mainLayout->addWidget(adminRoleButton);

    // Exit Button
    exitButton = new QPushButton("Exit", this);
    exitButton->setMinimumHeight(40);
    mainLayout->addWidget(exitButton);

    mainLayout->addStretch();
    mainLayout->setAlignment(Qt::AlignTop);
}

void LoginWindow::applyStyles() {
    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #1e1e2e;
        }
        
        QLabel {
            color: #ffffff;
        }
        
        QLineEdit {
            background-color: #2d2d44;
            color: #ffffff;
            border: 2px solid #00a86b;
            border-radius: 5px;
            padding: 5px;
            font-size: 12px;
        }
        
        QLineEdit:focus {
            border: 2px solid #00c77d;
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
    )");

    exitButton->setStyleSheet(R"(
        QPushButton {
            background-color: #e74c3c;
            color: white;
            border: none;
            border-radius: 5px;
            font-weight: bold;
            padding: 8px;
        }
        
        QPushButton:hover {
            background-color: #c0392b;
        }
        
        QPushButton:pressed {
            background-color: #a93226;
        }
    )");

    backButton->setStyleSheet(R"(
        QPushButton {
            background-color: #3498db;
            color: white;
            border: none;
            border-radius: 5px;
            font-weight: bold;
            padding: 8px;
        }
        
        QPushButton:hover {
            background-color: #2980b9;
        }
        
        QPushButton:pressed {
            background-color: #21618c;
        }
    )");
}

void LoginWindow::connectSignals() {
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterButtonClicked);
    connect(adminButton, &QPushButton::clicked, this, &LoginWindow::onAdminButtonClicked);
    connect(adminRoleButton, &QPushButton::clicked, this, &LoginWindow::onAdminButtonClicked);
    connect(studentRoleButton, &QPushButton::clicked, [this]() { showLoginForm(); });
    connect(backButton, &QPushButton::clicked, this, &LoginWindow::showRoleSelection);
    connect(exitButton, &QPushButton::clicked, this, &QWidget::close);
}

void LoginWindow::showRoleSelection() {
    // Show role selection buttons
    studentRoleButton->setVisible(true);
    adminRoleButton->setVisible(true);
    
    // Hide login form
    emailInput->setVisible(false);
    passwordInput->setVisible(false);
    loginButton->setVisible(false);
    registerButton->setVisible(false);
    adminButton->setVisible(false);
    backButton->setVisible(false);
    
    // Hide related labels
    auto labels = findChildren<QLabel*>();
    for (auto label : labels) {
        if (label->text() == "Email:" || label->text() == "Password:") {
            label->setVisible(false);
        }
    }
    
    subtitleLabel->setText("Please select your role to continue");
    statusLabel->setText("");
}

void LoginWindow::showLoginForm() {
    // Show login form
    emailInput->setVisible(true);
    passwordInput->setVisible(true);
    loginButton->setVisible(true);
    registerButton->setVisible(true);
    adminButton->setVisible(true);
    backButton->setVisible(true);
    
    // Show related labels
    auto labels = findChildren<QLabel*>();
    for (auto label : labels) {
        if (label->text() == "Email:" || label->text() == "Password:") {
            label->setVisible(true);
        }
    }
    
    // Hide role selection
    studentRoleButton->setVisible(false);
    adminRoleButton->setVisible(false);
    
    subtitleLabel->setText("Enter your credentials to login");
    statusLabel->setText("");
    
    // Clear inputs
    emailInput->clear();
    passwordInput->clear();
    emailInput->setFocus();
}

void LoginWindow::onLoginButtonClicked() {
    QString email = emailInput->text().trimmed();
    QString password = passwordInput->text();

    if (email.isEmpty() || password.isEmpty()) {
        statusLabel->setText("Please enter both email and password.");
        return;
    }

    // Authenticate student
    FileManager fm;
    Student student = fm.authenticateStudent(email, password);

    if (!student.isValid()) {
        statusLabel->setText("Invalid email or password. Please try again.");
        passwordInput->clear();
        return;
    }

    // Successful login
    statusLabel->setText("");
    emit studentLoginSuccessful(student);
}

void LoginWindow::onRegisterButtonClicked() {
    emit registrationRequested();
}

void LoginWindow::onAdminButtonClicked() {
    emit adminLoginRequested();
}

