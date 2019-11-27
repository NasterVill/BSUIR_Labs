from enum import Enum


class Commands(Enum):
    ECHO = 'ECHO'
    TIME = 'TIME'
    CONNECT = 'CONNECT'
    DISCONNECT = 'DISCONNECT'
    UPLOAD = 'UPLOAD'
    DOWNLOAD = 'DOWNLOAD'
    HELP = 'HELP'
    QUIT = 'QUIT'
