from server_package.client_descriptor import ClientDescriptor
from server_package.commands.command import Command
from server_package.commands.echo_command import EchoCommand
from server_package.commands.time_command import TimeCommand
from server_package.commands.download_command import DownloadCommand
from server_package.commands.upload_command import UploadCommand
from shared.errors.invalid_message_error import InvalidMessageError
from shared.commands import Commands


class Executor:
    _current_command: Command = Command()
    _current_client: ClientDescriptor

    def set_current_client(self, current_client: ClientDescriptor):
        self._current_client = current_client

    def execute(self):
        try:
            self._current_command.execute()
        except Exception as error:
            raise error

    def build_command(self, message: dict):
        if type(message['type']) == str:
            command_type = message['type'].upper()

            if command_type == Commands.ECHO.value:
                self._current_command = EchoCommand(message, self._current_client)
            elif command_type == Commands.TIME.value:
                self._current_command = TimeCommand(self._current_client)
            elif command_type == Commands.UPLOAD.value:
                self._current_command = UploadCommand(message, self._current_client)
            elif command_type == Commands.DOWNLOAD.value:
                self._current_command = DownloadCommand(message, self._current_client)

        else:
            raise InvalidMessageError
