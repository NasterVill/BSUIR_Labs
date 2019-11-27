from server_package.commands.echo_command import EchoCommand
from server_package.commands.time_command import TimeCommand
from server_package.commands.download_command import DownloadCommand
from server_package.commands.upload_command import UploadCommand
from shared.errors.invalid_message import InvalidMessageError
from shared.commands import Commands
import pickle as pk


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
