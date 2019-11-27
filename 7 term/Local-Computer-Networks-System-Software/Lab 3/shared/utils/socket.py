import socket
import platform
import struct


def _set_keepalive_linux(
        sock: socket.socket,
        keep_alive_time: int = 1,
        keep_alive_interval: int = 3,
        max_probes: int = 5
):
    """
    Set TCP keepalive on an open socket.

    It activates after 1 second (keep_alive_time) of idleness,
    then sends a keepalive ping once every 3 seconds (keep_alive_interval),
    and closes the connection after 5 failed ping (max_probes), or 15 seconds
    """
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1)
    sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPIDLE, keep_alive_time)
    sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPINTVL, keep_alive_interval)
    sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPCNT, max_probes)


def _set_keepalive_osx(sock: socket.socket, keep_alive_interval: int = 3):
    """
    Set TCP keepalive on an open socket.

    sends a keepalive ping once every 3 seconds (keep_alive_interval)
    """
    # scraped from /usr/include, not exported by python's socket module
    TCP_KEEPALIVE = 0x10
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1)
    sock.setsockopt(socket.IPPROTO_TCP, TCP_KEEPALIVE, keep_alive_interval)


def _set_keepalive_windows(sock: socket.socket, keep_alive_time: int = 1, keep_alive_interval: int = 3, max_probes: int = 5):
    """
    Set TCP keepalive on an open socket.

    If keep alive is on (1)
    it activates after 1 second (keep_alive_time) of idleness,
    then sends a keepalive ping once every 3 seconds (keep_alive_interval),
    for 10 times, because for Windwos Vista and higher this number is
    fixed and can not be changed.

    All time params are measured in ms
    """
    second = 1000

    struct_format = 'hh' if platform.system() == 'Windows' else 'li'
    l_onoff = 1
    l_linger = 55
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_LINGER,
                    struct.pack(struct_format, l_onoff, l_linger))

    sock.ioctl(socket.SIO_KEEPALIVE_VALS, (1, keep_alive_time * second, keep_alive_interval * second))


def set_socket_keep_alive(
        sock: socket.socket,
        keep_alive_time: int = 10,
        keep_alive_interval: int = 3,
        max_probes: int = 10
):
    system = platform.system()

    if system == 'Windows':
        _set_keepalive_windows(sock, keep_alive_time, keep_alive_interval)
    elif system == 'Linux':
        _set_keepalive_linux(sock, keep_alive_time, keep_alive_interval, max_probes)
    elif system == 'MacOS':
        _set_keepalive_osx(sock, keep_alive_interval)
