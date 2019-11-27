from server_package.commands.tcp.echo_command import EchoCommand
from server_package.commands.tcp.time_command import TimeCommand
from server_package.commands.tcp.download_command import DownloadCommand
from server_package.commands.tcp.upload_command import UploadCommand
from server_package.commands.udp.echo_command import UdpEchoCommand
from server_package.commands.udp.time_command import UdpTimeCommand
from server_package.commands.udp.download_command import UdpDownloadCommand
from server_package.commands.udp.upload_command import UdpUploadCommand
from shared.errors.invalid_message import InvalidMessageError
from shared.commands import Commands
import pickle as pk
import socket


def build_command(message: bytes):
    parsed_message = pk.loads(message)

    if type(parsed_message['command']) == str:
        command_type = parsed_message['command'].upper()

        if command_type == Commands.ECHO.value:
            return EchoCommand(parsed_message)
        elif command_type == Commands.TIME.value:
            return TimeCommand()
        elif command_type == Commands.UPLOAD.value:
            return UploadCommand(parsed_message)
        elif command_type == Commands.DOWNLOAD.value:
            return DownloadCommand(parsed_message)

    else:
        raise InvalidMessageError


def build_udp_command(sock: socket.socket, message: bytes, address):
    parsed_message = pk.loads(message)

    if type(parsed_message['command']) == str:
        command_type = parsed_message['command'].upper()

        if command_type == Commands.UDP_ECHO.value:
            return UdpEchoCommand(sock, address)
        elif command_type == Commands.UDP_TIME.value:
            return UdpTimeCommand(sock, address)
        elif command_type == Commands.UDP_UPLOAD.value:
            return UdpUploadCommand(sock, address, parsed_message)
        elif command_type == Commands.UDP_DOWNLOAD.value:
            return UdpDownloadCommand(sock, address, parsed_message)

    else:
        raise InvalidMessageError

