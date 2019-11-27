import os
import socket
import pickle as pk
from time import perf_counter
from shared.commands import Commands
from client_package.commands.command import Command
from shared.consts import PACKET_SIZE, MAX_PROBES, BIT_RATE_KBPS
from shared.errors.file_exchange_denial import FileExchangeDenial
from shared.utils.console import progress


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

            print(
                f'\nFile {self._file_name} has been successfully uploaded by client,'
                f' Bit rate: {bit_rate * BIT_RATE_KBPS} kBps'
            )

        except FileNotFoundError:
            print('Such file doesn\'t exist. Please try another one')

    def _send_request_message(self, file_name: str, file_size: int):
        data = {'command': Commands.UPLOAD.value, 'file_name': file_name, 'file_size': file_size}

        message = pk.dumps(data)

        self._connection.send(message)

        response = pk.loads(self._connection.recv(PACKET_SIZE))

        if 'ACK' not in response:
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

                response = pk.loads(self._connection.recv(PACKET_SIZE))

                if 'ACK' not in response:
                    print('Server denied upload!')

                    raise FileExchangeDenial

                sent_bytes += len(file_portion)

                file_portion = file_handle.read(PACKET_SIZE)

                progress(sent_bytes, file_size, self._file_name)
            except socket.timeout:
                probes += 1

                if probes > MAX_PROBES:
                    raise socket.timeout

                print(f'\nConnection timeout. Waiting for {probes} of {MAX_PROBES}')

                continue
