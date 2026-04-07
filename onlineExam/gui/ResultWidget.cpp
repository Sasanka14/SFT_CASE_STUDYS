#include "ResultWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QApplication>
#include <QFrame>
#include <QScrollArea>
#include <QPainter>

ResultWidget::ResultWidget(QWidget *parent) : QWidget(parent)
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
    card->setFixedWidth(660);
    scroll->setWidget(card);
    scroll->setAlignment(Qt::AlignHCenter);

    auto *mainLayout = new QVBoxLayout(card);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(40, 36, 40, 36);

    // Header
    // Header icon
    auto *headerIcon = new QLabel;
    headerIcon->setFixedSize(44, 44);
    {
        QPixmap pix(44, 44);
        pix.fill(Qt::transparent);
        QPainter p(&pix);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor("#6366f1"));
        p.drawRoundedRect(2, 2, 40, 40, 12, 12);
        // Chart bars
        p.setBrush(Qt::white);
        p.drawRect(10, 24, 6, 12);
        p.drawRect(19, 18, 6, 18);
        p.drawRect(28, 12, 6, 24);
        p.end();
        headerIcon->setPixmap(pix);
    }
    mainLayout->addWidget(headerIcon, 0, Qt::AlignCenter);

    titleLabel = new QLabel("Exam Results");
    titleLabel->setObjectName("titleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    studentLabel = new QLabel;
    studentLabel->setObjectName("subtitleLabel");
    studentLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(studentLabel);

    mainLayout->addSpacing(16);

    // Badge (PASS / FAIL)
    badgeLabel = new QLabel;
    badgeLabel->setObjectName("badgeLabel");
    badgeLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(badgeLabel);

    mainLayout->addSpacing(10);

    // Score + Percent side by side
    auto *scoreRow = new QHBoxLayout;
    scoreRow->setSpacing(20);
    scoreLabel   = new QLabel;
    scoreLabel->setObjectName("scoreLabel");
    scoreLabel->setAlignment(Qt::AlignCenter);
    percentLabel = new QLabel;
    percentLabel->setObjectName("percentLabel");
    percentLabel->setAlignment(Qt::AlignCenter);
    scoreRow->addStretch();
    scoreRow->addWidget(scoreLabel);
    scoreRow->addWidget(percentLabel);
    scoreRow->addStretch();
    mainLayout->addLayout(scoreRow);

    // Stats line
    statsLabel = new QLabel;
    statsLabel->setObjectName("statsLabel");
    statsLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statsLabel);

    mainLayout->addSpacing(14);

    // Separator
    auto *sep = new QFrame;
    sep->setFrameShape(QFrame::HLine);
    sep->setObjectName("separator");
    sep->setFixedHeight(1);
    mainLayout->addWidget(sep);

    // Review section header with icon
    auto *reviewRow = new QWidget;
    auto *reviewLay = new QHBoxLayout(reviewRow);
    reviewLay->setContentsMargins(0, 6, 0, 0);
    reviewLay->setSpacing(8);
    auto *reviewIcon = new QLabel;
    {
        QPixmap px(20, 20);
        px.fill(Qt::transparent);
        QPainter p(&px);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor("#6366f1"));
        p.drawRoundedRect(1, 1, 18, 18, 5, 5);
        p.setPen(QPen(Qt::white, 1.5));
        p.drawLine(6, 7, 14, 7);
        p.drawLine(6, 10, 14, 10);
        p.drawLine(6, 13, 11, 13);
        p.end();
        reviewIcon->setPixmap(px);
    }
    reviewIcon->setFixedSize(20, 20);
    reviewLay->addWidget(reviewIcon);
    auto *reviewTitle = new QLabel("Answer Review");
    reviewTitle->setObjectName("sectionTitle");
    reviewLay->addWidget(reviewTitle);
    reviewLay->addStretch();
    mainLayout->addWidget(reviewRow);

    mainLayout->addSpacing(6);

    // Review table
    reviewTable = new QTableWidget;
    reviewTable->setColumnCount(4);
    reviewTable->setHorizontalHeaderLabels({"#", "Question", "Your Answer", "Status"});
    reviewTable->horizontalHeader()->setStretchLastSection(true);
    reviewTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    reviewTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    reviewTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    reviewTable->setAlternatingRowColors(true);
    reviewTable->verticalHeader()->setVisible(false);
    reviewTable->setShowGrid(false);
    mainLayout->addWidget(reviewTable);

    mainLayout->addSpacing(12);

    // Buttons
    auto *buttonBar = new QHBoxLayout;
    buttonBar->setSpacing(12);

    restartButton = new QPushButton("New Exam");
    restartButton->setObjectName("startButton");
    restartButton->setMinimumHeight(48);
    restartButton->setCursor(Qt::PointingHandCursor);

    quitButton = new QPushButton("Quit");
    quitButton->setObjectName("skipButton");
    quitButton->setMinimumHeight(48);
    quitButton->setCursor(Qt::PointingHandCursor);

    buttonBar->addWidget(quitButton);
    buttonBar->addStretch();
    buttonBar->addWidget(restartButton);
    mainLayout->addLayout(buttonBar);

    connect(restartButton, &QPushButton::clicked,
            this, &ResultWidget::restartRequested);
    connect(quitButton, &QPushButton::clicked,
            qApp, &QApplication::quit);
}

