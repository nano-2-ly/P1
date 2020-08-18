import socket

print("program start!")

HOST = ''                 # Symbolic name meaning all available interfaces
PORT = 8080           # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 8080))
s.listen(1)

p = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
p.bind(('', 8008))
p.listen(1)


conn1, addr1 = s.accept()
print("python accepted!")

conn2, addr2 = p.accept()
print("nodeMCU accepted!")

while True:
    try:
        data = conn1.recv(65535).decode()
        print(data)

        conn2.sendall(data.encode())

    except :
        print("exception occur")
        conn1, addr1 = s.accept()
        conn2, addr2 = p.accept()
        print("accepted!")
