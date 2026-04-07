#ifndef ELIGIBILITYSERVICE_H
#define ELIGIBILITYSERVICE_H

#include "../models/Student.h"
#include "../models/Scholarship.h"

class EligibilityService {
public:
    EligibilityService() = default;

    /**
     * Check if a student is eligible for a scholarship
     * @param student The student to check
     * @param scholarship The scholarship to check against
     * @return true if eligible, false otherwise
     */
    static bool checkEligibility(const Student& student, const Scholarship& scholarship);

    /**
     * Get eligibility reason as a readable string
     * @param student The student to check
     * @param scholarship The scholarship to check against
     * @return String describing eligibility status
     */
    static QString getEligibilityReason(const Student& student, const Scholarship& scholarship);

    /**
     * Check if student marks meet minimum requirement
     */
    static bool checkMarksEligibility(float studentMarks, float minMarks);

    /**
     * Check if student income meets maximum requirement
     */
    static bool checkIncomeEligibility(float studentIncome, float maxIncome);
};

#endif // ELIGIBILITYSERVICE_H
