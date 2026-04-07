from datetime import datetime

def security_warning(action_name):
    def decorator(func):
        def wrapper(*args, **kwargs):
            print(f"[SECURITY] {action_name} started at {datetime.now()}")
            return func(*args, **kwargs)
        return wrapper
    return decorator
