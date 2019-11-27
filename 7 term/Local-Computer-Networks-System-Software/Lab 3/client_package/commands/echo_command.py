import socket
from client_package.commands.command import Command
from shared.commands import Commands
from shared.utils.message import get_message
from shared.consts import PACKET_SIZE
import pickle as pk


class EchoCommand(Command):
    _data: str
    _connection: socket.socket

    def __init__(self, params: list, connection: socket.socket):
        self._data = ' '.join(params)
        self._connection = connection

    def execute(self):
        data = {'command': Commands.ECHO.value, 'data_length': len(self._data)}

        message = pk.dumps(data)

        self._connection.sendall(message)

        response = pk.loads(self._connection.recv(PACKET_SIZE))

        if response['ACK'] is True:
            self._connection.sendall(bytes(self._data, 'utf-8'))

            echoed_data = get_message(self._connection, len(self._data), PACKET_SIZE).decode('utf-8')

            print(f'Echoed data: {echoed_data}')
        else:
            raise Exception('Server couldn\'t process the command!')
