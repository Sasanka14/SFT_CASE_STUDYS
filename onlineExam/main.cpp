#include <iostream>
#include <limits>
#include <cctype>
#include <iomanip>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <algorithm>

#include "Student.h"
#include "ConsoleUI.h"
#include "QuestionBank.h"
#include "ResultAnalyzer.h"
#include "TimerManager.h"
#include "APIQuestionGenerator.h"
#include "EnvLoader.h"
#include "ExamConfig.h"

// Structure to store student answers
struct Answer
{
    int questionNum;
    std::string questionText;
    int userChoice; // -1 = skipped, -2 = timeout
    int correctChoice;
    bool isCorrect;
};

// Global mutex for thread-safe output
std::mutex outputMutex;
bool waitingForInput = false;

// ================= HELPER: read a line safely =================
static std::string readLine()
{
    std::string line;
    std::getline(std::cin, line);
    return line;
}

static int readInt(int defaultVal = -1)
{
    std::string line = readLine();
    if (line.empty()) return defaultVal;
    try { return std::stoi(line); }
    catch (...) { return defaultVal; }
}

// ================= START LIVE TIMER UPDATE THREAD =================
void startLiveTimerUpdates(TimerManager &timer)
{
    std::thread([&timer]()
                {
        int lastSecond = -1;
        
        while (!timer.isTimeUp())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
            if (waitingForInput)
                continue;
            
            int currentSecond = timer.getRemainingSeconds();
            
            if (currentSecond != lastSecond)
            {
                lastSecond = currentSecond;
                
                int min = currentSecond / 60;
                int rem = currentSecond % 60;
                
                std::lock_guard<std::mutex> lock(outputMutex);
                
                std::cout << "\033[A\r\033[K⏳ Time Left: "
                          << std::setw(2) << std::setfill('0') << min
                          << ":"
                          << std::setw(2) << std::setfill('0') << rem
                          << "\n" << std::flush;
            }
        } })
        .detach();
}

// ================= DISPLAY TIMER STATUS =================
void displayTimerStatus(TimerManager &timer)
{
    int sec = timer.getRemainingSeconds();
    int min = sec / 60;
    int rem = sec % 60;

    std::lock_guard<std::mutex> lock(outputMutex);

    if (sec <= 10 && sec > 0)
    {
        std::cout << "\033[31m⏳ Time Left: "
                  << std::setw(2) << std::setfill('0') << min
                  << ":"
                  << std::setw(2) << std::setfill('0') << rem
                  << " ⚠️ HURRY UP!\033[0m\n";
    }
    else
    {
        std::cout << "⏳ Time Left: "
                  << std::setw(2) << std::setfill('0') << min
                  << ":"
                  << std::setw(2) << std::setfill('0') << rem
                  << "\n";
    }
}

// ================= SAFE INPUT =================
int getUserChoice(TimerManager &timer)
{
    std::string input;

    while (true)
    {
        if (timer.isTimeUp())
            return -2;

        displayTimerStatus(timer);

        waitingForInput = true;
        std::cout << "Enter choice (A-D) or S to skip: ";
        std::cin >> input;
        waitingForInput = false;

        if (timer.isTimeUp())
            return -2;

        if (input.size() != 1)
        {
            ConsoleUI::printError("Invalid input.");
            continue;
        }

        char c = std::toupper(input[0]);

        if (c >= 'A' && c <= 'D')
            return c - 'A';

        if (c == 'S')
            return -1;

        ConsoleUI::printError("Invalid input.");
    }
}

// ================= REVIEW ANSWERS =================
void displayAnswerReview(const std::vector<Answer> &answers)
{
    ConsoleUI::printHeader("\n=== ANSWER REVIEW ===");
    std::cout << "Review your answers before submitting:\n\n";

    int correct = 0, wrong = 0, skipped = 0;

    for (const auto &ans : answers)
    {
        if (ans.userChoice == -1)
        {
            ConsoleUI::printWarning("Q" + std::to_string(ans.questionNum) +
                                    " [SKIPPED] " + ans.questionText);
            skipped++;
        }
        else if (ans.userChoice == -2)
        {
            ConsoleUI::printError("Q" + std::to_string(ans.questionNum) +
                                  " [TIMEOUT] " + ans.questionText);
        }
        else if (ans.isCorrect)
        {
            ConsoleUI::printSuccess("Q" + std::to_string(ans.questionNum) +
                                    " ✓ Correct - You chose: " +
                                    std::string(1, char('A' + ans.userChoice)));
            correct++;
        }
        else
        {
            ConsoleUI::printError("Q" + std::to_string(ans.questionNum) +
                                  " ✗ Wrong - You chose: " +
                                  std::string(1, char('A' + ans.userChoice)) +
                                  ", Correct: " +
                                  std::string(1, char('A' + ans.correctChoice)));
            wrong++;
        }
    }

    std::cout << "\nSummary: " << correct << " correct, "
              << wrong << " wrong, " << skipped << " skipped\n";
}

