import secrets
import string
from SecurityDecorators import security_warning

class PasswordGenerator:
    def __init__(self,length=12,use_upper=True,use_lower=True,use_digits=True,use_symbols=True
    ):
        self.length = length if length else 12
        self.use_upper = use_upper
        self.use_lower = use_lower
        self.use_digits = use_digits
        self.use_symbols = use_symbols

        self._validate_options()
        self.char_pool = self._build_char_pool()

    def _validate_options(self):
        if not isinstance(self.length, int) or self.length < 8:
            print("Invalid or short length. Using minimum length 8.")
            self.length = 8

        if not any([self.use_upper, self.use_lower, self.use_digits, self.use_symbols]):
            raise ValueError("At least one character type must be selected.")

    def _build_char_pool(self):
        pool = ""
        if self.use_upper:
            pool += string.ascii_uppercase
        if self.use_lower:
            pool += string.ascii_lowercase
        if self.use_digits:
            pool += string.digits
        if self.use_symbols:
            pool += string.punctuation
        return pool

    @security_warning("Password Generation")
    def generate(self):
        return ''.join(
            secrets.choice(self.char_pool)
            for _ in range(self.length)
        )
