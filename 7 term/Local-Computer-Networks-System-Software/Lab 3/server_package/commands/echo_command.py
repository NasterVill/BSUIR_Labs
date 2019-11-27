import pickle as pk
from server_package.commands.command import Command


class EchoCommand(Command):
    _data: str = ''
    _ack_send: bool = False
    is_finished = False

    def __init__(self, configuration: dict):
        self._data_length = configuration['data_length']
        self._sent_bytes = 0

    def set_response(self, response: bytes):
        self._data = response.decode("utf-8")

    def generate_message(self):
        if self._ack_send is False:
            self._ack_send = True

            response = pk.dumps({'ACK': True})

            yield response

        data = self._data
        self._data = ''
        self._sent_bytes += len(data)

        if self._sent_bytes == self._data_length:
            self.is_finished = True

        if data:
            print(f'Echo: {data}')

        yield bytes(data, 'utf-8')