void ResultWidget::showResults(const std::vector<AnswerRecord> &answers,
                               const ExamConfig &config,
                               int totalQuestions)
{
    studentLabel->setText(
        QString::fromStdString(config.studentName) + "  ·  " +
        QString::fromStdString(config.studentId));

    int correct = 0, wrong = 0, skipped = 0;
    for (auto &a : answers) {
        if (a.userChoice == -1) skipped++;
        else if (a.isCorrect) correct++;
        else wrong++;
    }

    double pct = totalQuestions > 0
        ? (correct * 100.0 / totalQuestions) : 0.0;
    bool passed = pct >= config.passingPercent;

    scoreLabel->setText(
        QString("Score: %1 / %2").arg(correct).arg(totalQuestions));
    percentLabel->setText(
        QString("%1%").arg(pct, 0, 'f', 1));

    if (passed) {
        badgeLabel->setText("PASSED");
        badgeLabel->setStyleSheet(
            "color: #059669; font-size: 32px; font-weight: 800; "
            "padding: 12px 32px; border: 2.5px solid #059669; border-radius: 12px; "
            "background: #ecfdf5; letter-spacing: 2px;");
    } else {
        badgeLabel->setText("FAILED");
        badgeLabel->setStyleSheet(
            "color: #dc2626; font-size: 32px; font-weight: 800; "
            "padding: 12px 32px; border: 2.5px solid #dc2626; border-radius: 12px; "
            "background: #fef2f2; letter-spacing: 2px;");
    }

    statsLabel->setText(
        QString("%1 correct  ·  %2 wrong  ·  %3 skipped  ·  Pass threshold: %4%")
            .arg(correct).arg(wrong).arg(skipped)
            .arg(config.passingPercent, 0, 'f', 0));

    // Fill review table
    reviewTable->setRowCount((int)answers.size());
    for (int i = 0; i < (int)answers.size(); i++) {
        auto &a = answers[i];

        auto *numItem = new QTableWidgetItem(QString::number(a.questionNum));
        numItem->setTextAlignment(Qt::AlignCenter);
        reviewTable->setItem(i, 0, numItem);

        auto qText = QString::fromStdString(a.questionText);
        if (qText.length() > 70) qText = qText.left(67) + "...";
        reviewTable->setItem(i, 1, new QTableWidgetItem(qText));

        QString ansStr;
        if (a.userChoice == -1)
            ansStr = "—";
        else
            ansStr = QString(QChar('A' + a.userChoice));
        auto *ansItem = new QTableWidgetItem(ansStr);
        ansItem->setTextAlignment(Qt::AlignCenter);
        reviewTable->setItem(i, 2, ansItem);

        QString status;
        QColor color;
        if (a.userChoice == -1) {
            status = "Skipped";
            color = QColor("#d97706");
        } else if (a.isCorrect) {
            status = "Correct";
            color = QColor("#059669");
        } else {
            status = QString("Wrong (Ans: %1)").arg(QChar('A' + a.correctChoice));
            color = QColor("#dc2626");
        }

        auto *statusItem = new QTableWidgetItem(status);
        statusItem->setForeground(color);
        statusItem->setFont(QFont(statusItem->font().family(), -1, QFont::Bold));
        reviewTable->setItem(i, 3, statusItem);
    }

    reviewTable->resizeColumnsToContents();
    reviewTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    reviewTable->setColumnWidth(0, 40);
    reviewTable->setColumnWidth(2, 90);
}
