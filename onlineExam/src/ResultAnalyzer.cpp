#include "ResultAnalyzer.h"

void ResultAnalyzer::recordCorrect() { correct++; }
void ResultAnalyzer::recordWrong() { wrong++; }
void ResultAnalyzer::recordSkipped() { skipped++; }

void ResultAnalyzer::computeFinal() {
    score = (correct * 4.0) - (wrong * 1.0) - (skipped * 0.35);
}

int ResultAnalyzer::getCorrect() const { return correct; }
int ResultAnalyzer::getWrong() const { return wrong; }
int ResultAnalyzer::getSkipped() const { return skipped; }
double ResultAnalyzer::getScore() const { return score; }

double ResultAnalyzer::getPercentage(int totalQuestions) const {
    if (totalQuestions == 0) return 0.0;
    double maxScore = totalQuestions * 4.0;
    return (score / maxScore) * 100.0;
}