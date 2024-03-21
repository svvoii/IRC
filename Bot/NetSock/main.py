# This is the main program which acts as a server designed to run in the container
# It listens for incoming connections from the client and sends back the prompt received from the client

import os
import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Read the HOST_IP from the environment variables
# hostname = os.getenv("HOSTNAME")
port = int(os.getenv("NET_PORT"))
server_address = ('0.0.0.0', port) # must be `0.0.0.0` to allow connections from outside the container
sock.bind(server_address)

sock.listen(1)

while True:
	# Wait for a connection
	print("Waiting for a connection...")
	connection, client_address = sock.accept()

	print(f"Connection from {client_address}")

	try:
		print(f"Connection from {client_address}")

		while True:
			prompt = connection.recv(512)
			print(f"Received from client: {prompt}")

			if prompt:
				print(f"Sending prompt back to the client: {prompt}")
				connection.sendall(prompt)
			else:
				print("No more prompt from client")
				break

	finally:
		# Clean up the connection
		connection.close()

if __name__ == '__main__':
	pass
