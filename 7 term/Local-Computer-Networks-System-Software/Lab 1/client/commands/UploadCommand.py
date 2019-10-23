import os
import socket
from datetime import datetime
from shared.Commands import Commands
from client.commands.Command import Command
from shared.Utils.Message import compose_message, get_message
from shared.Consts import PACKET_SIZE


class UploadCommand(Command):
    _relative_file_name: str
    _connection: socket.socket

    def __init__(self, params: list, connection: socket.socket):
        self._connection = connection
        self._relative_file_name = params[0]

    def execute(self):
        try:
            dt = datetime.now()
            start_time = dt.second

            file = open(self._relative_file_name, 'rb')

            file_size = os.path.getsize(self._relative_file_name)

            data = {'type': Commands.UPLOAD.value, 'file_name': self._relative_file_name, 'file_size': file_size}

            message = compose_message(data)

            self._connection.send(message)

            response = get_message(self._connection, PACKET_SIZE)

            offset = response['offset']

            file.seek(offset)

            file_portion = file.read(PACKET_SIZE)
            while file_portion:
                self._connection.send(file_portion)
                file_portion = file.read(PACKET_SIZE)

            file.close()

            dt = datetime.now()
            end_time = dt.second

            bit_rate = file_size / float((end_time - start_time))

            print(f'File {self._relative_file_name} has been successfully uploaded by client, Bit rate: {bit_rate} Bps')

        except FileNotFoundError:
            print('Such file doesn\'t exist. Try another one')
