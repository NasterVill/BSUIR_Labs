import datetime
from server.commands.Command import Command
from server.ClientDescriptor import ClientDescriptor
from shared.Utils.Message import compose_message


class TimeCommand(Command):
    _client: ClientDescriptor

    def __init__(self, configuration: dict, client: ClientDescriptor):
        self._client = client

    def execute(self):
        data = {'payload': datetime.datetime.now()}

        message = compose_message(data)

        self._client.connection.sendall(message)
