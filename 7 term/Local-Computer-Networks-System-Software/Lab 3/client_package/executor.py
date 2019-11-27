import socket
from shared.commands import Commands
from client_package.commands.command import Command
from client_package.errors.invalid_command import InvalidCommand
from client_package.commands.echo_command import EchoCommand
from client_package.commands.time_command import TimeCommand
from client_package.commands.connect_command import ConnectCommand
from client_package.commands.disconnect_command import DisconnectCommand
from client_package.commands.upload_command import UploadCommand
from client_package.commands.download_command import DownloadCommand
from client_package.commands.help_command import HelpCommand


class Executor:
    _command: Command
    _connection: socket.socket
    _set_client_connection: callable

    def __init__(self, set_client_connection: callable):
        self._set_client_connection = set_client_connection

        self._set_connection(socket.socket(socket.AF_INET, socket.SOCK_STREAM))

    def execute(self):
        self._command.execute()

    def _set_connection(self, connection: socket.socket):
        self._connection = connection

        self._set_client_connection(connection)

    def build_command(self, command: str):
        command_parts = command.split(' ')
        command_name = command_parts[0].upper()
        params = command_parts[1:]

        if command_name == Commands.ECHO.value:
            self._command = EchoCommand(params, self._connection)
        elif command_name == Commands.TIME.value:
            self._command = TimeCommand(params, self._connection)
        elif command_name == Commands.CONNECT.value:
            self._command = ConnectCommand(params, self._set_connection)
        elif command_name == Commands.DISCONNECT.value:
            self._command = DisconnectCommand(self._connection)
        elif command_name == Commands.UPLOAD.value:
            self._command = UploadCommand(params, self._connection)
        elif command_name == Commands.DOWNLOAD.value:
            self._command = DownloadCommand(params, self._connection)
        elif command_name == Commands.HELP.value:
            self._command = HelpCommand()
        else:
            raise InvalidCommand
