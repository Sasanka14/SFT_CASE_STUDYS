#ifndef QUESTIONBANK_H
#define QUESTIONBANK_H

#include <vector>
#include <string>
#include "Question.h"

class QuestionBank {
private:
    std::vector<Question> questions;

public:
    bool loadFromFile(const std::string& filename);
    std::vector<Question>& getQuestions();
    void shuffleQuestions();
};

#endif