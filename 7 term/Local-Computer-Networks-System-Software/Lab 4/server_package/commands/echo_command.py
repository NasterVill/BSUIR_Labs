from server_package.commands.command import Command
from server_package.client_descriptor import ClientDescriptor
from shared.utils.message import compose_message


class EchoCommand(Command):
    _data: str
    _client: ClientDescriptor

    def __init__(self, configuration: dict, client: ClientDescriptor):
        self._data = configuration['payload']
        self._client = client

    def execute(self):
        data = {'payload': self._data}

        print(f'Echoed data from client: {self._data}')

        message = compose_message(data)

        self._client.connection.sendall(message)
