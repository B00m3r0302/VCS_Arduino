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
            "4F2": "Battery Status"
            # Add more OBD-II parameters here
        }

    def decode(self, can_id):
        if can_id in self.library:
            return self.library[can_id]
        else:
            return "Unknown"

def main():
    # Create OBD2Library object
    obd2 = OBD2Library()

    # Print known OBD-II parameters
    print("Known OBD-II Parameters:")
    for can_id, parameter in obd2.library.items():
        print(f"{can_id}: {parameter}")

    # Generate library file for C++
    with open("obd_library.hpp", "w") as hpp_file:
        hpp_file.write("#ifndef OBD_LIBRARY_HPP\n")
        hpp_file.write("#define OBD_LIBRARY_HPP\n\n")
        hpp_file.write("class OBD2Library {\n")
        hpp_file.write("public:\n")
        hpp_file.write("\tstd::string decode(const std::string& can_id);\n\n")
        hpp_file.write("private:\n")
        hpp_file.write("\tstd::unordered_map<std::string, std::string> library = {\n")
        for can_id, parameter in obd2.library.items():
            hpp_file.write(f"\t\t{{\"{can_id}\", \"{parameter}\"}},\n")
        hpp_file.write("\t};\n")
        hpp_file.write("};\n\n")
        hpp_file.write("#endif // OBD_LIBRARY_HPP\n")

    print("Created obd_library.hpp")

    # Generate library file for Python
    with open("obd_library.py", "w") as py_file:
        py_file.write("class OBD2Library:\n")
        py_file.write("\tdef __init__(self):\n")
        py_file.write("\t\tself.library = {\n")
        for can_id, parameter in obd2.library.items():
            py_file.write(f"\t\t\t\"{can_id}\": \"{parameter}\",\n")
        py_file.write("\t\t}\n\n")
        py_file.write("\tdef decode(self, can_id):\n")
        py_file.write("\t\treturn self.library.get(can_id, 'Unknown')\n")

    print("Created obd_library.py")

if __name__ == "__main__":
    main()
