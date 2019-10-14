import socket
from client.commands.Command import Command


class DisconnectCommand(Command):
    def __init__(self, connection: socket.socket):
        pass

    def execute(self):
        pass