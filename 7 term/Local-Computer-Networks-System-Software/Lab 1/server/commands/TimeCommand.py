import datetime
from server.commands.Command import Command
from server.ClientDescriptor import ClientDescriptor
from shared.Utils.Message import compose_message


class TimeCommand(Command):
    _client: ClientDescriptor

    def __init__(self, client: ClientDescriptor):
        self._client = client

    def execute(self):
        date = datetime.datetime.now()

        data = {'payload': date}

        message = compose_message(data)

        print(date)

        self._client.connection.sendall(message)
