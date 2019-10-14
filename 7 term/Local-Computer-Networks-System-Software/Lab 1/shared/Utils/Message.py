import pickle
from shared.Consts import HEADER_SIZE


def compose_message(data: dict):
    message = pickle.dumps(data)
    message = bytes(f"{len(message):<{HEADER_SIZE}}", 'utf-8') + message

    return message


def get_message(connection, packet_size):
    new_message = True
    full_message = b''
    message_len = 0
    while True:
        temp_message = connection.recv(packet_size)

        # if the client has disconnected
        if not temp_message:
            return None

        # start processing new message
        if new_message:
            message_len = int(temp_message[:HEADER_SIZE])
            new_message = False

        full_message += temp_message

        # if we got whole message
        if len(full_message) - HEADER_SIZE == message_len:
            return pickle.loads(full_message[HEADER_SIZE:])
