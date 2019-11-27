import threading
from server_package.commands.command import Command
from server_package.client_descriptor import ClientDescriptor
from shared.utils.message import compose_message


class EchoCommand(Command):
    _data: str
    _client: ClientDescriptor
    _mutex: threading.Lock

    def __init__(self, configuration: dict, client: ClientDescriptor, mutex: threading.Lock):
        self._data = configuration['payload']
        self._client = client
        self._mutex = mutex

    def execute(self):
        data = {'payload': self._data}

        self._mutex.acquire()
        print(f'Echoed data from client: {self._data}')
        self._mutex.release()

        message = compose_message(data)

        self._client.connection.sendall(message)
