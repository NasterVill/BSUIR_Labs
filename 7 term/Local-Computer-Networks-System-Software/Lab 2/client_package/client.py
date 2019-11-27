import socket
from client_package.executor import Executor
from client_package.errors.invalid_command import InvalidCommand
from shared.consts import UDP_SERVER_PORT


class Client:
    _executor: Executor
    _connection: socket.socket
    _udp_sock: socket.socket
    UDP_PORT: int = UDP_SERVER_PORT

    def __init__(self):
        self._set_udp_socket()
        self._executor = Executor(self._udp_sock, self._set_connection)

    def _set_udp_socket(self):
        self._udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self._udp_sock.settimeout(10.0)

    def work(self):
        print('Client application is up and running. Enter \'help\' to get familiar wit the list of commands')

        while True:
            command = input()

            try:
                self._executor.build_command(command)

                self._executor.execute()

            except socket.timeout:
                print(f'Connection timed out!\n')

                self._connection.close()

                continue

            except InvalidCommand:
                print('Provided command is invalid. Type \'help\' to get info about supported commands')

                continue

            except Exception as error:
                print(f'Caught error: {error}')

                if self._connection:
                    self._connection.close()

                continue

    def _set_connection(self, connection: socket.socket):
        self._connection = connection
