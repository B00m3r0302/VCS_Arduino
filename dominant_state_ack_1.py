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
    for i in range(10000000000000000):  # Loop through the range
        data = f"{i:016X}"  # Convert integer to hexadecimal with leading zeros
        send_can_message("005", data)
        print(f"Sent message: 005#{data}")
        
        # Send dominant bits message for 026#
        send_can_message("026", "FFFFFFFFFFFFFFFF")
        print("Sent dominant message: 026#FFFFFFFFFFFFFFFF")
        
        time.sleep(interval)  # Maintain 25 frames per second
        current_time = time.time()
        if current_time - previous_time >= 5 * interval:  # Send the message 5 times
            break
    previous_time = current_time

# Execute the send_messages function
send_messages()
