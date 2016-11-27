import socket

#edge(machine which is communication with BBB)

address = ('192.168.3.3', 8001)     # reserve ip,port for this program to send data
b = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
b.bind(address)


msg = "world"
dst = ('192.168.3.13', 8001)    #destination(BBB-XINU) ip,port
b.sendto(msg, dst)
b.close()
