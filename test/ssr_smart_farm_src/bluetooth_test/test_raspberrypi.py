import bluetooth

def find_hc05():
	
	nearby_devices = bluetooth.discover_devices(
		duration =8, lookp_names=True, flush_cache=True, lookup_class = False)
	
	for addr, name in nearby_devices:
		if 'HC-05' in name:
			return addr
	
	return None
	
def main():
	
	bt_addr = find_hc05()
	if bt_addr is None:
		print("Could not find HC-05 device.")
		return
		
	port = 1
	sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
	sock.connect(bt_addr, port)
	
	print("Connected to HC-05")
		
	try:
		while True:
			data = input("Enter message to send: ")
			sock.send(data)
			received = sock.recv(1024)
			print(f"Received: {received.decode('utf-8')}")
	except KeyboardInterrupt:
		print("Disconnected")
	finally:
		sock.close()
		
if __name__ == " __main__":
	main()
