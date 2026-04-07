#ifndef EXAMCONFIG_H
#define EXAMCONFIG_H

#include <string>
#include <vector>

enum class QuestionSource {
    FILE,
    GEMINI,
    OPENAI,
    AUTO    // Gemini first, then OpenAI fallback
};

enum class Difficulty {
    EASY,
    MEDIUM,
    HARD,
    MIXED
};

struct ExamConfig {
    // Student info
    std::string studentName = "";
    std::string studentId   = "";

    // Question settings
    QuestionSource source      = QuestionSource::AUTO;
    std::string    topic       = "";
    int            numQuestions = 10;
    Difficulty     difficulty   = Difficulty::MEDIUM;

    // Timer settings
    int examTimeMinutes = 5;       // total exam time in minutes
    int perQuestionWarn = 30;      // warn if question takes > N seconds

    // Scoring
    double passingPercent = 60.0;

    // Helpers
    std::string getDifficultyString() const {
        switch (difficulty) {
            case Difficulty::EASY:   return "easy";
            case Difficulty::MEDIUM: return "medium";
            case Difficulty::HARD:   return "hard";
            case Difficulty::MIXED:  return "mixed";
        }
        return "medium";
    }

    std::string getSourceString() const {
        switch (source) {
            case QuestionSource::FILE:   return "Local File";
            case QuestionSource::GEMINI: return "Gemini AI";
            case QuestionSource::OPENAI: return "OpenAI";
            case QuestionSource::AUTO:   return "Auto (Gemini → OpenAI)";
        }
        return "Auto";
    }
};

#endif
