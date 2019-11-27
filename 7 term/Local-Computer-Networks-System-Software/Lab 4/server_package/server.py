import socket
from server_package.executor import Executor
from server_package.client_descriptor import ClientDescriptor
from shared.errors.invalid_message_error import InvalidMessageError
from shared.errors.disconnected_exception import DisconnectedException
from shared.consts import PACKET_SIZE, TIMEOUT
from shared.utils.message import get_message
from shared.utils import ip, socket as Socket


class Server:
    MAX_ACTIVE_CLIENTS = 1
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

                # handle case with tcp keepalive timeout and
                # if client_package has disconnected softly during command execution
                except socket.timeout:
                    self._current_client.connection.close()

                    print(f'Connection has timed out! Client {self._current_client.ip_address} was disconnected')

                    break

                except DisconnectedException:
                    self._current_client.connection.close()

                    print(
                        'Connection closed by client_package. ' +
                        f'{self._current_client.ip_address} has disconnected from the server'
                    )

                    break

                except ConnectionResetError as error:
                    print(f'Unexpected connection reset error! {error}')

                    break

    def _get_message(self):
        message = get_message(self._current_client.connection, PACKET_SIZE)

        if not message:
            raise DisconnectedException
        else:
            return message

    def _init_socket(self):
        # creating socket that accepts IPv4 address and works with TCP protocol
        self._main_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        Socket.set_socket_keep_alive(self._main_sock, keep_alive_time=5, keep_alive_interval=10, max_probes=10)
        self._main_sock.settimeout(1000)
        self._main_sock.bind((ip.get_local_ip_address(is_local_host=False), self.PORT))

        print(f'Server created on address: {ip.get_local_ip_address(is_local_host=False)}:{self.PORT}')

    def _listen_for_new_client(self):
        print('Listening for clients')
        self._main_sock.listen(self.MAX_ACTIVE_CLIENTS)
        connection, address = self._main_sock.accept()

        print(f'Client connected. Address {address}')

        return connection, address

    def _initialize_client(self):
        connection, address = self._listen_for_new_client()

        connection.settimeout(TIMEOUT)

        Socket.set_socket_keep_alive(connection, keep_alive_time=5, keep_alive_interval=10, max_probes=10)

        self._current_client = ClientDescriptor(connection, address)

        self._executor.set_current_client(self._current_client)