// ╔══════════════════════════════════════════════════════════════╗
// ║                     MENU SYSTEM                              ║
// ╚══════════════════════════════════════════════════════════════╝

void showConfigSummary(const ExamConfig &cfg)
{
    std::cout << "\n";
    ConsoleUI::printHeader("╔══════════════ Current Settings ══════════════╗");
    std::cout << "  Student Name      : " << (cfg.studentName.empty() ? "(not set)" : cfg.studentName) << "\n";
    std::cout << "  Student ID        : " << (cfg.studentId.empty() ? "(not set)" : cfg.studentId) << "\n";
    std::cout << "  Question Source   : " << cfg.getSourceString() << "\n";
    std::cout << "  Topic             : " << (cfg.topic.empty() ? "(not set)" : cfg.topic) << "\n";
    std::cout << "  No. of Questions  : " << cfg.numQuestions << "\n";
    std::cout << "  Difficulty        : " << cfg.getDifficultyString() << "\n";
    std::cout << "  Exam Time         : " << cfg.examTimeMinutes << " minutes\n";
    std::cout << "  Per-Question Warn : " << cfg.perQuestionWarn << " seconds\n";
    std::cout << "  Passing %         : " << cfg.passingPercent << "%\n";
    ConsoleUI::printHeader("╚══════════════════════════════════════════════╝");
    std::cout << "\n";
}

void showMainMenu()
{
    ConsoleUI::printHeader("╔══════════════ EXAM SETUP MENU ══════════════╗");
    std::cout << "  1. Set Student Info\n";
    std::cout << "  2. Set Question Source (File / Gemini / OpenAI / Auto)\n";
    std::cout << "  3. Set Topic (for AI questions)\n";
    std::cout << "  4. Set Number of Questions\n";
    std::cout << "  5. Set Difficulty Level\n";
    std::cout << "  6. Set Exam Duration\n";
    std::cout << "  7. Set Passing Percentage\n";
    std::cout << "  8. View Current Settings\n";
    std::cout << "  9. Start Exam 🚀\n";
    std::cout << "  0. Exit\n";
    ConsoleUI::printHeader("╚══════════════════════════════════════════════╝");
    std::cout << "  Enter choice: ";
}

