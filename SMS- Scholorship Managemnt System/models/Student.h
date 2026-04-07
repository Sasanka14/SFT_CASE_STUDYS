#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QDebug>
#include <QStringList>

class Student {
private:
    int id;
    QString name;
    QString email;
    float income;
    float marks;
    QString password;

public:
    // Constructors
    Student();
    Student(int id, const QString& name, const QString& email, float income, float marks, const QString& password = "");

    // Getters
    int getId() const { return id; }
    QString getName() const { return name; }
    QString getEmail() const { return email; }
    float getIncome() const { return income; }
    float getMarks() const { return marks; }
    QString getPassword() const { return password; }

    // Setters
    void setId(int id) { this->id = id; }
    void setName(const QString& name) { this->name = name; }
    void setEmail(const QString& email) { this->email = email; }
    void setIncome(float income) { this->income = income; }
    void setMarks(float marks) { this->marks = marks; }
    void setPassword(const QString& pwd) { this->password = pwd; }

    // Serialization
    QString serialize() const;
    static Student deserialize(const QString& line);

    // Display
    QString toString() const;
    bool isValid() const;
};

#endif // STUDENT_H
