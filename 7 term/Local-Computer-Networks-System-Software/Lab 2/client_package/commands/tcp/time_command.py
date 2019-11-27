import socket
from client_package.commands.command import Command
from shared.commands import Commands
from shared.consts import TCP_PACKET_SIZE
import pickle as pk


class TimeCommand(Command):
    _connection: socket.socket

    def __init__(self, params: list, connection: socket.socket):
        self._connection = connection

    def execute(self):
        data = {'command': Commands.TIME.value}

        message = pk.dumps(data)

        self._connection.sendall(message)

        response = pk.loads(self._connection.recv(TCP_PACKET_SIZE))

        time = response['payload']

        print(f'Time, according to server, is {time}')
