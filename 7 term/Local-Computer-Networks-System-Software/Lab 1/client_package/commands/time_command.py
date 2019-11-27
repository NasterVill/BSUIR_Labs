import socket
from client_package.commands.command import Command
from shared.commands import Commands
from shared.utils.message import compose_message, get_message
from shared.consts import PACKET_SIZE


class TimeCommand(Command):
    _connection: socket.socket

    def __init__(self, params: list, connection: socket.socket):
        self._connection = connection

    def execute(self):
        data = {'type': Commands.TIME.value}

        message = compose_message(data)

        self._connection.sendall(message)

        response = get_message(self._connection, PACKET_SIZE)

        time = response['payload']

        print(f'Time, according to server, is {time}')
