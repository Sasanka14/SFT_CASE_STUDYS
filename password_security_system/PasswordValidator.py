import re

class PasswordValidator:
    def __init__(self, password: str):
        self.password = password

    def has_min_length(self):
        return len(self.password) >= 8

    def has_uppercase(self):
        return bool(re.search(r"[A-Z]", self.password))

    def has_lowercase(self):
        return bool(re.search(r"[a-z]", self.password))

    def has_digit(self):
        return bool(re.search(r"[0-9]", self.password))

    def has_symbol(self):
        return bool(re.search(r"[^\w]", self.password))

    def validate(self):
        checks = {
            "Minimum Length (8+)": self.has_min_length(),
            "Uppercase Letter": self.has_uppercase(),
            "Lowercase Letter": self.has_lowercase(),
            "Digit": self.has_digit(),
            "Special Character": self.has_symbol()
        }
        return checks

    def strength_score(self):
        score = 0

        if self.has_min_length():
            score += 20
        if self.has_uppercase():
            score += 20
        if self.has_lowercase():
            score += 20
        if self.has_digit():
            score += 20
        if self.has_symbol():
            score += 20

        return score

    def strength_label(self):
        score = self.strength_score()

        if score < 40:
            return "Weak"
        elif score < 70:
            return "Moderate"
        elif score < 90:
            return "Strong"
        else:
            return "Very Strong"
