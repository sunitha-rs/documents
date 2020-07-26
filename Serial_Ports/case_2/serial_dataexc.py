import serial
import queue
import threading

queue = queue.Queue(1000)

serialPort1 =serial.Serial(port = "COM5", baudrate=9600,bytesize=8, timeout=10, stopbits=serial.STOPBITS_ONE)
serialPort2 =serial.Serial(port = "COM6", baudrate=9600,bytesize=8, timeout=10, stopbits=serial.STOPBITS_ONE)




def writing_port1():	
	CurrentDataPacket1=input("write some thing\n\r")
	write_port1=serialPort1.write(str.encode(CurrentDataPacket1))
	return write_port1

def writing_port2():	
	CurrentDataPacket2=input("write some thing\n\r")
	write_port2=serialPort2.write(str.encode(CurrentDataPacket2))
	return write_port2


def serial_read(s,source):
	print("in serial_read")
	#while True:
	line = s.readline()
	#queue.put(line)
	print("value in line: ",line,"  source: ",source)
	if source == "port1":
		write2=serialPort2.write(line)
		print("write2 : ",write2)
		read2=serialPort2.readlines()	
		print("read2 : ",read2)
	elif source == "port2":
		write1=serialPort1.write(line)
		print("write1: ",write1)
		read1=serialPort1.readlines()
		print("read1: ",read1)	
	else:
		print("ports not in range")	
		
		


print("________________________________\n\r")


WP1=writing_port1()
print("writing in port1: ",WP1)
WP2=writing_port2()
print("writing in port2: ",WP2)

threadA = threading.Thread(target=serial_read, args=(serialPort1,"port1"),).start()
threadT = threading.Thread(target=serial_read, args=(serialPort2,"port2"),).start()




    





   