#ifndef RESULTANALYZER_H
#define RESULTANALYZER_H

class ResultAnalyzer {
private:
    int correct = 0;
    int wrong = 0;
    int skipped = 0;
    double score = 0.0;

public:
    void recordCorrect();
    void recordWrong();
    void recordSkipped();

    void computeFinal();

    int getCorrect() const;
    int getWrong() const;
    int getSkipped() const;
    double getScore() const;
    double getPercentage(int totalQuestions) const;
};

#endif