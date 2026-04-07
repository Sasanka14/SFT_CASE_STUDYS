# security_manager.py

import csv
from datetime import datetime


class SecurityManager:
    def __init__(
        self,
        history_file="validation_history.csv",
        audit_file="security_audit.log"
    ):
        self.history_file = history_file
        self.audit_file = audit_file

    # ---- save validation history ----
    def save_validation_history(self, password, score, label):
        file_exists = False
        try:
            with open(self.history_file, "r"):
                file_exists = True
        except FileNotFoundError:
            pass

        with open(self.history_file, "a", newline="") as file:
            writer = csv.writer(file)

            if not file_exists:
                writer.writerow([
                    "timestamp",
                    "password",
                    "strength_score",
                    "strength_label"
                ])

            writer.writerow([
                datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
                password,
                score,
                label
            ])

    # ---- security audit log ----
    def log_security_event(self, message):
        with open(self.audit_file, "a") as file:
            file.write(
                f"[{datetime.now().strftime('%Y-%m-%d %H:%M:%S')}] {message}\n"
            )
