import socket
from client.commands.Command import Command
from shared.Commands import Commands
from shared.Utils.Message import compose_message, get_message
from shared.Consts import PACKET_SIZE


class EchoCommand(Command):
    _data: str
    _connection: socket.socket

    def __init__(self, params: list, connection: socket.socket):
        self._data = ' '.join(params)
        self._connection = connection

    def execute(self):
        data = {'type': Commands.ECHO.value, 'payload': self._data}

        message = compose_message(data)

        self._connection.sendall(message)

        response = get_message(self._connection, PACKET_SIZE)

        echoed_data = response['payload']

        print(f'Echoed data: {echoed_data}')
