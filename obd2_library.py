class OBD2Library:
    def __init__(self):
        self.library = {
            "010": "Engine RPM",
            "011": "Vehicle Speed",
            "012": "Throttle Position",
            # Add more OBD-II parameters here
        }

    def decode(self, can_id):
        if can_id in self.library:
            return self.library[can_id]
        else:
            return "Unknown"

# Example usage:
obd2 = OBD2Library()

# Decode CAN IDs
print(obd2.decode("010"))  # Output: Engine RPM
print(obd2.decode("011"))  # Output: Vehicle Speed
print(obd2.decode("020"))  # Output: Unknown (not in the library)
