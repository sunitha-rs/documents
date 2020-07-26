
import serial
       

serialPort1 =serial.Serial(port = "COM3", baudrate=9600,bytesize=8, timeout=5, stopbits=serial.STOPBITS_ONE)


#print(serialPort1.name)

while True:
	CurrentDataPacket=input("write some thing\n\r")
	write=serialPort1.write(str.encode(CurrentDataPacket))
	print("writing into port",write)


	read=serialPort1.readlines()
	print("reading from the port",read)

