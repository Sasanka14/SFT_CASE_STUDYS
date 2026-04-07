#include "SetupWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QFont>
#include <QFrame>
#include <QScrollArea>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QStyle>
#include <QApplication>

static QLabel *makeFieldLabel(const QString &text)
{
    auto *lbl = new QLabel(text);
    lbl->setObjectName("fieldLabel");
    lbl->setMinimumWidth(100);
    lbl->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    return lbl;
}

static QFrame *makeSeparator()
{
    auto *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setObjectName("separator");
    line->setFixedHeight(1);
    return line;
}

static QPixmap makeSectionIcon(const QColor &bg, const QString &letter)
{
    QPixmap px(22, 22);
    px.fill(Qt::transparent);
    QPainter p(&px);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(bg);
    p.drawRoundedRect(1, 1, 20, 20, 6, 6);
    p.setPen(Qt::white);
    p.setFont(QFont("Arial", 11, QFont::Bold));
    p.drawText(QRect(1, 1, 20, 20), Qt::AlignCenter, letter);
    p.end();
    return px;
}

static QWidget *makeSectionHeader(const QString &text, const QColor &iconColor,
                                  const QString &iconLetter)
{
    auto *row = new QWidget;
    auto *lay = new QHBoxLayout(row);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(8);

    auto *icon = new QLabel;
    icon->setPixmap(makeSectionIcon(iconColor, iconLetter));
    icon->setFixedSize(22, 22);
    lay->addWidget(icon);

    auto *lbl = new QLabel(text);
    lbl->setObjectName("sectionTitle");
    lay->addWidget(lbl);
    lay->addStretch();
    return row;
}

static void addCardShadow(QWidget *w)
{
    auto *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(18);
    shadow->setOffset(0, 3);
    shadow->setColor(QColor(0, 0, 0, 30));
    w->setGraphicsEffect(shadow);
}

SetupWidget::SetupWidget(QWidget *parent) : QWidget(parent)
{
    // Outer layout centres a fixed-width card
    auto *outerLayout = new QVBoxLayout(this);
    outerLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    outerLayout->setContentsMargins(20, 20, 20, 20);

    // Scrollable wrapper so it works on small screens
    auto *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setObjectName("setupScroll");
    outerLayout->addWidget(scroll);

    auto *card = new QWidget;
    card->setObjectName("setupCard");
    card->setFixedWidth(560);
    scroll->setWidget(card);
    scroll->setAlignment(Qt::AlignHCenter);

    auto *mainLayout = new QVBoxLayout(card);
    mainLayout->setSpacing(6);
    mainLayout->setContentsMargins(44, 36, 44, 36);

    // ── Header icon (painted, no emoji) ──
    auto *headerIcon = new QLabel;
    headerIcon->setFixedSize(52, 52);
    headerIcon->setAlignment(Qt::AlignCenter);
    {
        QPixmap pix(52, 52);
        pix.fill(Qt::transparent);
        QPainter p(&pix);
        p.setRenderHint(QPainter::Antialiasing);
        // Circle background
        p.setPen(Qt::NoPen);
        p.setBrush(QColor("#3b82f6"));
        p.drawRoundedRect(4, 4, 44, 44, 14, 14);
        // Pen icon lines
        p.setPen(QPen(Qt::white, 2.5, Qt::SolidLine, Qt::RoundCap));
        p.drawLine(18, 32, 32, 18);
        p.drawLine(32, 18, 35, 15);
        p.drawLine(18, 32, 15, 35);
        p.drawLine(15, 35, 35, 15);
        // Paper lines
        p.setPen(QPen(QColor("#93c5fd"), 1.5));
        p.drawLine(20, 22, 30, 22);
        p.drawLine(20, 26, 28, 26);
        p.end();
        headerIcon->setPixmap(pix);
    }
    mainLayout->addWidget(headerIcon, 0, Qt::AlignCenter);

    auto *title = new QLabel("Online Examination System");
    title->setObjectName("titleLabel");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    auto *subtitle = new QLabel("Configure your exam settings below");
    subtitle->setObjectName("subtitleLabel");
    subtitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(subtitle);

    mainLayout->addSpacing(18);

    // ── Section 1 · Student Information ──
    mainLayout->addWidget(makeSectionHeader("Student Information",
                                            QColor("#3b82f6"), "S"));
    mainLayout->addWidget(makeSeparator());
    mainLayout->addSpacing(8);

    auto *studentForm = new QFormLayout;
    studentForm->setSpacing(14);
    studentForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    studentForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    nameEdit = new QLineEdit;
    nameEdit->setPlaceholderText("Enter your full name");
    nameEdit->setMinimumHeight(38);

    idEdit = new QLineEdit;
    idEdit->setPlaceholderText("e.g. ITM001");
    idEdit->setMinimumHeight(38);

    studentForm->addRow(makeFieldLabel("Name"), nameEdit);
    studentForm->addRow(makeFieldLabel("Student ID"), idEdit);
    mainLayout->addLayout(studentForm);

    mainLayout->addSpacing(22);

    // ── Section 2 · Question Settings ──
    mainLayout->addWidget(makeSectionHeader("Question Settings",
                                            QColor("#8b5cf6"), "Q"));
    mainLayout->addWidget(makeSeparator());
    mainLayout->addSpacing(8);

    auto *questionForm = new QFormLayout;
    questionForm->setSpacing(14);
    questionForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    questionForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    sourceCombo = new QComboBox;
    sourceCombo->setMinimumHeight(38);
    sourceCombo->addItem(qApp->style()->standardIcon(QStyle::SP_BrowserReload),
                         "Auto (Gemini \u2192 OpenAI \u2192 File)");
    sourceCombo->addItem(qApp->style()->standardIcon(QStyle::SP_CommandLink),
                         "Gemini AI");
    sourceCombo->addItem(qApp->style()->standardIcon(QStyle::SP_ComputerIcon),
                         "OpenAI");
    sourceCombo->addItem(qApp->style()->standardIcon(QStyle::SP_DirIcon),
                         "Local File");

    topicLabel = new QLabel("Topic");
    topicLabel->setObjectName("fieldLabel");
    topicLabel->setMinimumWidth(100);
    topicLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    topicEdit = new QLineEdit;
    topicEdit->setPlaceholderText("e.g. Java, Python, Data Structures...");
    topicEdit->setMinimumHeight(38);

    numQuestionsSpin = new QSpinBox;
    numQuestionsSpin->setRange(1, 500);
    numQuestionsSpin->setValue(10);
    numQuestionsSpin->setMinimumHeight(38);

    difficultyCombo = new QComboBox;
    difficultyCombo->setMinimumHeight(38);
    // Colored dot icons for difficulty
    auto makeDot = [](const QColor &c) {
        QPixmap px(14, 14);
        px.fill(Qt::transparent);
        QPainter pt(&px);
        pt.setRenderHint(QPainter::Antialiasing);
        pt.setPen(Qt::NoPen);
        pt.setBrush(c);
        pt.drawEllipse(1, 1, 12, 12);
        pt.end();
        return QIcon(px);
    };
    difficultyCombo->addItem(makeDot(QColor("#22c55e")), "Easy");
    difficultyCombo->addItem(makeDot(QColor("#eab308")), "Medium");
    difficultyCombo->addItem(makeDot(QColor("#ef4444")), "Hard");
    difficultyCombo->addItem(makeDot(QColor("#8b5cf6")), "Mixed");
    difficultyCombo->setCurrentIndex(1);

    questionForm->addRow(makeFieldLabel("Source"), sourceCombo);
    questionForm->addRow(topicLabel, topicEdit);
    questionForm->addRow(makeFieldLabel("No. of Questions"), numQuestionsSpin);
    questionForm->addRow(makeFieldLabel("Difficulty"), difficultyCombo);
    mainLayout->addLayout(questionForm);

    connect(sourceCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SetupWidget::onSourceChanged);

    mainLayout->addSpacing(22);

    // ── Section 3 · Exam Settings ──
    mainLayout->addWidget(makeSectionHeader("Exam Settings",
                                            QColor("#059669"), "E"));
    mainLayout->addWidget(makeSeparator());
    mainLayout->addSpacing(8);

    auto *examForm = new QFormLayout;
    examForm->setSpacing(14);
    examForm->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    examForm->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    timeSpin = new QSpinBox;
    timeSpin->setRange(1, 180);
    timeSpin->setValue(5);
    timeSpin->setSuffix("  min");
    timeSpin->setMinimumHeight(38);

    passingPercentSpin = new QDoubleSpinBox;
    passingPercentSpin->setRange(1.0, 100.0);
    passingPercentSpin->setValue(60.0);
    passingPercentSpin->setSuffix("  %");
    passingPercentSpin->setMinimumHeight(38);

    examForm->addRow(makeFieldLabel("Duration"), timeSpin);
    examForm->addRow(makeFieldLabel("Pass Threshold"), passingPercentSpin);
    mainLayout->addLayout(examForm);

    mainLayout->addSpacing(28);

    // ── Start Button ──
    startButton = new QPushButton("Start Exam  →");
    startButton->setObjectName("startButton");
    startButton->setMinimumHeight(52);
    startButton->setCursor(Qt::PointingHandCursor);
    mainLayout->addWidget(startButton);

    mainLayout->addStretch();

    connect(startButton, &QPushButton::clicked,
            this, &SetupWidget::onStartClicked);
}

