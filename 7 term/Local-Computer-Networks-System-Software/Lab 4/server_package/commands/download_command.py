import os
import socket
from time import perf_counter
from shared.commands import Commands
from server_package.commands.command import Command
from server_package.client_descriptor import ClientDescriptor
from shared.utils.message import compose_message, get_message
from shared.consts import PACKET_SIZE, MAX_PROBES, BIT_RATE_KBPS
from shared.errors.file_exchange_denial import FileExchangeDenial
from shared.utils.console import progress


class DownloadCommand(Command):
    _file_name: str
    _client: ClientDescriptor

    def __init__(self, config_dict: dict, client: ClientDescriptor):
        self._client = client
        self._file_name = config_dict['file_name']

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
        data = {'type': Commands.DOWNLOAD.value, 'file_name': file_name, 'file_size': file_size}

        message = compose_message(data)

        self._client.connection.send(message)

        response = get_message(self._client.connection)

        if not response['start']:
            print('Client denied download!')

            raise FileExchangeDenial

    def _send_file(self, file_handle, file_size):
        print(f'Uploading File {self._file_name}')

        file_handle.seek(0)
        file_portion = file_handle.read(PACKET_SIZE)

        probes = 0
        sent_bytes = 0
        while file_portion:
            try:
                self._client.connection.send(file_portion)

                sent_bytes += len(file_portion)

                file_portion = file_handle.read(PACKET_SIZE)

                progress(sent_bytes, file_size)
            except socket.timeout:
                probes += 1

                if probes > MAX_PROBES:
                    raise socket.timeout

                print(f'\nConnection timeout. Waiting for {probes} of {MAX_PROBES}')

                continue
