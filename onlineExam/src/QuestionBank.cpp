#include "QuestionBank.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <set>

bool QuestionBank::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    questions.clear();
    std::set<std::string> seen; // track unique question texts

    std::string line;
    while (std::getline(file, line)) {
        // Expected format:
        // question|opt1|opt2|opt3|opt4|correctIndex|difficulty|topic

        std::stringstream ss(line);
        std::vector<std::string> parts;
        std::string part;

        while (std::getline(ss, part, '|')) {
            parts.push_back(part);
        }

        if (parts.size() < 8) continue;

        // Skip duplicates
        if (seen.count(parts[0])) continue;
        seen.insert(parts[0]);

        Question q;
        std::vector<std::string> opts = {
            parts[1], parts[2], parts[3], parts[4]
        };

        q.setQuestion(
            parts[0],
            opts,
            std::stoi(parts[5]),
            parts[6],
            parts[7]
        );

        questions.push_back(q);
    }

    return !questions.empty();
}

std::vector<Question>& QuestionBank::getQuestions() {
    return questions;
}

void QuestionBank::shuffleQuestions() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(questions.begin(), questions.end(), g);
}