void SetupWidget::onSourceChanged(int index)
{
    // Hide topic field when source is "Local File" (index 3)
    bool showTopic = (index != 3);
    topicLabel->setVisible(showTopic);
    topicEdit->setVisible(showTopic);
}

void SetupWidget::onStartClicked()
{
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Missing Info", "Please enter your name.");
        nameEdit->setFocus();
        return;
    }
    if (idEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Missing Info", "Please enter your student ID.");
        idEdit->setFocus();
        return;
    }

    int srcIdx = sourceCombo->currentIndex();
    if (srcIdx != 3 && topicEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Missing Info",
            "Please enter a topic for AI-generated questions,\n"
            "or switch to 'Local File' source.");
        topicEdit->setFocus();
        return;
    }

    emit startExamRequested(getConfig());
}

ExamConfig SetupWidget::getConfig() const
{
    ExamConfig cfg;
    cfg.studentName    = nameEdit->text().trimmed().toStdString();
    cfg.studentId      = idEdit->text().trimmed().toStdString();
    cfg.topic          = topicEdit->text().trimmed().toStdString();
    cfg.numQuestions   = numQuestionsSpin->value();
    cfg.examTimeMinutes = timeSpin->value();
    cfg.passingPercent = passingPercentSpin->value();

    switch (sourceCombo->currentIndex()) {
        case 0: cfg.source = QuestionSource::AUTO;   break;
        case 1: cfg.source = QuestionSource::GEMINI; break;
        case 2: cfg.source = QuestionSource::OPENAI; break;
        case 3: cfg.source = QuestionSource::FILE;   break;
    }

    switch (difficultyCombo->currentIndex()) {
        case 0: cfg.difficulty = Difficulty::EASY;   break;
        case 1: cfg.difficulty = Difficulty::MEDIUM; break;
        case 2: cfg.difficulty = Difficulty::HARD;   break;
        case 3: cfg.difficulty = Difficulty::MIXED;  break;
    }

    return cfg;
}
