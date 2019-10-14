import socket
from client.Executor import Executor


class Client:
    _socket: socket.socket
    _executor: Executor

    def __init__(self):
        self._executor = Executor()

    def work(self):
        while True:
            command = input()

            try:
                self._executor.build_command(command)

                self._executor.execute()

            except Exception:
                print('Smth went wrong')
