#include "MainWindow.h"
#include "QuestionBank.h"
#include "APIQuestionGenerator.h"
#include <QMessageBox>
#include <QApplication>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Online Examination System");
    setMinimumSize(720, 640);
    resize(860, 720);

    stack_ = new QStackedWidget;
    setCentralWidget(stack_);

    setupWidget_  = new SetupWidget;
    examWidget_   = new ExamWidget;
    resultWidget_ = new ResultWidget;

    stack_->addWidget(setupWidget_);   // index 0
    stack_->addWidget(examWidget_);    // index 1
    stack_->addWidget(resultWidget_);  // index 2

    stack_->setCurrentIndex(0);

    connect(setupWidget_, &SetupWidget::startExamRequested,
            this, &MainWindow::onStartExam);
    connect(examWidget_, &ExamWidget::examFinished,
            this, &MainWindow::onExamFinished);
    connect(resultWidget_, &ResultWidget::restartRequested,
            this, &MainWindow::onRestart);
}

void MainWindow::onStartExam(const ExamConfig &config)
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    std::vector<Question> questions;

    // Load questions based on source
    if (config.source == QuestionSource::GEMINI) {
        questions = APIQuestionGenerator::generateFromGemini(
            config.topic, config.getDifficultyString(), config.numQuestions);
    }
    else if (config.source == QuestionSource::OPENAI) {
        questions = APIQuestionGenerator::generateFromOpenAI(
            config.topic, config.getDifficultyString(), config.numQuestions);
    }
    else if (config.source == QuestionSource::AUTO) {
        questions = APIQuestionGenerator::generateQuestions(
            config.topic, config.getDifficultyString(), config.numQuestions);
    }

    // Fallback to file
    if (questions.empty()) {
        QuestionBank qb;
        if (!qb.loadFromFile("../data/questions.txt")) {
            QApplication::restoreOverrideCursor();
            QMessageBox::critical(this, "Error",
                "Failed to load questions from any source.");
            return;
        }
        qb.shuffleQuestions();
        auto &allQ = qb.getQuestions();
        int take = std::min(config.numQuestions, (int)allQ.size());
        questions.assign(allQ.begin(), allQ.begin() + take);

        if (config.source != QuestionSource::FILE) {
            QMessageBox::information(this, "Fallback",
                "AI questions unavailable.\n"
                "Loaded " + QString::number(take) + " questions from local file.");
        }
    }

    QApplication::restoreOverrideCursor();

    examWidget_->startExam(config, questions);
    stack_->setCurrentIndex(1);
}

void MainWindow::onExamFinished(const std::vector<AnswerRecord> &answers,
                                const ExamConfig &config,
                                int totalQuestions)
{
    resultWidget_->showResults(answers, config, totalQuestions);
    stack_->setCurrentIndex(2);
}

void MainWindow::onRestart()
{
    stack_->setCurrentIndex(0);
}
