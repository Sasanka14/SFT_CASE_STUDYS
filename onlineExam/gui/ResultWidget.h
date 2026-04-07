#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <vector>
#include "ExamConfig.h"
#include "ExamWidget.h" // for AnswerRecord

class ResultWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ResultWidget(QWidget *parent = nullptr);

    void showResults(const std::vector<AnswerRecord> &answers,
                     const ExamConfig &config,
                     int totalQuestions);

signals:
    void restartRequested();

private:
    QLabel       *titleLabel;
    QLabel       *studentLabel;
    QLabel       *scoreLabel;
    QLabel       *percentLabel;
    QLabel       *badgeLabel;
    QLabel       *statsLabel;
    QTableWidget *reviewTable;
    QPushButton  *restartButton;
    QPushButton  *quitButton;
};

#endif
