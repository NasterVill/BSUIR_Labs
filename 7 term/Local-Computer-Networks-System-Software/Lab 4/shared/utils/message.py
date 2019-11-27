import pickle
import socket
from shared.consts import HEADER_SIZE, PACKET_SIZE, MAX_PROBES


def compose_message(data: dict):
    message = pickle.dumps(data)
    message = bytes(f"{len(message):<{HEADER_SIZE}}", 'utf-8') + message

    return message


def get_message(connection, packet_size=PACKET_SIZE):
    new_message = True
    message = b''
    message_len = 0
    probes = 0
    while True:
        try:
            temp_message = connection.recv(packet_size)

            # if the client_package has disconnected
            if not temp_message:
                return None

            # start processing new message
            if new_message:
                message_len = int(temp_message[:HEADER_SIZE])
                temp_message = temp_message[HEADER_SIZE:]
                new_message = False

            message += temp_message

            # print('get message', len(message), message_len, len(message) == message_len)

            # if we got whole message
            if len(message) == message_len:
                return pickle.loads(message)

        except socket.timeout:
            # allow client_package to send data / reconnect for MAX_PROBES times
            if probes <= MAX_PROBES:
                probes += 1

                continue
            else:
                raise socket.timeout
