#include "Question.h"
#include <algorithm>
#include <random>

Question::Question() : correctIndex(0) {}

void Question::setQuestion(const std::string& text,
                           const std::vector<std::string>& opts,
                           int correct,
                           const std::string& diff,
                           const std::string& topicName) {
    questionText = text;
    options = opts;
    correctIndex = correct;
    difficulty = diff;
    topic = topicName;
}

void Question::shuffleOptions() {
    std::vector<std::pair<std::string, bool>> temp;

    for (int i = 0; i < options.size(); ++i) {
        temp.push_back({options[i], i == correctIndex});
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(temp.begin(), temp.end(), g);

    for (int i = 0; i < temp.size(); ++i) {
        options[i] = temp[i].first;
        if (temp[i].second) {
            correctIndex = i;
        }
    }
}

std::string Question::getQuestionText() const {
    return questionText;
}

std::vector<std::string> Question::getOptions() const {
    return options;
}

int Question::getCorrectIndex() const {
    return correctIndex;
}