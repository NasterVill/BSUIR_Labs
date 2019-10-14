import socket
from server.Executor import Executor
from server.ClientDescriptor import ClientDescriptor
from shared.Utils import Ip, Socket
from shared.Consts import HEADER_SIZE
import pickle


class Server:
    MAX_ACTIVE_CLIENTS = 1
    DEFAULT_DATA_PACKET_SIZE = 1024
    PORT: int = 9090

    # general socket used to listen for clients
    _main_sock: socket.socket

    _executor: Executor
    # socket for specific connection
    _current_client: ClientDescriptor

    def __init__(self):
        self._executor = Executor()

        self._init_socket()

    def _init_socket(self):
        # creating socket that accepts IPv4 address and works with TCP protocol
        self._main_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._main_sock.bind((Ip.get_local_ip_address(is_local_host=True), self.PORT))

        print(f'Server created on address: {Ip.get_local_ip_address(is_local_host=True)}:{self.PORT}')

    def _listen_for_new_client(self):
        self._main_sock.listen(self.MAX_ACTIVE_CLIENTS)
        connection, address = self._main_sock.accept()

        print(f'Client connected. Address {address}')

        return connection, address

    def _initialize_client(self):
        connection, address = self._listen_for_new_client()

        Socket.set_socket_keep_alive(connection)

        self._current_client = ClientDescriptor(connection, address)

        self._executor.set_current_client(self._current_client)

    def work(self):
        while True:
            self._initialize_client()

            full_message = b''
            new_message = True
            message_len = 0
            while True:
                try:
                    tmp_message = self._current_client.connection.recv(self.DEFAULT_DATA_PACKET_SIZE)

                # handle case with tcp keepalive timeout
                except socket.timeout:
                    # TODO: check if this 'close' call is a necessity
                    self._current_client.connection.close()

                    print(f'Connection has timed out! Client {self._current_client.ip_address} was disconnected')

                    break

                # if the client has disconnected
                if not tmp_message:
                    self._current_client.connection.close()

                    print(
                        f'Connection closed by client. ' +
                        f'{self._current_client.ip_address} has disconnected from the server'
                    )

                    break

                # starting processing new message
                if new_message:
                    message_len = int(tmp_message[:HEADER_SIZE])
                    new_message = False

                full_message += tmp_message

                # if we got whole message
                if len(full_message) - HEADER_SIZE == message_len:
                    message = pickle.loads(full_message[HEADER_SIZE:])

                    # got full message and can now process it
                    message += ''

                pass
