import os
import pickle as pk
from shared.commands import Commands
from server_package.commands.command import Command
from shared.consts import TCP_PACKET_SIZE


class DownloadCommand(Command):
    _file_name: str
    _file_size: int = 0
    _data: bytes = b''
    _meta_data_sent: bool = False
    _sent_bytes: int = 0
    is_finished = False

    def __init__(self, config_dict: dict):
        self._file_name = config_dict['file_name']
        self._file_size = os.path.getsize(self._file_name)

    def set_response(self, response: bytes):
        self._data = response

    def generate_message(self):
        try:
            if self._sent_bytes == self._file_size:
                self.is_finished = True
                print(f'\nFile {self._file_name} has been successfully uploaded by server')

            if not self._meta_data_sent:
                self._meta_data_sent = True

                data = {'type': Commands.DOWNLOAD.value, 'file_name': self._file_name, 'file_size': self._file_size}

                yield pk.dumps(data)

            with open(self._file_name, 'rb') as file:
                file.seek(self._sent_bytes)

                file_portion = file.read(TCP_PACKET_SIZE)

                self._sent_bytes += len(file_portion)

                yield file_portion

        except FileNotFoundError:
            print('Such file doesn\'t exist. Please try another one')
