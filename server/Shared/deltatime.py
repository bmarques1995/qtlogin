from datetime import timedelta
import re

def string_to_timedelta(time_str) -> timedelta:
    # Split the input string into days and time
    if not is_valid_dh_format(time_str):
        return timedelta(days=0, hours=0, minutes=0)
    days, time = time_str.split()
    hours, minutes = map(int, time.split(':'))
    days = int(days)
    # Create a timedelta object
    return timedelta(days=days, hours=hours, minutes=minutes)

def is_valid_dh_format(time_str):
    # Regular expression to match "dd hh:mm"
    pattern = r"^\d{1,3} \d{2}:\d{2}$"
    if re.match(pattern, time_str):
        # Further validation for hour and minute ranges
        try:
            days, time = time_str.split()
            hours, minutes = map(int, time.split(':'))
            if 0 <= hours < 24 and 0 <= minutes < 60:
                return True
        except ValueError:
            return False
    return False
