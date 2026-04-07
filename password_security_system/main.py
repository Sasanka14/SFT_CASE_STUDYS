# main.py

from PasswordGenerator import PasswordGenerator
from PasswordValidator import PasswordValidator
from SecurityFeatures import SecurityFeatures
from SecurityManager import SecurityManager

length = None

while True:
    user_input = input("Enter password length (minimum 8, press Enter for default 12):").strip()

    if user_input == "":
        length = 12
        break

    if not user_input.isdigit():
        print("Please enter a valid number.")
        continue

    length = int(user_input)
    if length < 8:
        print("Password length must be at least 8. Try again.")
        continue

    break

# Generate Password 
generator = PasswordGenerator(length=length)
password = generator.generate()

print("\nGenerated Secure Password:", password)

# Validate Password
validator = PasswordValidator(password)
results = validator.validate()
score = validator.strength_score()
label = validator.strength_label()

print("\nPassword Validation Report:")
for rule, passed in results.items():
    print(f"{rule}: {'✓' if passed else '✗'}")

# Security Features
security = SecurityFeatures(password)
score, label = security.strength_indicator()
suggestions = security.improvement_suggestions()

print(f"\nStrength Score: {score}/100")
print(f"Strength Indicator: {label}")

if suggestions:
    print("\nImprovement Suggestions:")
    for s in suggestions:
        print("-", s)
else:
    print("\nNo improvements needed. Password is secure.")

# Security Management
manager = SecurityManager()
manager.save_validation_history(password, score, label)

if "Weak" in label:
    manager.log_security_event(f"Weak password detected | Score={score}")
else:
    manager.log_security_event(f"Password validated successfully | Score={score}")