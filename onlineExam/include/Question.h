#ifndef QUESTION_H
#define QUESTION_H

#include <string>
#include <vector>

class Question {
private:
    std::string questionText;
    std::vector<std::string> options;
    int correctIndex;
    std::string difficulty;
    std::string topic;

public:
    Question();

    void setQuestion(const std::string& text,
                     const std::vector<std::string>& opts,
                     int correct,
                     const std::string& diff,
                     const std::string& topic);

    void shuffleOptions();

    std::string getQuestionText() const;
    std::vector<std::string> getOptions() const;
    int getCorrectIndex() const;
};

#endif