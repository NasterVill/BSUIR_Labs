import socket
from server.Executor import Executor
from server.ClientDescriptor import ClientDescriptor
from shared.Errors.InvalidMessageError import InvalidMessageError
from server.Errors.ClientHasDisconnectedError import ClientHasDisconnectedError
from shared.Utils.Message import get_message
from shared.Utils import Ip, Socket


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

    def work(self):
        while True:
            self._initialize_client()

            while True:
                try:
                    message = self._get_message()

                    self._executor.build_command(message)

                    self._executor.execute()

                except InvalidMessageError:
                    print('Invalid message received! Waiting for a new one')

                    continue
                # handle case with tcp keepalive timeout and if client has disconnected softly during command execution
                except socket.timeout:
                    # TODO: check if this 'close' call is a necessity
                    self._current_client.connection.close()

                    print(f'Connection has timed out! Client {self._current_client.ip_address} was disconnected')

                    break

                except ClientHasDisconnectedError:
                    self._current_client.connection.close()

                    print(
                        'Connection closed by client. ' +
                        f'{self._current_client.ip_address} has disconnected from the server'
                    )

                    break

    def _get_message(self):
        message = get_message(self._current_client.connection, self.DEFAULT_DATA_PACKET_SIZE)

        if not message:
            raise ClientHasDisconnectedError
        else:
            return message

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

        # connection.settimeout(10)

        self._current_client = ClientDescriptor(connection, address)

        self._executor.set_current_client(self._current_client)
