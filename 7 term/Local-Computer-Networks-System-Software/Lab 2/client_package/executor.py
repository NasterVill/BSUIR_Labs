import socket
from shared.commands import Commands
from client_package.commands.command import Command
from client_package.errors.invalid_command import InvalidCommand
from client_package.commands.tcp.echo_command import EchoCommand
from client_package.commands.tcp.time_command import TimeCommand
from client_package.commands.tcp.connect_command import ConnectCommand
from client_package.commands.tcp.disconnect_command import DisconnectCommand
from client_package.commands.tcp.upload_command import UploadCommand
from client_package.commands.tcp.download_command import DownloadCommand
from client_package.commands.udp.echo_command import UdpEchoCommand
from client_package.commands.udp.time_command import UdpTimeCommand
from client_package.commands.udp.download_command import UdpDownloadCommand
from client_package.commands.udp.upload_command import UdpUploadCommand
from client_package.commands.help_command import HelpCommand


class Executor:
    _command: Command
    _connection: socket.socket
    _udp_socket: socket.socket
    _set_client_connection: callable

    def __init__(self, udp_socket: socket.socket, set_client_connection: callable):
        self._set_client_connection = set_client_connection
        self._udp_socket = udp_socket
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
        elif command_name == Commands.UDP_ECHO.value:
            self._command = UdpEchoCommand(params, self._udp_socket)
        elif command_name == Commands.UDP_TIME.value:
            self._command = UdpTimeCommand(params, self._udp_socket)
        elif command_name == Commands.UDP_DOWNLOAD.value:
            self._command = UdpDownloadCommand(params, self._udp_socket)
        elif command_name == Commands.UDP_UPLOAD.value:
            self._command = UdpUploadCommand(params, self._udp_socket)
        elif command_name == Commands.HELP.value:
            self._command = HelpCommand()
        else:
            raise InvalidCommand
