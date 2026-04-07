# SecurityFeatures.py

import csv
from PasswordValidator import PasswordValidator


class SecurityFeatures:
    def __init__(self, password, weak_password_file="weak_passwords.csv"):
        self.password = password
        self.validator = PasswordValidator(password)
        self.weak_password_file = weak_password_file

    #  weak password detection 
    def is_common_weak_password(self):
        try:
            with open(self.weak_password_file, newline="") as file:
                reader = csv.reader(file)
                next(reader, None)  # skip header
                for row in reader:
                    if row and row[0] == self.password:
                        return True
        except FileNotFoundError:
            pass
        return False

    #  strength indicator (improved model) 
    def strength_indicator(self):
        base_score = 0

        if self.validator.has_min_length():
            base_score += 15
        if self.validator.has_uppercase():
            base_score += 15
        if self.validator.has_lowercase():
            base_score += 15
        if self.validator.has_digit():
            base_score += 15

        # length bonus
        length = len(self.password)
        if length >= 16:
            length_bonus = 25
        elif length >= 12:
            length_bonus = 18
        elif length >= 10:
            length_bonus = 12
        else:
            length_bonus = 5

        # diversity bonus
        diversity = sum([
            self.validator.has_uppercase(),
            self.validator.has_lowercase(),
            self.validator.has_digit(),
            self.validator.has_symbol()
        ])
        diversity_bonus = diversity * 3
        if diversity == 4:
            diversity_bonus += 3

        score = base_score + length_bonus + diversity_bonus
        score = min(score, 100)

        if self.is_common_weak_password():
            return 20, "Weak (Common Password Detected)"

        if score < 40:
            label = "Weak"
        elif score < 70:
            label = "Moderate"
        elif score < 90:
            label = "Strong"
        else:
            label = "Very Strong"

        return score, label

    #  improvement suggestions 
    def improvement_suggestions(self):
        suggestions = []

        if not self.validator.has_uppercase():
            suggestions.append("Add at least one uppercase letter.")
        if not self.validator.has_lowercase():
            suggestions.append("Add at least one lowercase letter.")
        if not self.validator.has_digit():
            suggestions.append("Include at least one numeric digit.")
        if not self.validator.has_symbol():
            suggestions.append("Include at least one special character.")
        if not self.validator.has_min_length():
            suggestions.append("Increase password length to at least 8 characters.")
        if self.is_common_weak_password():
            suggestions.append("Avoid using common or predictable passwords.")

        return suggestions
