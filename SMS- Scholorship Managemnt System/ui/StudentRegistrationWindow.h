#ifndef STUDENTREGISTRATIONWINDOW_H
#define STUDENTREGISTRATIONWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>

class StudentRegistrationWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit StudentRegistrationWindow(QWidget* parent = nullptr);

signals:
    void registrationCompleted();
    void registrationCancelled();
    void registrationError(const QString& message);

private slots:
    void onRegisterButtonClicked();
    void onCancelButtonClicked();
    void validateEmail(const QString& email);

private:
    void setupUI();
    bool validateInputs();
    QString generateEmail(const QString& fullName);

    // UI Components
    QLineEdit* fullNameInput;
    QLineEdit* emailInput;
    QDoubleSpinBox* incomeInput;
    QSpinBox* marksInput;
    QLineEdit* passwordInput;
    QLineEdit* confirmPasswordInput;
    QPushButton* registerButton;
    QPushButton* cancelButton;
    QLabel* statusLabel;
};

#endif // STUDENTREGISTRATIONWINDOW_H
