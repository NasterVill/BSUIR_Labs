import socket
from server.Executor import Executor
from shared.Utils import Ip, Socket


class Server:
    port: int = 9090

    executor: Executor
    sock: socket.socket
    last_client_address: str

    def __init__(self):
        self.executor = Executor()

        self._init_socket()

        self.executor.sock = self.sock

    def _init_socket(self):
        self.sock = socket.socket()
        self.sock.bind((Ip.get_local_ip_address(True), self.port))
        Socket.set_socket_keep_alive(self.sock)

    def _listen_for_new_client(self):
        self.sock.listen(1)
        connection, address = self.sock.accept()

        print('Client connected. Address {}'.format(address))

        return connection, address

    def start(self):
        connection, address = self._listen_for_new_client()

        self.last_client_address = address

        while True:
            self.sock.recv(1024)

