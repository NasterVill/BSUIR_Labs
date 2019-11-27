from client_package.commands.command import Command
from shared.commands import Commands
from shared.udp.udp_transport import send
from shared.udp.compose_packets import compose_packets
import socket
import pickle as pk
from typing import Tuple


class UdpEchoCommand(Command):
    _data: str
    _connection: socket.socket
    _address: Tuple[str, int]

    def __init__(self, params: list, connection: socket.socket):
        temp = tuple(params[0].split(':'))
        self._address = (temp[0], int(temp[1]))
        self._data = ' '.join(params[1:])
        self._connection = connection

    def execute(self):
        command = {'command': Commands.UDP_ECHO.value}
        message = compose_packets(pk.dumps(command))
        send(self._connection, self._address, message, 1)

        message = compose_packets(bytes(self._data, 'utf-8'))
        send(self._connection, self._address, message)

        print(f'Echoed data: {self._data}')
