#include "ExamWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QMessageBox>
#include <QFrame>
#include <QScrollArea>
#include <QPainter>

ExamWidget::ExamWidget(QWidget *parent) : QWidget(parent)
{
    auto *outerLayout = new QVBoxLayout(this);
    outerLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    outerLayout->setContentsMargins(20, 20, 20, 20);

    auto *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setObjectName("setupScroll");
    outerLayout->addWidget(scroll);

    auto *card = new QWidget;
    card->setObjectName("setupCard");
    card->setFixedWidth(620);
    scroll->setWidget(card);
    scroll->setAlignment(Qt::AlignHCenter);

    auto *mainLayout = new QVBoxLayout(card);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(40, 28, 40, 28);

    // ── Top bar: header + timer ──
    auto *topBar = new QHBoxLayout;
    topBar->setSpacing(16);

    // Status dot + header
    auto *statusDot = new QLabel;
    {
        QPixmap dotPx(10, 10);
        dotPx.fill(Qt::transparent);
        QPainter dp(&dotPx);
        dp.setRenderHint(QPainter::Antialiasing);
        dp.setPen(Qt::NoPen);
        dp.setBrush(QColor("#22c55e"));
        dp.drawEllipse(0, 0, 10, 10);
        dp.end();
        statusDot->setPixmap(dotPx);
    }
    statusDot->setFixedSize(10, 10);

    headerLabel = new QLabel("Exam in Progress");
    headerLabel->setObjectName("examHeader");

    // Clock icon + timer
    auto *clockIcon = new QLabel;
    {
        QPixmap clk(20, 20);
        clk.fill(Qt::transparent);
        QPainter cp(&clk);
        cp.setRenderHint(QPainter::Antialiasing);
        cp.setPen(QPen(QColor("#3b82f6"), 1.8));
        cp.setBrush(Qt::NoBrush);
        cp.drawEllipse(2, 2, 16, 16);
        cp.drawLine(10, 5, 10, 10);
        cp.drawLine(10, 10, 14, 12);
        cp.end();
        clockIcon->setPixmap(clk);
    }
    clockIcon->setFixedSize(20, 20);

    timerLabel = new QLabel("00:00");
    timerLabel->setObjectName("timerLabel");
    timerLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    topBar->addWidget(statusDot);
    topBar->addWidget(headerLabel);
    topBar->addStretch();
    topBar->addWidget(clockIcon);
    topBar->addWidget(timerLabel);
    mainLayout->addLayout(topBar);

    // Thin progress bar
    progressBar = new QProgressBar;
    progressBar->setTextVisible(false);
    progressBar->setFixedHeight(6);
    mainLayout->addWidget(progressBar);

    mainLayout->addSpacing(14);

    // ── Question counter ──
    questionCounter = new QLabel;
    questionCounter->setObjectName("questionCounter");
    mainLayout->addWidget(questionCounter);

    // ── Question text ──
    questionLabel = new QLabel;
    questionLabel->setObjectName("questionLabel");
    questionLabel->setWordWrap(true);
    mainLayout->addWidget(questionLabel);

    mainLayout->addSpacing(8);

    // ── Option radio buttons ──
    optionGroup = new QButtonGroup(this);
    for (int i = 0; i < 4; i++) {
        optionButtons[i] = new QRadioButton;
        optionButtons[i]->setObjectName("optionButton");
        optionButtons[i]->setCursor(Qt::PointingHandCursor);
        optionGroup->addButton(optionButtons[i], i);
        mainLayout->addWidget(optionButtons[i]);
    }

    mainLayout->addSpacing(20);

    // ── Separator ──
    auto *sep = new QFrame;
    sep->setFrameShape(QFrame::HLine);
    sep->setObjectName("separator");
    sep->setFixedHeight(1);
    mainLayout->addWidget(sep);

    mainLayout->addSpacing(8);

    // ── Next / Skip buttons ──
    auto *buttonBar = new QHBoxLayout;
    buttonBar->setSpacing(12);

    skipButton = new QPushButton("Skip");
    skipButton->setObjectName("skipButton");
    skipButton->setMinimumHeight(44);
    skipButton->setCursor(Qt::PointingHandCursor);

    nextButton = new QPushButton("Next  →");
    nextButton->setObjectName("nextButton");
    nextButton->setMinimumHeight(44);
    nextButton->setCursor(Qt::PointingHandCursor);

    buttonBar->addWidget(skipButton);
    buttonBar->addStretch();
    buttonBar->addWidget(nextButton);
    mainLayout->addLayout(buttonBar);

    mainLayout->addStretch();

    connect(nextButton, &QPushButton::clicked, this, &ExamWidget::onNextClicked);
    connect(skipButton, &QPushButton::clicked, this, &ExamWidget::onSkipClicked);

    // Timer
    examTimer_ = new QTimer(this);
    connect(examTimer_, &QTimer::timeout, this, &ExamWidget::onTimerTick);
}

