import socket

HOST = "0.0.0.0"
PORT = 6000

# create socket
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((HOST, PORT))
server.listen(1)

print(f"Listening on {HOST}:{PORT}...")

while True:
    conn, addr = server.accept()
    print(f"Connection from {addr}")

    data = conn.recv(1024).decode()
    if data:
        print("Received from ESP:", data)
    else:
        print("No data received")

    conn.close()