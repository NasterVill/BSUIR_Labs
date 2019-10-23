import os
from datetime import datetime
from server.commands.Command import Command
from server.ClientDescriptor import ClientDescriptor
from shared.Utils.Message import compose_message
from server.Errors.ClientHasDisconnectedException import ClientHasDisconnectedException
from shared.Consts import PACKET_SIZE


class UploadCommand(Command):
    _file_name: str
    _file_size: int
    _client: ClientDescriptor
    _last_command: Command

    def __init__(self, configuration: dict, client: ClientDescriptor, last_command: Command):
        self._last_command = last_command
        self._file_name = configuration['file_name']
        self._file_size = configuration['file_size']
        self._client = client

    def _is_old_client_reconnect(self):
        if isinstance(self._last_command, UploadCommand):
            if self._last_command._client.ip_address == self._client.ip_address:
                return True

        return False

    def _process_file_handle(self):
        if os.path.exists(self._file_name) and not self._is_old_client_reconnect():
            os.remove(self._file_name)

    def _compose_response_message(self, real_size):
        data = {'offset': 0}

        if self._is_old_client_reconnect():
            data = {'offset': (real_size - 1)}

        return compose_message(data)

    def execute(self):
        try:
            dt = datetime.now()
            start_time = dt.second

            real_file_size = 0
            if os.path.exists(self._file_name):
                real_file_size = os.path.getsize(self._file_name)

            server_response = self._compose_response_message(real_file_size)

            self._process_file_handle()

            self._client.connection.send(server_response)

            read_bytes = real_file_size
            with open(self._file_name, 'ab') as file:
                while read_bytes < self._file_size:
                    data = self._client.connection.recv(PACKET_SIZE)

                    read_bytes += len(data)

                    if not data:
                        raise ClientHasDisconnectedException

                    file.write(data)

            dt = datetime.now()
            end_time = dt.second

            bit_rate = self._file_size / float((end_time - start_time))

            print(f'File {self._file_name} has been successfully download to sever, Bit rate: {bit_rate} Bps')

        except FileNotFoundError:
            print('Such file doesn\'t exist. Try another one')
