#ifndef SETUPWIDGET_H
#define SETUPWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include "ExamConfig.h"

class SetupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SetupWidget(QWidget *parent = nullptr);
    ExamConfig getConfig() const;

signals:
    void startExamRequested(const ExamConfig &config);

private slots:
    void onStartClicked();
    void onSourceChanged(int index);

private:
    QLineEdit      *nameEdit;
    QLineEdit      *idEdit;
    QComboBox      *sourceCombo;
    QLineEdit      *topicEdit;
    QSpinBox       *numQuestionsSpin;
    QComboBox      *difficultyCombo;
    QSpinBox       *timeSpin;
    QDoubleSpinBox *passingPercentSpin;
    QPushButton    *startButton;
    QLabel         *topicLabel;
};

#endif
