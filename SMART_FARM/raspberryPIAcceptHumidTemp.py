import serial

# 시리얼 포트 설정
ser = serial.Serial('/dev/ttyACM0', 9600)       #아두이노가 연결된 포트, 9600 bps

try:
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
            print(line)

except KeyboardInterrupt:
    print("Program terminated")

finally:
    ser.close()

