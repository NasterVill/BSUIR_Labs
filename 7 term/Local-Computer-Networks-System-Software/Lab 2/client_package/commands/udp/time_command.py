from client_package.commands.command import Command
from shared.commands import Commands
from shared.udp.udp_transport import receive, send
from shared.udp.compose_packets import compose_packets
import socket
import pickle as pk
from typing import Tuple


class UdpTimeCommand(Command):
    _connection: socket.socket
    _address: Tuple[str, int]

    def __init__(self, params: list, connection: socket.socket):
        temp = tuple(params[0].split(':'))
        self._address = (temp[0], int(temp[1]))
        self._connection = connection

    def execute(self):
        command = {'command': Commands.UDP_TIME.value}
        message = compose_packets(pk.dumps(command))
        send(self._connection, self._address, message, 1)

        date, address = receive(self._connection, self._address)
        date = pk.loads(date)
        print(f'Date is: {date}')
