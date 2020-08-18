import socket
  
print("program start!")

HOST = ''                 
PORT = 8080           
python_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
python_client.bind(('', 8080))
python_client.listen(1)

nodemcu_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
nodemcu_client.bind(('', 8008))
nodemcu_client.listen(1)
          
         
conn1, addr1 = python_client.accept()
print("python accepted!")
        
conn2, addr2 = nodemcu_client.accept()
print("nodeMCU accepted!")
      
while True:
    try:
        data = conn1.recv(65535).decode()
        print(data)
    except :
        print("python connection has problem.")
        conn1, addr1 = python_client.accept()
        
        data = conn1.recv(65535).decode()
        print(data)

    try:
        conn2.sendall(data.encode())
    except:
        print("nodeMCU connection has problem.")
        conn2, addr2 = nodemcu_client.accept()
        
        conn2.sendall(data.encode())

