import os
import socket
from server_package.commands.command import Command
from shared.udp.udp_transport import send
from shared.udp.compose_packets import compose_packets_from_file
from typing import Tuple


class UdpDownloadCommand(Command):
    _sock: socket.socket

    def __init__(self, sock: socket.socket, address: Tuple[str, int], configuration: dict):
        self._sock = sock
        self._address = address
        self._file_name = configuration['file_name']

        if not os.path.exists(self._file_name):
            raise FileNotFoundError

    def generate_message(self):
        message = compose_packets_from_file(self._file_name)
        send(self._sock, self._address, message, show_progress=True)

        print(f'File {self._file_name} has been successfully uploaded by server')