ExamConfig runSetupMenu()
{
    ExamConfig cfg;

    // Set some defaults
    cfg.numQuestions    = 10;
    cfg.examTimeMinutes = 5;
    cfg.difficulty      = Difficulty::MEDIUM;
    cfg.source          = QuestionSource::AUTO;
    cfg.passingPercent  = 60.0;

    while (true)
    {
        showMainMenu();
        int choice = readInt(-1);

        switch (choice)
        {
        case 1: { // Student Info
            std::cout << "\n  Enter student name: ";
            cfg.studentName = readLine();
            std::cout << "  Enter student ID: ";
            cfg.studentId = readLine();
            ConsoleUI::printSuccess("  ✓ Student info updated.");
            break;
        }

        case 2: { // Question Source
            std::cout << "\n  Select question source:\n";
            std::cout << "    1. Local File (data/questions.txt)\n";
            std::cout << "    2. Gemini AI (free tier)\n";
            std::cout << "    3. OpenAI (requires paid key)\n";
            std::cout << "    4. Auto (Gemini → OpenAI → File fallback)\n";
            std::cout << "  Choice: ";
            int src = readInt(4);
            switch (src) {
                case 1: cfg.source = QuestionSource::FILE;   break;
                case 2: cfg.source = QuestionSource::GEMINI; break;
                case 3: cfg.source = QuestionSource::OPENAI; break;
                default: cfg.source = QuestionSource::AUTO;  break;
            }
            ConsoleUI::printSuccess("  ✓ Source set to: " + cfg.getSourceString());
            break;
        }

        case 3: { // Topic
            std::cout << "\n  Enter topic for AI questions (e.g. Java, Python, Math): ";
            cfg.topic = readLine();
            ConsoleUI::printSuccess("  ✓ Topic set to: " + cfg.topic);
            break;
        }

        case 4: { // Number of Questions
            std::cout << "\n  Enter number of questions (1-500): ";
            int n = readInt(10);
            if (n < 1) n = 1;
            if (n > 500) n = 500;
            cfg.numQuestions = n;
            ConsoleUI::printSuccess("  ✓ Questions set to: " + std::to_string(n));
            break;
        }

        case 5: { // Difficulty
            std::cout << "\n  Select difficulty:\n";
            std::cout << "    1. Easy\n";
            std::cout << "    2. Medium\n";
            std::cout << "    3. Hard\n";
            std::cout << "    4. Mixed\n";
            std::cout << "  Choice: ";
            int d = readInt(2);
            switch (d) {
                case 1: cfg.difficulty = Difficulty::EASY;   break;
                case 2: cfg.difficulty = Difficulty::MEDIUM; break;
                case 3: cfg.difficulty = Difficulty::HARD;   break;
                case 4: cfg.difficulty = Difficulty::MIXED;  break;
                default: cfg.difficulty = Difficulty::MEDIUM; break;
            }
            ConsoleUI::printSuccess("  ✓ Difficulty set to: " + cfg.getDifficultyString());
            break;
        }

        case 6: { // Exam Duration
            std::cout << "\n  Enter exam duration in minutes (1-180): ";
            int t = readInt(5);
            if (t < 1) t = 1;
            if (t > 180) t = 180;
            cfg.examTimeMinutes = t;
            ConsoleUI::printSuccess("  ✓ Exam time set to: " + std::to_string(t) + " minutes");
            break;
        }

        case 7: { // Passing Percentage
            std::cout << "\n  Enter passing percentage (1-100): ";
            int p = readInt(60);
            if (p < 1) p = 1;
            if (p > 100) p = 100;
            cfg.passingPercent = static_cast<double>(p);
            ConsoleUI::printSuccess("  ✓ Passing % set to: " + std::to_string(p) + "%");
            break;
        }

        case 8: { // View Settings
            showConfigSummary(cfg);
            break;
        }

        case 9: { // Start Exam
            // Validate required fields
            if (cfg.studentName.empty() || cfg.studentId.empty()) {
                ConsoleUI::printError("  ❌ Please set student name and ID first! (Option 1)");
                break;
            }
            if (cfg.source != QuestionSource::FILE && cfg.topic.empty()) {
                ConsoleUI::printError("  ❌ Please set a topic for AI questions! (Option 3)");
                break;
            }
            showConfigSummary(cfg);
            std::cout << "  Press Enter to start the exam...";
            readLine();
            return cfg;
        }

        case 0: { // Exit
            std::cout << "\n  Goodbye! 👋\n\n";
            std::exit(0);
        }

        default:
            ConsoleUI::printError("  Invalid choice. Try again.");
            break;
        }
    }
}

// ╔══════════════════════════════════════════════════════════════╗
// ║                       EXAM ENGINE                            ║
// ╚══════════════════════════════════════════════════════════════╝

