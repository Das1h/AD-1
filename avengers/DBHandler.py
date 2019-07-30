import socket
import struct

class DBHandler:

    def __init__(self, host='127.0.0.1', port=12345):
        self.host = host 
        self.port = port 
        self.address = (self.host, self.port)
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.MSG_HEADER_SIZE = 4

    def connect(self):
        try:
            self.sock.connect(self.address)
            return True
        except socket.error:
            self.disconnect()
            return False

    def disconnect(self):
        self.sock.close()

    def request(self, msg):
        self.send_msg(msg)
        return self.recv_msg()

    def send_msg(self, msg):
        msg_body = msg.encode('utf-8')
        msg_header = struct.pack('!i', len(msg_body))
        return self.sock.sendall(msg_header + msg_body)

    def recv_msg(self):
        msg_header = self.recvall(self.MSG_HEADER_SIZE)
        if not msg_header:
            return None
        msg_len = struct.unpack('!i', msg_header)[0]
        return self.recvall(msg_len).decode('utf-8')

    def recvall(self, length):
        data = b''
        while length > 0:
            packet = self.sock.recv(length)
            if not packet:
                return None
            data += packet
            length -= len(packet)
        return data
