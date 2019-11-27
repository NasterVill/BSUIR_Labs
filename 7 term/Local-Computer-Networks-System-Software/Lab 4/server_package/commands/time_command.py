import datetime
import threading
from server_package.commands.command import Command
from server_package.client_descriptor import ClientDescriptor
from shared.utils.message import compose_message


class TimeCommand(Command):
    _client: ClientDescriptor
    _mutex: threading.Lock

    def __init__(self, client: ClientDescriptor, mutex: threading.Lock):
        self._client = client
        self._mutex = mutex

    def execute(self):
        date = datetime.datetime.now()

        data = {'payload': date}

        message = compose_message(data)

        self._mutex.acquire()
        print(f'Client requested current time. Time is {date}')
        self._mutex.release()

        self._client.connection.sendall(message)
