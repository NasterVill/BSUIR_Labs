from server.commands.Command import Command
from server.ClientDescriptor import ClientDescriptor
from server.Errors.ClientHasDisconnectedError import ClientHasDisconnectedError


class DisconnectCommand(Command):
    _client: ClientDescriptor

    def __init__(self, configuration: dict, client: ClientDescriptor):
        self._client = client

    def execute(self):
        self._client.connection.close()
