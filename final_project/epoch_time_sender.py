import time
import serial

BAUD_RATE = 9600
SERIAL_PORT = "/dev/cu.usbmodem1422401"  # Replace with the appropriate serial port on your computer

# Open the serial connection
ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
ser.flushInput()

while True:
    if ser.inWaiting() > 0:
        print("waiting")
        # Read the incoming data from the Arduino
        received_data = ser.readline().decode().strip()
        
        if received_data == "GET_EPOCH":
            # Get the current epoch time
            epoch_time = str(int(time.time()))

            # Send the epoch time back to the Arduino
            ser.write(epoch_time.encode() + b'\n')
