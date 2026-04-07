#include "EligibilityService.h"
#include <QString>

bool EligibilityService::checkEligibility(const Student& student, const Scholarship& scholarship) {
    if (!student.isValid() || !scholarship.isValid()) {
        return false;
    }

    return checkMarksEligibility(student.getMarks(), scholarship.getMinMarks()) &&
           checkIncomeEligibility(student.getIncome(), scholarship.getMaxIncome());
}

QString EligibilityService::getEligibilityReason(const Student& student, const Scholarship& scholarship) {
    if (!student.isValid()) {
        return "Invalid student data";
    }
    if (!scholarship.isValid()) {
        return "Invalid scholarship data";
    }

    bool marksOk = checkMarksEligibility(student.getMarks(), scholarship.getMinMarks());
    bool incomeOk = checkIncomeEligibility(student.getIncome(), scholarship.getMaxIncome());

    if (marksOk && incomeOk) {
        return "Eligible";
    }

    QString reason = "Not Eligible: ";
    if (!marksOk) {
        reason += QString("Marks %1 < Required %2")
            .arg(student.getMarks(), 0, 'f', 2)
            .arg(scholarship.getMinMarks(), 0, 'f', 2);
    }

    if (!incomeOk) {
        if (!marksOk) reason += "; ";
        reason += QString("Income $%1 > Limit $%2")
            .arg(student.getIncome(), 0, 'f', 2)
            .arg(scholarship.getMaxIncome(), 0, 'f', 2);
    }

    return reason;
}

bool EligibilityService::checkMarksEligibility(float studentMarks, float minMarks) {
    return studentMarks >= minMarks;
}

bool EligibilityService::checkIncomeEligibility(float studentIncome, float maxIncome) {
    return studentIncome <= maxIncome;
}
