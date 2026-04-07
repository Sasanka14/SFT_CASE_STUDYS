#include <QApplication>
#include "ui/LoginWindow.h"
#include "ui/StudentDashboard.h"
#include "ui/AdminDashboard.h"
#include "ui/StudentRegistrationWindow.h"
#include "services/FileManager.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Initialize file manager and data
    FileManager fm("./data");
    fm.ensureDataDirectoryExists();

    // Create main windows
    LoginWindow* loginWindow = new LoginWindow();
    StudentDashboard* studentDashboard = nullptr;
    AdminDashboard* adminDashboard = nullptr;
    StudentRegistrationWindow* registrationWindow = nullptr;

    // Connect login signals for successful student login
    QObject::connect(loginWindow, &LoginWindow::studentLoginSuccessful, [&](const Student& student) {
        loginWindow->hide();
        studentDashboard = new StudentDashboard(student);
        studentDashboard->show();

        QObject::connect(studentDashboard, &StudentDashboard::logoutRequested, [&]() {
            studentDashboard->close();
            delete studentDashboard;
            studentDashboard = nullptr;
            loginWindow->show();
        });
    });

    // Connect registration request signal
    QObject::connect(loginWindow, &LoginWindow::registrationRequested, [&]() {
        if (!registrationWindow) {
            registrationWindow = new StudentRegistrationWindow(loginWindow);
            
            QObject::connect(registrationWindow, &StudentRegistrationWindow::registrationCompleted, [&]() {
                registrationWindow->close();
                delete registrationWindow;
                registrationWindow = nullptr;
                // Show success message and return to login
                loginWindow->show();
            });

            QObject::connect(registrationWindow, &StudentRegistrationWindow::registrationCancelled, [&]() {
                registrationWindow->close();
                delete registrationWindow;
                registrationWindow = nullptr;
                loginWindow->show();
            });
        }
        registrationWindow->show();
    });

    // Connect admin login signal
    QObject::connect(loginWindow, &LoginWindow::adminLoginRequested, [&]() {
        loginWindow->hide();
        adminDashboard = new AdminDashboard();
        adminDashboard->show();

        QObject::connect(adminDashboard, &AdminDashboard::logoutRequested, [&]() {
            adminDashboard->close();
            delete adminDashboard;
            adminDashboard = nullptr;
            loginWindow->show();
        });
    });

    loginWindow->show();
    return app.exec();
}

