import os
import socket
from time import perf_counter
from shared.Commands import Commands
from client.commands.Command import Command
from shared.Utils.Message import compose_message, get_message
from shared.Consts import PACKET_SIZE, MAX_PROBES
from shared.Errors.FileExchangeDenial import FileExchangeDenial
from shared.Utils.Console import progress


class UploadCommand(Command):
    _file_name: str
    _connection: socket.socket

    def __init__(self, params: list, connection: socket.socket):
        self._connection = connection
        self._file_name = params[0]

    def execute(self):
        try:
            start_time = perf_counter()

            file = open(self._file_name, 'rb')
            file_size = os.path.getsize(self._file_name)

            self._send_request_message(self._file_name, file_size)

            self._send_file(file, file_size)

            file.close()

            end_time = perf_counter()

            if (end_time - start_time) > 0:
                bit_rate = file_size / float((end_time - start_time))
            else:
                bit_rate = -1

            print(f'\nFile {self._file_name} has been successfully uploaded by client, Bit rate: {bit_rate} Bps')

        except FileNotFoundError:
            print('Such file doesn\'t exist. Please try another one')

    def _send_request_message(self, file_name: str, file_size: int):
        data = {'type': Commands.UPLOAD.value, 'file_name': file_name, 'file_size': file_size}

        message = compose_message(data)

        self._connection.send(message)

        response = get_message(self._connection)

        if not response['start']:
            print('Server denied upload!')

            raise FileExchangeDenial

    def _send_file(self, file_handle, file_size: int):
        print(f'Uploading File {self._file_name}')

        file_handle.seek(0)
        file_portion = file_handle.read(PACKET_SIZE)

        probes = 0
        sent_bytes = 0
        while file_portion:
            try:
                self._connection.send(file_portion)

                sent_bytes += len(file_portion)

                file_portion = file_handle.read(PACKET_SIZE)

                progress(sent_bytes, file_size)
            except socket.timeout:
                probes += 1

                if probes > MAX_PROBES:
                    raise socket.timeout

                continue
