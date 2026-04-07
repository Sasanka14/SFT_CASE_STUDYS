#include "Student.h"
#include <QStringList>

Student::Student() : id(0), name(""), email(""), income(0.0f), marks(0.0f), password("") {}

Student::Student(int id, const QString& name, const QString& email, float income, float marks, const QString& password)
    : id(id), name(name), email(email), income(income), marks(marks), password(password) {}

QString Student::serialize() const {
    return QString("%1|%2|%3|%4|%5|%6")
        .arg(id)
        .arg(name)
        .arg(email)
        .arg(income)
        .arg(marks)
        .arg(password);
}

Student Student::deserialize(const QString& line) {
    QStringList parts = line.split("|");
    if (parts.size() < 6) {
        qWarning() << "Invalid student data format - expected 6 fields, got" << parts.size();
        return Student();
    }
    
    return Student(
        parts[0].toInt(),
        parts[1],
        parts[2],
        parts[3].toFloat(),
        parts[4].toFloat(),
        parts[5]
    );
}

QString Student::toString() const {
    return QString("ID: %1 | Name: %2 | Email: %3 | Income: ₹%4 | Marks: %5")
        .arg(id)
        .arg(name)
        .arg(email)
        .arg(income, 0, 'f', 0)
        .arg(marks, 0, 'f', 1);
}

bool Student::isValid() const {
    return id > 0 && !name.isEmpty() && !email.isEmpty() && income >= 0 && marks >= 0 && marks <= 100 && !password.isEmpty();
}
