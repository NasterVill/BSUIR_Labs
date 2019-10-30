import socket
from client.Executor import Executor
from client.Errors.InvalidCommand import InvalidCommand


class Client:
    _executor: Executor
    _connection: socket.socket

    def __init__(self):
        self._executor = Executor(self._set_connection)

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
                print(f'Unexpected error caught! {error}')

                self._connection.close()

                continue

    def _set_connection(self, connection: socket.socket):
        self._connection = connection
