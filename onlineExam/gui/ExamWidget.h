#ifndef EXAMWIDGET_H
#define EXAMWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QProgressBar>
#include <QTimer>
#include <vector>
#include "Question.h"
#include "ExamConfig.h"

struct AnswerRecord {
    int questionNum;
    std::string questionText;
    int userChoice;    // -1 skipped
    int correctChoice;
    bool isCorrect;
};

class ExamWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ExamWidget(QWidget *parent = nullptr);

    void startExam(const ExamConfig &config,
                   const std::vector<Question> &questions);

signals:
    void examFinished(const std::vector<AnswerRecord> &answers,
                      const ExamConfig &config,
                      int totalQuestions);

private slots:
    void onNextClicked();
    void onSkipClicked();
    void onTimerTick();

private:
    void showQuestion(int index);
    void finishExam();

    // UI
    QLabel        *headerLabel;
    QLabel        *timerLabel;
    QProgressBar  *progressBar;
    QLabel        *questionLabel;
    QRadioButton  *optionButtons[4];
    QButtonGroup  *optionGroup;
    QPushButton   *nextButton;
    QPushButton   *skipButton;
    QLabel        *questionCounter;

    // State
    std::vector<Question>      questions_;
    std::vector<AnswerRecord>  answers_;
    ExamConfig                 config_;
    int                        currentIndex_ = 0;
    int                        remainingSeconds_ = 0;
    QTimer                    *examTimer_;
};

#endif
