import socket

#edge(machine which is communication with BBB)

address = ('192.168.3.3', 8000)     #reserve ip,port for this program(for listening)
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(address)

while True:
    data, addr = s.recvfrom(2048)
    if not data:
        print "client has exist"
        break
    print "received:", data, "from", addr

s.close()
