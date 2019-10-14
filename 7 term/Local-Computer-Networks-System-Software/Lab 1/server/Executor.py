from server.ClientDescriptor import ClientDescriptor
from server.commands.Command import Command
from shared.Errors.InvalidMessageError import InvalidMessageError


class Executor:
    _current_command: Command
    _last_command: Command
    _current_client: ClientDescriptor

    def set_current_client(self, current_client: ClientDescriptor):
        self._current_client = current_client

    def execute(self):
        self._current_command.execute()

        self._last_command = self._current_command

    def build_command(self, message: dict):
        if message['type'] is str:
            command_type = message['type'].upper()

            if command_type == 'ECHO':
                self._current_command = EchoCommand(message, self._current_client)
            elif command_type == 'TIME':
                self._current_command = TimeCommand(message, self._current_client)
            elif command_type == 'CLOSE':
                self._current_command = DisconnectCommand(message, self._current_client)
            elif command_type == 'UPLOAD':
                self._current_command = UploadCommand(message, self._current_client, self._last_command)
            elif command_type == 'DOWNLOAD':
                self._current_command = DownloadCommand(message, self._current_client, self._last_command)

        else:
            raise InvalidMessageError
