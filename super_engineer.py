import os
import subprocess
import time
import curses

def decode_obd2(data):
    # Your decoding logic for OBD-II data here
    return "Decoded: " + data

def process_can_message(stdscr, message, can_data):
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
            stdscr.addstr(f"CAN ID: {can_id} | Data: {decoded_data} | Changes: {decoded_data} <---- {can_data[can_id]}\n")
            can_data[can_id] = decoded_data  # Update the data in the dictionary
    else:
        # Add the CAN ID to the dictionary
        can_data[can_id] = decoded_data
        stdscr.addstr(f"CAN ID: {can_id} | Data: {decoded_data}\n")
    stdscr.refresh()

def main(stdscr):
    curses.noecho()
    curses.cbreak()

    stdscr.addstr("Monitoring CAN messages...\n")
    stdscr.refresh()

    # Initialize CAN data dictionary
    can_data = {}

    # Start the cansniffer process
    cansniffer_process = subprocess.Popen(['cansniffer'], stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)

    try:
        while True:
            # Read the cansniffer output
            line = cansniffer_process.stdout.readline().decode().strip()
            if line:
                process_can_message(stdscr, line, can_data)
            time.sleep(0.05)  # Adjust the sleep time for 20 frames per second
    except KeyboardInterrupt:
        curses.echo()
        curses.nocbreak()
        curses.endwin()
        print("Exiting...")
        cansniffer_process.kill()

if __name__ == "__main__":
    curses.wrapper(main)
