import os
import socket
import pickle as pk
from time import perf_counter
from shared.commands import Commands
from client_package.commands.command import Command
from shared.errors.disconnected import DisconnectedException
from shared.consts import TCP_PACKET_SIZE, MAX_PROBES, BIT_RATE_KBPS
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
                    data = self._connection.recv(TCP_PACKET_SIZE)

                    self._connection.send(pk.dumps({'ACK': True}))

                    read_bytes += len(data)

                    progress(read_bytes, self._file_size, self._file_name)

                    if not data:
                        raise DisconnectedException

                    file.write(data)
                except socket.timeout:
                    probes += 1

                    if probes > MAX_PROBES:
                        raise socket.timeout

                    print(f'\nConnection timeout. Waiting for {probes} of {MAX_PROBES}')

                    continue

    def _initiate_file_exchange(self):
        data = {'command': Commands.DOWNLOAD.value, 'file_name': self._file_name}

        message = pk.dumps(data)

        self._connection.send(message)

        response = pk.loads(self._connection.recv(TCP_PACKET_SIZE))

        if os.path.exists(self._file_name):
            os.remove(self._file_name)

        self._file_size = response['file_size']

    def execute(self):
        try:
            start_time = perf_counter()

            self._initiate_file_exchange()

            message = pk.dumps({'ACK': True})

            self._connection.send(message)

            self._get_file()

            end_time = perf_counter()

            if (end_time - start_time) > 0:
                bit_rate = self._file_size / float((end_time - start_time))
            else:
                bit_rate = -1

            print(
                f'\nFile {self._file_name} has been successfully downloaded by server,'
                f' Bit rate: {bit_rate * BIT_RATE_KBPS} kBps'
            )

        except FileNotFoundError:
            print('Such file doesn\'t exist. Try another one')
