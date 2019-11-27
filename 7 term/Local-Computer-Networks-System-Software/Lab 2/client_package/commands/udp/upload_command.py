from client_package.commands.command import Command
from shared.commands import Commands
from shared.udp.udp_transport import send
from shared.udp.compose_packets import compose_packets_from_file, compose_packets
import os
import socket
import pickle as pk
from typing import Tuple


class UdpUploadCommand(Command):
    _connection: socket.socket
    _address: Tuple[str, int]
    _file_name: str

    def __init__(self, params: list, connection: socket.socket):
        temp = tuple(params[0].split(':'))
        self._address = (temp[0], int(temp[1]))
        self._connection = connection
        self._file_name = params[1]

        if not os.path.exists(self._file_name):
            raise FileNotFoundError

    def execute(self):
        command = {'command': Commands.UDP_UPLOAD.value, 'file_name': self._file_name}
        message = compose_packets(pk.dumps(command))
        send(self._connection, self._address, message, 1)

        message = compose_packets_from_file(self._file_name)
        send(self._connection, self._address, message, show_progress=True)

        print(f'File {self._file_name} has been successfully uploaded by client')
