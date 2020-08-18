import socket

HOST = '15.165.204.254'                 # Symbolic name meaning all available interfaces
PORT = 8080           # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
print("connected!")
while True:
    
    data = input("input: ")
    
    s.sendall(data.encode())
    
