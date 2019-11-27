import os
from server_package.commands.command import Command
import pickle as pk


class UploadCommand(Command):
    _file_name: str
    _file_size: int
    _ack_send: bool = False
    _data: bytes = b''
    is_finished = False

    def __init__(self, configuration: dict):
        self._file_name = configuration['file_name']
        self._file_size = configuration['file_size']

        if os.path.exists(self._file_name):
            os.remove(self._file_name)

        print(f'Downloading File {self._file_name}')

    def set_response(self, response: bytes):
        self._data = response

    def generate_message(self):
        try:
            if self._ack_send is False:
                self._ack_send = True

                response = pk.dumps({'ACK': True})

                yield response

            with open(self._file_name, 'ab') as file:
                file.write(self._data)
                self._data = b''

            if self._file_size == os.path.getsize(self._file_name):
                print(f'\nFile {self._file_name} has been successfully downloaded by server')
                self.is_finished = True

            yield pk.dumps({'ACK': True})

        except FileNotFoundError:
            print('Such file doesn\'t exist. Try another one')

# version 1.0