import os
import socket
import threading
from time import perf_counter
from server_package.commands.command import Command
from server_package.client_descriptor import ClientDescriptor
from shared.errors.disconnected_exception import DisconnectedException
from shared.consts import PACKET_SIZE, MAX_PROBES, BIT_RATE_KBPS
from shared.utils.message import compose_message
from shared.utils.console import progress


class UploadCommand(Command):
    _file_name: str
    _file_size: int
    _mutex: threading.Lock

    def __init__(self, configuration: dict, client: ClientDescriptor, mutex: threading.Lock):
        self._file_name = configuration['file_name']
        self._file_size = configuration['file_size']
        self._client = client
        self._mutex = mutex

    def _get_file(self):
        self._mutex.acquire()
        print(f'Downloading File {self._file_name}')
        self._mutex.release()

        read_bytes = 0
        probes = 0
        with open(self._file_name, 'wb') as file:
            while read_bytes < self._file_size:
                try:
                    data = self._client.connection.recv(PACKET_SIZE)

                    read_bytes += len(data)

                    if not data:
                        raise DisconnectedException

                    file.write(data)
                except socket.timeout:
                    probes += 1

                    if probes > MAX_PROBES:
                        raise socket.timeout

                    self._mutex.acquire()
                    print(f'\nConnection timeout. Waiting for {probes} of {MAX_PROBES}')
                    self._mutex.release()

                    continue

    def execute(self):
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

        self._mutex.acquire()
        print(
            f'\nFile {self._file_name} has been successfully uploaded by client,'
            f' Bit rate: {bit_rate * BIT_RATE_KBPS} kBps'
        )
        self._mutex.release()
