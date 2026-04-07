#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "SetupWidget.h"
#include "ExamWidget.h"
#include "ResultWidget.h"
#include "ExamConfig.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onStartExam(const ExamConfig &config);
    void onExamFinished(const std::vector<AnswerRecord> &answers,
                        const ExamConfig &config,
                        int totalQuestions);
    void onRestart();

private:
    QStackedWidget *stack_;
    SetupWidget    *setupWidget_;
    ExamWidget     *examWidget_;
    ResultWidget   *resultWidget_;
};

#endif