void runExam(const ExamConfig &cfg)
{
    Student s(cfg.studentName, cfg.studentId);

    std::cout << "\n";
    ConsoleUI::printHeader("=== EXAM STARTING ===");
    std::cout << "Student : " << s.getName()
              << " | ID: " << s.getId() << "\n";
    std::cout << "Source  : " << cfg.getSourceString() << "\n";
    std::cout << "Questions: " << cfg.numQuestions
              << " | Time: " << cfg.examTimeMinutes << " min"
              << " | Pass: " << cfg.passingPercent << "%\n\n";

    // ---------- Load Questions ----------
    std::vector<Question> questions;

    if (cfg.source == QuestionSource::GEMINI)
    {
        questions = APIQuestionGenerator::generateFromGemini(
            cfg.topic, cfg.getDifficultyString(), cfg.numQuestions);
    }
    else if (cfg.source == QuestionSource::OPENAI)
    {
        questions = APIQuestionGenerator::generateFromOpenAI(
            cfg.topic, cfg.getDifficultyString(), cfg.numQuestions);
    }
    else if (cfg.source == QuestionSource::AUTO)
    {
        questions = APIQuestionGenerator::generateQuestions(
            cfg.topic, cfg.getDifficultyString(), cfg.numQuestions);
    }
    // FILE source or fallback if AI returned nothing
    if (questions.empty())
    {
        if (cfg.source != QuestionSource::FILE)
            ConsoleUI::printWarning("⚠️ AI questions unavailable. Falling back to local file.");

        QuestionBank qb;
        if (!qb.loadFromFile("../data/questions.txt"))
        {
            ConsoleUI::printError("❌ Failed to load questions from file.");
            return;
        }
        qb.shuffleQuestions();
        auto &allQ = qb.getQuestions();

        // Take only the requested number of questions
        int take = std::min(cfg.numQuestions, (int)allQ.size());
        questions.assign(allQ.begin(), allQ.begin() + take);
        ConsoleUI::printSuccess("✅ Loaded " + std::to_string(take) + " questions from file.");
    }

    // ---------- Start Timer ----------
    TimerManager timer;
    timer.startExamTimer(cfg.examTimeMinutes * 60);
    startLiveTimerUpdates(timer);

    // ---------- Exam Loop ----------
    ResultAnalyzer analyzer;
    std::vector<Answer> answers;

    int qno = 1;
    for (const auto &q : questions)
    {
        if (timer.isTimeUp())
        {
            ConsoleUI::printError("\n⏰ Exam time finished!");
            break;
        }

        ConsoleUI::printHeader(
            "\nQuestion " + std::to_string(qno) + "/" +
            std::to_string(questions.size()) + ":");

        const_cast<Question &>(q).shuffleOptions();

        std::cout << q.getQuestionText() << "\n";

        auto opts = q.getOptions();
        for (size_t i = 0; i < opts.size(); ++i)
        {
            std::cout << "   " << char('A' + i)
                      << ") " << opts[i] << "\n";
        }

        auto questionStart = std::chrono::steady_clock::now();
        int choice = getUserChoice(timer);
        std::cout << "\n";

        if (choice == -2)
        {
            ConsoleUI::printError("⏰ Exam time finished!");
            break;
        }

        auto questionEnd = std::chrono::steady_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::seconds>(
                questionEnd - questionStart)
                .count();

        if (elapsed >= cfg.perQuestionWarn)
        {
            ConsoleUI::printWarning("⚠️ You took " + std::to_string(elapsed) + "s on this question.");
        }

        if (choice == -1)
        {
            analyzer.recordSkipped();
            answers.push_back({qno, q.getQuestionText(), -1, q.getCorrectIndex(), false});
        }
        else
        {
            bool isCorrect = (choice == q.getCorrectIndex());
            if (isCorrect)
                analyzer.recordCorrect();
            else
                analyzer.recordWrong();
            answers.push_back({qno, q.getQuestionText(), choice, q.getCorrectIndex(), isCorrect});
        }

        qno++;
    }

    // ---------- Stop Timer ----------
    timer.stop();

    // ---------- Review ----------
    displayAnswerReview(answers);

    std::cout << "\nPress Enter to view final results...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // ---------- Final Result ----------
    analyzer.computeFinal();

    ConsoleUI::printHeader("\n=== FINAL RESULT ===");

    int total = questions.size();
    double percentage = analyzer.getPercentage(total);

    std::cout << "Student         : " << cfg.studentName
              << " (" << cfg.studentId << ")\n";
    std::cout << "Total Questions : " << total << "\n";
    std::cout << "Correct         : " << analyzer.getCorrect() << "\n";
    std::cout << "Wrong           : " << analyzer.getWrong() << "\n";
    std::cout << "Skipped         : " << analyzer.getSkipped() << "\n";
    std::cout << "Score           : " << analyzer.getScore() << "\n";
    std::cout << "Percentage      : "
              << std::fixed << std::setprecision(2) << percentage << "%\n";
    std::cout << "Required        : " << cfg.passingPercent << "%\n\n";

    if (percentage >= cfg.passingPercent)
        ConsoleUI::printPassBadge();
    else
        ConsoleUI::printFailBadge();
}

// ========================= MAIN =========================
int main()
{
    // Load environment variables from .env file
    EnvLoader::load("../.env");

    ConsoleUI::printHeader("\n  ╔════════════════════════════════════════════╗");
    ConsoleUI::printHeader("  ║       ONLINE EXAMINATION SYSTEM            ║");
    ConsoleUI::printHeader("  ╚════════════════════════════════════════════╝\n");

    ConsoleUI::printSuccess("  System initialized successfully.\n");

    // Run setup menu → get config → start exam
    ExamConfig config = runSetupMenu();
    runExam(config);

    return 0;
}