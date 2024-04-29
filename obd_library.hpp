#ifndef OBD_LIBRARY_HPP
#define OBD_LIBRARY_HPP

class OBD2Library {
public:
	std::string decode(const std::string& can_id);

private:
	std::unordered_map<std::string, std::string> library = {
		{"005", "Engine RPM"},
		{"026", "Brake Pedal"},
		{"361", "Unlock"},
		{"2D8", "Blinker"},
		{"620", "Start Stystem"},
		{"024", "System Off"},
		{"614", "Turn Signals"},
		{"63F", "Lighting Status"},
		{"226", "Foot Brake"},
		{"3BC", "Gear Information"},
		{"401", "Diagnostic Errors"},
		{"4F2", "Battery Status"},
	};
};

#endif // OBD_LIBRARY_HPP
