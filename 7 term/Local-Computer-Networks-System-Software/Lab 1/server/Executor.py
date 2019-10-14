from server.ClientDescriptor import ClientDescriptor
from server.commands.Command import Command


class Executor:
    _current_command: Command
    _last_command: Command
    _current_client: ClientDescriptor
    _last_client_address: str

    def set_last_client_address(self, address):
        self._last_client_address = address

    def set_current_client(self, current_client: ClientDescriptor):
        self._current_client = current_client

    def execute(self):
        self._current_command.perform(self._current_client)

        self._last_command = self._current_command
