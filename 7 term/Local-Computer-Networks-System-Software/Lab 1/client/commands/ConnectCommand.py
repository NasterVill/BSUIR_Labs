import socket
from client.commands.Command import Command


class ConnectCommand(Command):
    def __init__(self, params: list, connection: socket.socket):
        pass

    def execute(self):
        pass