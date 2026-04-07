#ifndef SCHOLARSHIP_H
#define SCHOLARSHIP_H

#include <QString>
#include <QStringList>

class Scholarship {
private:
    int id;
    QString name;
    float minMarks;
    float maxIncome;
    float amount; // Added for completeness

public:
    // Constructors
    Scholarship();
    Scholarship(int id, const QString& name, float minMarks, float maxIncome, float amount = 50000.0f);

    // Getters
    int getId() const { return id; }
    QString getName() const { return name; }
    float getMinMarks() const { return minMarks; }
    float getMaxIncome() const { return maxIncome; }
    float getAmount() const { return amount; }

    // Setters
    void setId(int id) { this->id = id; }
    void setName(const QString& name) { this->name = name; }
    void setMinMarks(float minMarks) { this->minMarks = minMarks; }
    void setMaxIncome(float maxIncome) { this->maxIncome = maxIncome; }
    void setAmount(float amount) { this->amount = amount; }

    // Serialization
    QString serialize() const;
    static Scholarship deserialize(const QString& line);

    // Display
    QString toString() const;
    bool isValid() const;
};

#endif // SCHOLARSHIP_H
