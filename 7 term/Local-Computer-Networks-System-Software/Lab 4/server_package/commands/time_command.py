import datetime
from server_package.commands.command import Command
from server_package.client_descriptor import ClientDescriptor
from shared.utils.message import compose_message


class TimeCommand(Command):
    _client: ClientDescriptor

    def __init__(self, client: ClientDescriptor):
        self._client = client

    def execute(self):
        date = datetime.datetime.now()

        data = {'payload': date}

        message = compose_message(data)

        print(f'Client requested current time. Time is {date}')

        self._client.connection.sendall(message)
