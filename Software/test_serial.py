import serial
import time

# Initialize serial connection to Arduino
ser = serial.Serial('COM9', 9600)  # Adjust COM port
time.sleep(2)  # Wait for the connection to stabilize

while True:
    try:
        # Read and print data from Arduino if available
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').strip()
            print(f"Arduino Data: {data}")
    except KeyboardInterrupt:
        print("Exiting Program")
        break

ser.close()  # Close the serial port after exiting
