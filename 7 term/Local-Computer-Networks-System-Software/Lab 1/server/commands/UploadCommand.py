import os
import socket
from time import perf_counter
from server.commands.Command import Command
from server.ClientDescriptor import ClientDescriptor
from shared.Errors.DisconnectedException import DisconnectedException
from shared.Consts import PACKET_SIZE, MAX_PROBES
from shared.Utils.Message import compose_message
from shared.Utils.Console import progress


class UploadCommand(Command):
    _file_name: str
    _file_size: int

    def __init__(self, configuration: dict, client: ClientDescriptor):
        self._file_name = configuration['file_name']
        self._file_size = configuration['file_size']
        self._client = client

    def _get_file(self):
        print(f'Downloading File {self._file_name}')

        read_bytes = 0
        probes = 0
        with open(self._file_name, 'wb') as file:
            while read_bytes < self._file_size:
                try:
                    data = self._client.connection.recv(PACKET_SIZE)

                    read_bytes += len(data)

                    progress(read_bytes, self._file_size)

                    if not data:
                        raise DisconnectedException

                    file.write(data)
                except socket.timeout:
                    probes += 1

                    if probes > MAX_PROBES:
                        raise socket.timeout

                    continue

    def execute(self):
        try:
            start_time = perf_counter()

            if os.path.exists(self._file_name):
                os.remove(self._file_name)

            message = compose_message({'start': True})

            self._client.connection.send(message)

            self._get_file()

            end_time = perf_counter()

            if (end_time - start_time) > 0:
                bit_rate = self._file_size / float((end_time - start_time))
            else:
                bit_rate = -1

            print(
                f'\nFile {self._file_name} has been successfully downloaded by server,'
                f' Bit rate: {bit_rate} Bps'
            )

        except FileNotFoundError:
            print('Such file doesn\'t exist. Try another one')
