#ifndef APIQUESTIONGENERATOR_H
#define APIQUESTIONGENERATOR_H

#include <vector>
#include <string>
#include "Question.h"

class APIQuestionGenerator {
public:
    // Auto: tries Gemini first, then OpenAI
    static std::vector<Question> generateQuestions(
        const std::string& topic,
        const std::string& difficulty,
        int count);

    // Gemini only
    static std::vector<Question> generateFromGemini(
        const std::string& topic,
        const std::string& difficulty,
        int count);

    // OpenAI only
    static std::vector<Question> generateFromOpenAI(
        const std::string& topic,
        const std::string& difficulty,
        int count);
};

#endif