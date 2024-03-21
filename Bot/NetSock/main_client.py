import os
import socket

# Read the HOST_IP from the environment variables
hostname = os.getenv("HOSTNAME")
port = int(os.getenv("NET_PORT"))

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

s.connect((hostname, port))

while True:
	# Receive the prompt from the host
	prompt = s.recv(1024)
	print(f"Received from host: {prompt}")

	if prompt == b'exit':
		break
	
	# Send the prompt to chatbot via API
	response = requests.post("http://bot:8000/chat", json={"prompt": prompt})

	# DBUG #
	print(f"Responce from chatbot: {response.text}")

	# Send the response back to the host
	s.sendall(response.text.encode())

s.close()

if __name__ == '__main__':
	pass
