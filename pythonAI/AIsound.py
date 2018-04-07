import socket
import string
import sys

#创建socket对象
ClientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

host = '127.0.0.1'
port = 6666

#连接服务，指定主机和端口
ClientSocket.connect((host, port))

Apiresult = '播放'

if Apiresult.find('播放')>= 0: 
	msg = '1'
elif Apiresult.find('暂停')>= 0:
	msg = '0'
elif Apiresult.find('下一首')>= 0:
	msg = '2'
elif Apiresult.find('上一首')>= 0:
	msg = '3'
else:
	msg = 'nothing'
	
ClientSocket.send(msg.encode('utf-8'))
ClientSocket.close()	

