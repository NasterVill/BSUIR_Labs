import os
import socket
from time import perf_counter
from shared.commands import Commands
from client_package.commands.command import Command
from shared.errors.disconnected_exception import DisconnectedException
from shared.consts import PACKET_SIZE, MAX_PROBES, BIT_RATE_KBPS
from shared.utils.message import compose_message, get_message
from shared.utils.console import progress


class DownloadCommand(Command):
    _file_name: str
    _connection: socket.socket
    _file_size: int

    def __init__(self, params: list, connection: socket.socket):
        self._file_name = params[0]
        self._connection = connection

    def _get_file(self):
        print(f'Downloading File {self._file_name}')

        read_bytes = 0
        probes = 0
        with open(self._file_name, 'wb') as file:
            while read_bytes < self._file_size:
                try:
                    data = self._connection.recv(PACKET_SIZE)

                    read_bytes += len(data)

                    progress(read_bytes, self._file_size)

                    if not data:
                        raise DisconnectedException

                    file.write(data)
                except socket.timeout:
                    probes += 1

                    if probes > MAX_PROBES:
                        raise socket.timeout

                    print(f'\nConnection timeout. Waiting for {probes} of {MAX_PROBES}')

                    continue

    def execute(self):
        try:
            start_time = perf_counter()

            message = compose_message({'type': Commands.DOWNLOAD.value, 'file_name': self._file_name})

            self._connection.send(message)

            response = get_message(self._connection)

            self._file_size = response['file_size']

            if os.path.exists(self._file_name):
                os.remove(self._file_name)

            message = compose_message({'start': True})

            self._connection.send(message)

            self._get_file()

            end_time = perf_counter()

            if (end_time - start_time) > 0:
                bit_rate = self._file_size / float((end_time - start_time))
            else:
                bit_rate = -1

            print(
                f'\nFile {self._file_name} has been successfully uploaded by client,'
                f' Bit rate: {bit_rate * BIT_RATE_KBPS} kBps'
            )

        except FileNotFoundError:
            print('Such file doesn\'t exist. Try another one')