void ExamWidget::startExam(const ExamConfig &config,
                           const std::vector<Question> &questions)
{
    config_    = config;
    questions_ = questions;
    answers_.clear();
    currentIndex_ = 0;

    progressBar->setRange(1, (int)questions_.size());
    remainingSeconds_ = config_.examTimeMinutes * 60;

    examTimer_->start(1000);
    showQuestion(0);
}

void ExamWidget::showQuestion(int index)
{
    if (index >= (int)questions_.size()) {
        finishExam();
        return;
    }

    currentIndex_ = index;

    // Shuffle options for this question
    questions_[index].shuffleOptions();

    auto &q = questions_[index];
    questionCounter->setText(
        QString("Question %1 of %2")
            .arg(index + 1).arg(questions_.size()));

    questionLabel->setText(QString::fromStdString(q.getQuestionText()));

    auto opts = q.getOptions();
    for (int i = 0; i < 4; i++) {
        if (i < (int)opts.size()) {
            optionButtons[i]->setText(
                QString("%1)  %2").arg(QChar('A' + i))
                    .arg(QString::fromStdString(opts[i])));
            optionButtons[i]->setVisible(true);
        } else {
            optionButtons[i]->setVisible(false);
        }
    }

    // Clear selection
    optionGroup->setExclusive(false);
    for (int i = 0; i < 4; i++)
        optionButtons[i]->setChecked(false);
    optionGroup->setExclusive(true);

    progressBar->setValue(index + 1);

    // Update button text on last question
    if (index == (int)questions_.size() - 1)
        nextButton->setText("Finish  ✓");
    else
        nextButton->setText("Next  →");
}

void ExamWidget::onNextClicked()
{
    int selected = optionGroup->checkedId();
    if (selected < 0) {
        QMessageBox::information(this, "Select Answer",
            "Please select an option or click Skip.");
        return;
    }

    auto &q = questions_[currentIndex_];
    bool correct = (selected == q.getCorrectIndex());

    answers_.push_back({
        currentIndex_ + 1,
        q.getQuestionText(),
        selected,
        q.getCorrectIndex(),
        correct
    });

    showQuestion(currentIndex_ + 1);
}

void ExamWidget::onSkipClicked()
{
    auto &q = questions_[currentIndex_];
    answers_.push_back({
        currentIndex_ + 1,
        q.getQuestionText(),
        -1,
        q.getCorrectIndex(),
        false
    });

    showQuestion(currentIndex_ + 1);
}

void ExamWidget::onTimerTick()
{
    remainingSeconds_--;

    int min = remainingSeconds_ / 60;
    int sec = remainingSeconds_ % 60;

    timerLabel->setText(
        QString("%1:%2")
            .arg(min, 2, 10, QChar('0'))
            .arg(sec, 2, 10, QChar('0')));

    // Warning colors
    if (remainingSeconds_ <= 10)
        timerLabel->setStyleSheet("color: #dc2626; font-weight: 700; font-size: 22px;");
    else if (remainingSeconds_ <= 60)
        timerLabel->setStyleSheet("color: #d97706; font-weight: 700; font-size: 22px;");
    else
        timerLabel->setStyleSheet("");

    if (remainingSeconds_ <= 0) {
        examTimer_->stop();
        QMessageBox::warning(this, "Time's Up",
            "Time has expired.\nYour answers so far will be submitted.");
        finishExam();
    }
}

void ExamWidget::finishExam()
{
    examTimer_->stop();
    emit examFinished(answers_, config_, (int)questions_.size());
}
