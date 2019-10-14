from server.ClientDescriptor import ClientDescriptor
from server.commands.Command import Command
from server.commands.EchoCommand import EchoCommand
from server.commands.TimeCommand import TimeCommand
from server.commands.DownloadCommand import DownloadCommand
from server.commands.UploadCommand import UploadCommand
from shared.Errors.InvalidMessageError import InvalidMessageError
from shared.Commands import Commands


class Executor:
    _current_command: Command
    _last_command: Command
    _current_client: ClientDescriptor

    def set_current_client(self, current_client: ClientDescriptor):
        self._current_client = current_client

    def execute(self):
        try:
            self._current_command.execute()
        except Exception as error:
            raise error
        finally:
            self._last_command = self._current_command

    def build_command(self, message: dict):
        if type(message['type']) == str:
            command_type = message['type'].upper()

            if command_type == Commands.ECHO.value:
                self._current_command = EchoCommand(message, self._current_client)
            elif command_type == Commands.TIME.value:
                self._current_command = TimeCommand(self._current_client)
            elif command_type == Commands.UPLOAD.value:
                self._current_command = UploadCommand(message, self._current_client, self._last_command)
            elif command_type == Commands.DOWNLOAD.value:
                self._current_command = DownloadCommand(message, self._current_client, self._last_command)

        else:
            raise InvalidMessageError
