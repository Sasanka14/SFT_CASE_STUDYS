#include <QApplication>
#include <QFile>
#include "MainWindow.h"
#include "EnvLoader.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Online Examination System");

    // Load .env file
    EnvLoader::load("../.env");

    // ── Apply stylesheet ──
    QString style = R"(

        /* ═══════════════════════════════════════════
           GLOBAL
           ═══════════════════════════════════════════ */
        * {
            font-family: -apple-system, "SF Pro Text", "Helvetica Neue", Arial, sans-serif;
        }

        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                        stop:0 #e8edf5, stop:0.5 #e2e8f0, stop:1 #dbe2ef);
        }

        /* ─── Scroll area transparent ─── */
        #setupScroll {
            background: transparent;
        }
        #setupScroll > QWidget {
            background: transparent;
        }

        /* ─── Card container ─── */
        #setupCard {
            background: #ffffff;
            border-radius: 16px;
            border: 1px solid rgba(0, 0, 0, 0.06);
        }

        /* ═══════════════════════════════════════════
           TYPOGRAPHY
           ═══════════════════════════════════════════ */
        #titleLabel {
            font-size: 24px;
            font-weight: 700;
            color: #1a202c;
            padding: 0;
            margin: 0;
            letter-spacing: -0.5px;
        }

        #subtitleLabel {
            font-size: 13px;
            color: #94a3b8;
            padding: 0 0 4px 0;
            letter-spacing: 0.3px;
        }

        #sectionTitle {
            font-size: 13px;
            font-weight: 700;
            color: #64748b;
            text-transform: uppercase;
            letter-spacing: 1.2px;
            padding: 0;
        }

        #fieldLabel {
            font-size: 13px;
            font-weight: 600;
            color: #475569;
            padding-right: 8px;
        }

        /* ─── Separator line ─── */
        #separator {
            background: #e2e8f0;
            border: none;
            max-height: 1px;
        }

        /* ═══════════════════════════════════════════
           FORM INPUTS
           ═══════════════════════════════════════════ */
        QLineEdit, QSpinBox, QDoubleSpinBox {
            padding: 8px 14px;
            border: 1.5px solid #e2e8f0;
            border-radius: 10px;
            background: #f8fafc;
            font-size: 14px;
            color: #1e293b;
            selection-background-color: #3b82f6;
        }
        QLineEdit:hover, QSpinBox:hover, QDoubleSpinBox:hover {
            border-color: #cbd5e1;
            background: #ffffff;
        }
        QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: #3b82f6;
            background: #ffffff;
            outline: none;
        }
        QLineEdit::placeholder {
            color: #94a3b8;
        }

        QComboBox {
            padding: 8px 14px;
            border: 1.5px solid #e2e8f0;
            border-radius: 10px;
            background: #f8fafc;
            font-size: 14px;
            color: #1e293b;
            min-height: 20px;
        }
        QComboBox:hover {
            border-color: #cbd5e1;
            background: #ffffff;
        }
        QComboBox:focus {
            border-color: #3b82f6;
            background: #ffffff;
        }
        QComboBox::drop-down {
            border: none;
            width: 30px;
        }
        QComboBox::down-arrow {
            image: none;
            border-left: 5px solid transparent;
            border-right: 5px solid transparent;
            border-top: 6px solid #64748b;
            margin-right: 10px;
        }
        QComboBox QAbstractItemView {
            border: 1px solid #e2e8f0;
            border-radius: 8px;
            background: #ffffff;
            selection-background-color: #eff6ff;
            selection-color: #1e40af;
            padding: 4px;
            outline: none;
        }

        /* ═══════════════════════════════════════════
           BUTTONS
           ═══════════════════════════════════════════ */
        #startButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #3b82f6, stop:1 #2563eb);
            color: #ffffff;
            font-size: 16px;
            font-weight: 700;
            border: none;
            border-radius: 12px;
            padding: 14px;
            letter-spacing: 0.5px;
        }
        #startButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #2563eb, stop:1 #1d4ed8);
        }
        #startButton:pressed {
            background: #1e40af;
        }

        #nextButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #10b981, stop:1 #059669);
            color: #ffffff;
            font-size: 15px;
            font-weight: 700;
            border: none;
            border-radius: 10px;
            padding: 12px 36px;
        }
        #nextButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #059669, stop:1 #047857);
        }

        #skipButton {
            background: transparent;
            color: #64748b;
            font-size: 14px;
            font-weight: 600;
            border: 1.5px solid #cbd5e1;
            border-radius: 10px;
            padding: 12px 36px;
        }
        #skipButton:hover {
            background: #f1f5f9;
            border-color: #94a3b8;
            color: #475569;
        }

        /* ═══════════════════════════════════════════
           EXAM SCREEN
           ═══════════════════════════════════════════ */
        #examHeader {
            font-size: 20px;
            font-weight: 700;
            color: #1e293b;
        }

        #timerLabel {
            font-size: 22px;
            font-weight: 700;
            color: #3b82f6;
            font-variant-numeric: tabular-nums;
        }

        #questionCounter {
            font-size: 12px;
            font-weight: 600;
            color: #94a3b8;
            text-transform: uppercase;
            letter-spacing: 1px;
        }

        #questionLabel {
            font-size: 17px;
            font-weight: 600;
            color: #1e293b;
            padding: 12px 0;
            line-height: 1.5;
        }

        /* ─── Radio option cards ─── */
        QRadioButton {
            font-size: 14px;
            padding: 12px 16px;
            border: 1.5px solid #e2e8f0;
            border-radius: 10px;
            background: #ffffff;
            margin: 3px 0;
            color: #334155;
        }
        QRadioButton:hover {
            background: #f0f9ff;
            border-color: #93c5fd;
        }
        QRadioButton:checked {
            background: #eff6ff;
            border-color: #3b82f6;
            color: #1e40af;
            font-weight: 600;
        }
        QRadioButton::indicator {
            width: 18px;
            height: 18px;
            border-radius: 9px;
            border: 2px solid #cbd5e1;
            background: #ffffff;
            margin-right: 10px;
        }
        QRadioButton::indicator:checked {
            background: #3b82f6;
            border-color: #3b82f6;
        }

        /* ─── Progress Bar ─── */
        QProgressBar {
            height: 8px;
            border: none;
            border-radius: 4px;
            background: #e2e8f0;
            text-align: center;
            font-size: 0px;
        }
        QProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                        stop:0 #3b82f6, stop:1 #8b5cf6);
            border-radius: 4px;
        }

        /* ═══════════════════════════════════════════
           RESULT SCREEN
           ═══════════════════════════════════════════ */
        #scoreLabel, #percentLabel {
            font-size: 20px;
            font-weight: 700;
            color: #1e293b;
            padding: 4px 12px;
        }

        #statsLabel {
            font-size: 13px;
            color: #64748b;
            padding: 4px;
        }

        /* ─── Table ─── */
        QTableWidget {
            border: 1px solid #e2e8f0;
            border-radius: 10px;
            background: #ffffff;
            gridline-color: #f1f5f9;
            font-size: 13px;
            color: #334155;
            outline: none;
        }
        QTableWidget::item {
            padding: 8px 10px;
            border-bottom: 1px solid #f1f5f9;
        }
        QTableWidget::item:selected {
            background: #eff6ff;
            color: #1e293b;
        }
        QHeaderView::section {
            background: #f8fafc;
            font-weight: 700;
            font-size: 12px;
            color: #64748b;
            text-transform: uppercase;
            letter-spacing: 0.5px;
            padding: 10px 8px;
            border: none;
            border-bottom: 2px solid #e2e8f0;
        }

        /* ─── Message Boxes ─── */
        QMessageBox {
            background: #ffffff;
        }
        QMessageBox QLabel {
            color: #1e293b;
            font-size: 14px;
        }
        QMessageBox QPushButton {
            padding: 8px 24px;
            border-radius: 8px;
            background: #3b82f6;
            color: white;
            font-weight: 600;
            border: none;
            min-width: 80px;
        }
        QMessageBox QPushButton:hover {
            background: #2563eb;
        }
    )";

    app.setStyleSheet(style);

    MainWindow window;
    window.show();

    return app.exec();
}
