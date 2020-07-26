import serial
       

serialPort1 =serial.Serial(port = "COM5", baudrate=9600,bytesize=8, timeout=10, stopbits=serial.STOPBITS_ONE)
serialPort2 =serial.Serial(port = "COM6", baudrate=9600,bytesize=8, timeout=10, stopbits=serial.STOPBITS_ONE)

print(serialPort2.name)
print(serialPort1.name)

while True:
	CurrentDataPacket=input("write some thing\n\r")
	write1=serialPort1.write(str.encode(CurrentDataPacket))
	write2=serialPort2.write(str.encode(CurrentDataPacket))
	read2=serialPort2.readlines()
	read1=serialPort1.readlines()
	print("reading in port_2 : ",read2)
	print("reading in port_1 : ",read1)