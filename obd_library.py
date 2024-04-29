class OBD2Library:
	def __init__(self):
		self.library = {
			"005": "Engine RPM",
			"026": "Brake Pedal",
			"361": "Unlock",
			"2D8": "Blinker",
			"620": "Start Stystem",
			"024": "System Off",
			"614": "Turn Signals",
			"63F": "Lighting Status",
			"226": "Foot Brake",
			"3BC": "Gear Information",
			"401": "Diagnostic Errors",
			"4F2": "Battery Status",
		}

	def decode(self, can_id):
		return self.library.get(can_id, 'Unknown')
