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
    UDP_ECHO = 'UDP_ECHO'
    UDP_TIME = 'UDP_TIME'
    UDP_DOWNLOAD = 'UDP_DOWNLOAD'
    UDP_UPLOAD = 'UDP_UPLOAD'
