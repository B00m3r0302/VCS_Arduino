import os
import time
import subprocess
import curses

def decode_obd2(data):
    # Your decoding logic for OBD-II data here
    return "Decoded: " + data

def process_can_message(stdscr, message):
    parts = message.split()
    can_id = parts[2]
    data = parts[3:]
    data_str = ' '.join(data)
    
    # Decode OBD-II data
    decoded_data = decode_obd2(data_str)

    # Check if the CAN ID already exists in the dictionary
    if can_id in can_data:
        if can_data[can_id] != decoded_data:
            # Update the existing CAN ID data
            can_data[can_id] = decoded_data
            stdscr.addstr(f"CAN ID: {can_id} | Data: {decoded_data} | Changes: {decoded_data} <---- {can_data[can_id]}\n")
    else:
        # Add the CAN ID to the dictionary
        can_data[can_id] = decoded_data
        stdscr.addstr(f"CAN ID: {can_id} | Data: {decoded_data}\n")
    stdscr.refresh()

# Initialize CAN data dictionary
can_data = {}

# Start the candump process
candump_process = subprocess.Popen(['candump', 'can0'], stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)

try:
    stdscr = curses.initscr()
    curses.noecho()
    curses.cbreak()

    stdscr.addstr("Monitoring CAN messages...\n")
    stdscr.refresh()

    while True:
        # Read the candump output
        line = candump_process.stdout.readline().decode().strip()
        if line:
            process_can_message(stdscr, line)
        time.sleep(0.05)  # Adjust the sleep time for 20 frames per second
except KeyboardInterrupt:
    curses.echo()
    curses.nocbreak()
    curses.endwin()
    print("Exiting...")
    candump_process.kill()
