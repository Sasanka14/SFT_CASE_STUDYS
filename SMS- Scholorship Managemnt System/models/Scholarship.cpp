#include "Scholarship.h"
#include <QStringList>

Scholarship::Scholarship() 
    : id(0), name(""), minMarks(0.0f), maxIncome(0.0f), amount(0.0f) {}

Scholarship::Scholarship(int id, const QString& name, float minMarks, float maxIncome, float amount)
    : id(id), name(name), minMarks(minMarks), maxIncome(maxIncome), amount(amount) {}

QString Scholarship::serialize() const {
    return QString("%1|%2|%3|%4|%5")
        .arg(id)
        .arg(name)
        .arg(minMarks)
        .arg(maxIncome)
        .arg(amount);
}

Scholarship Scholarship::deserialize(const QString& line) {
    QStringList parts = line.split("|");
    if (parts.size() < 4) {
        return Scholarship();
    }
    
    float scholarshipAmount = parts.size() > 4 ? parts[4].toFloat() : 50000.0f;
    
    return Scholarship(
        parts[0].toInt(),
        parts[1],
        parts[2].toFloat(),
        parts[3].toFloat(),
        scholarshipAmount
    );
}

QString Scholarship::toString() const {
    return QString("ID: %1 | Name: %2 | Min Marks: %3 | Max Income: ₹%4 | Amount: ₹%5")
        .arg(id)
        .arg(name)
        .arg(minMarks, 0, 'f', 0)
        .arg(maxIncome, 0, 'f', 2)
        .arg(amount, 0, 'f', 2);
}

bool Scholarship::isValid() const {
    return id > 0 && !name.isEmpty() && minMarks >= 0 && minMarks <= 100 && 
           maxIncome > 0 && amount > 0;
}
