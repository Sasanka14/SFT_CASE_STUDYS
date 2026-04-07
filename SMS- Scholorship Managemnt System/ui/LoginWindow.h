#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "../models/Student.h"

class LoginWindow : public QMainWindow {
    Q_OBJECT

public:
    LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();

    // Signals to notify parent about login
signals:
    void studentLoginSuccessful(const Student& student);
    void adminLoginRequested();
    void registrationRequested();

private slots:
    void onLoginButtonClicked();
    void onRegisterButtonClicked();
    void onAdminButtonClicked();

private:
    void setupUI();
    void applyStyles();
    void connectSignals();
    void showLoginForm();
    void showRoleSelection();

    // UI Components
    QLabel* titleLabel;
    QLabel* subtitleLabel;
    QLabel* statusLabel;
    
    // Login Form Components
    QLineEdit* emailInput;
    QLineEdit* passwordInput;
    QPushButton* loginButton;
    QPushButton* registerButton;
    QPushButton* adminButton;
    QPushButton* backButton;
    QPushButton* exitButton;
    
    // Role Selection Components
    QPushButton* studentRoleButton;
    QPushButton* adminRoleButton;
};

#endif // LOGINWINDOW_H
