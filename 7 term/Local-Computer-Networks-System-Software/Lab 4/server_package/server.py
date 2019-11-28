import socket
import threading
from typing import List
from server_package.client_descriptor import ClientDescriptor
from shared.consts import TIMEOUT
from shared.utils import ip, socket as Socket
from server_package.connection_handler import listen


class Server:
    MAX_ACTIVE_CLIENTS = 10
    PORT: int = 9090

    # general socket used to listen for clients
    _main_sock: socket.socket

    _connections: List[socket.socket] = []

    _mutex: threading.Lock

    def __init__(self):
        self._mutex = threading.Lock()
        self._init_socket()

    def work(self):
        while True:
            client = self._initialize_client()
            if not client:
                continue
            new_thread = threading.Thread(target=listen, args=(client, self._dispose_of_connection, self._mutex))
            new_thread.start()

    def _dispose_of_connection(self, connection: socket.socket):
        self._connections.remove(connection)

    def _init_socket(self):
        # creating socket that accepts IPv4 address and works with TCP protocol
        self._main_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        Socket.set_socket_keep_alive(self._main_sock, keep_alive_time=5, keep_alive_interval=10, max_probes=10)
        self._main_sock.settimeout(1000)
        self._main_sock.bind((ip.get_local_ip_address(is_local_host=False), self.PORT))

        self._mutex.acquire()
        print(f'Server created on address: {ip.get_local_ip_address(is_local_host=False)}:{self.PORT}')
        self._mutex.release()

    def _listen_for_new_client(self):
        self._main_sock.listen(self.MAX_ACTIVE_CLIENTS)
        connection, address = self._main_sock.accept()

        self._mutex.acquire()
        print(f'Client connected. Address {address}')
        self._mutex.release()

        return connection, address

    def _initialize_client(self):
        connection, address = self._listen_for_new_client()

        if len(self._connections) == self.MAX_ACTIVE_CLIENTS:
            self._mutex.acquire()
            print(f'Too many clients! Already hit {self.MAX_ACTIVE_CLIENTS} out of {self.MAX_ACTIVE_CLIENTS}')
            self._mutex.release()
            connection.close()

            return

        connection.settimeout(TIMEOUT)

        Socket.set_socket_keep_alive(connection, keep_alive_time=5, keep_alive_interval=10, max_probes=10)

        self._mutex.acquire()
        self._connections.append(connection)
        self._mutex.release()

        return ClientDescriptor(connection, address)
