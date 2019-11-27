import pickle
import socket
from shared.consts import HEADER_SIZE, PACKET_SIZE, MAX_PROBES


def compose_message(data: dict):
    message = pickle.dumps(data)
    message = bytes(f"{len(message):<{HEADER_SIZE}}", 'utf-8') + message

    return message


def get_message(connection: socket.socket, size: int, packet_size=PACKET_SIZE):
    message = b''
    probes = 0

    while True:
        try:
            message += connection.recv(packet_size)

            if len(message) < size:
                continue
            else:
                return message

        except socket.timeout:
            # allow client to send data / reconnect for MAX_PROBES times
            if probes <= MAX_PROBES:
                probes += 1

                continue
            else:
                raise socket.timeout
