import socket
from client_package.commands.command import Command


class DisconnectCommand(Command):
    _connection: socket.socket

    def __init__(self, connection: socket.socket):
        self._connection = connection

    def execute(self):
        self._connection.shutdown(socket.SHUT_RDWR)
        self._connection.close()

        print('Successfully disconnected from the server_package')
