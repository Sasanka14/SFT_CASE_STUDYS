# Online Examination System

A desktop MCQ examination platform built with **C++17** and **Qt 6**, featuring AI-powered question generation via Google Gemini and OpenAI APIs.

![C++17](https://img.shields.io/badge/C%2B%2B-17-blue?logo=cplusplus)
![Qt 6](https://img.shields.io/badge/Qt-6-41cd52?logo=qt)
![CMake](https://img.shields.io/badge/CMake-3.16+-064f8c?logo=cmake)
![Platform](https://img.shields.io/badge/Platform-macOS-lightgrey?logo=apple)
![License](https://img.shields.io/badge/License-MIT-green)

---

## Features

- **AI Question Generation** — Generate topic-specific questions using Google Gemini (free tier) with OpenAI fallback
- **Desktop GUI** — Modern Qt 6 interface with 3-screen flow (Setup → Exam → Results)
- **Console Mode** — Full-featured terminal interface with interactive menu
- **Timed Exams** — Configurable countdown timer with color-coded warnings and auto-submit
- **Instant Results** — Pass/fail evaluation, score breakdown, and per-question review table
- **Configurable** — Source, topic, difficulty, question count, duration, and pass threshold
- **Question Deduplication** — Prevents repeated questions from the local question bank
- **Option Shuffling** — Randomizes answer order per question

---

## Screenshots

### Setup Screen
> Configure student info, question source, topic, difficulty, and exam settings

### Exam Screen
> Timed exam with radio button options, progress bar, and skip support

### Results Screen
> Pass/fail badge, score summary, and color-coded answer review table

---

## Architecture

```
┌───────────────────────────────────────────────┐
│              Presentation Layer               │
│   Console (main.cpp)  │  Qt GUI (gui/*.cpp)   │
├───────────────────────┴───────────────────────┤
│               Core Engine Layer               │
│  Question · QuestionBank · ExamConfig         │
│  Student · ResultAnalyzer · TimerManager      │
├───────────────────────────────────────────────┤
│              API / Service Layer              │
│  APIQuestionGenerator · EnvLoader             │
│  Gemini Client · OpenAI Client                │
├───────────────────────────────────────────────┤
│                 Data Layer                    │
│  questions.txt (pipe-delimited) · .env        │
└───────────────────────────────────────────────┘
```

---

## Tech Stack

| Component | Technology |
|-----------|-----------|
| Language | C++17 |
| Build System | CMake 3.16+ |
| GUI | Qt 6 Widgets |
| HTTP | libcurl |
| AI (Primary) | Google Gemini 2.0 Flash |
| AI (Fallback) | OpenAI GPT-3.5-Turbo |
| Threading | std::thread / std::atomic |

---

## Prerequisites

- **macOS** 12.0+ (Apple Silicon or Intel)
- **Homebrew**

```bash
brew install qt cmake
```

> `libcurl` is included with macOS.

---

## Build

```bash
git clone https://github.com/Sasanka14/OnlineExamSimulation-Basic-.git
cd OnlineExamSimulation-Basic-
mkdir -p build && cd build

# Configure
cmake .. -DCMAKE_PREFIX_PATH=/opt/homebrew/opt/qt

# Build GUI version
cmake --build . --target OnlineExamGUI -j10

# Build Console version
cmake --build . --target OnlineExamSystem -j10
```

---

## Setup

Create a `.env` file in the project root:

```env
GEMINI_API_KEY=your_gemini_key
OPENAI_API_KEY=your_openai_key
OPENAI_MODEL=gpt-3.5-turbo
OPENAI_TIMEOUT=30
```

> API keys are **optional** — select "Local File" as the question source to use the bundled question bank.

Get a free Gemini API key at [aistudio.google.com](https://aistudio.google.com/apikey).

---

## Usage

### Desktop GUI

```bash
cd build
./OnlineExamGUI
```

1. Fill in student name and ID
2. Select question source (Auto / Gemini / OpenAI / Local File)
3. Set topic, number of questions, difficulty, duration, and pass threshold
4. Click **Start Exam**
5. Answer questions or skip — timer auto-submits when it expires
6. Review results and start a new exam or quit

### Console

```bash
cd build
./OnlineExamSystem
```

Navigate the 9-option interactive menu to configure and start the exam.

---

## Project Structure

```
onlineExam/
├── CMakeLists.txt              # Build configuration (2 targets)
├── main.cpp                    # Console entry point (menu system)
├── .env                        # API keys (gitignored)
│
├── include/                    # Header files
│   ├── APIQuestionGenerator.h  # AI question generation
│   ├── ConsoleUI.h             # Terminal formatting
│   ├── EnvLoader.h             # .env file parser
│   ├── ExamConfig.h            # Configuration struct + enums
│   ├── Question.h              # MCQ data model
│   ├── QuestionBank.h          # File loader + dedup
│   ├── ResultAnalyzer.h        # Score calculator
│   ├── Student.h               # Student data class
│   └── TimerManager.h          # Async countdown timer
│
├── src/                        # Source implementations
│   ├── APIQuestionGenerator.cpp
│   ├── ConsoleUI.cpp
│   ├── EnvLoader.cpp
│   ├── Question.cpp
│   ├── QuestionBank.cpp
│   ├── ResultAnalyzer.cpp
│   ├── Student.cpp
│   └── TimerManager.cpp
│
├── gui/                        # Qt 6 desktop GUI
│   ├── guimain.cpp             # Entry point + stylesheet
│   ├── MainWindow.h/.cpp       # Screen manager (QStackedWidget)
│   ├── SetupWidget.h/.cpp      # Configuration form
│   ├── ExamWidget.h/.cpp       # Live exam screen
│   └── ResultWidget.h/.cpp     # Results + review
│
└── data/
    └── questions.txt           # Local question bank
```

---

## Question File Format

Questions in `data/questions.txt` use pipe-delimited format:

```
question_text|option1|option2|option3|option4|correct_index|difficulty|topic
```

**Example:**
```
What is the capital of France?|London|Paris|Berlin|Madrid|1|easy|Geography
```

`correct_index` is zero-based.

---

## How AI Generation Works

1. User selects a topic and difficulty
2. A structured prompt is sent to the Gemini API requesting JSON output
3. Response is parsed into `Question` objects
4. If Gemini fails → OpenAI is tried as fallback
5. If both fail → local file is loaded with a notification

The prompt requests a JSON array with `question`, `options` (4 strings), and `correct_answer` (0-based index).

---

## License

MIT
