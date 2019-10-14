import pickle
from shared.Consts import HEADER_SIZE


def compose_message(data: dict):
    message = pickle.dumps(data)
    message = bytes(f"{len(message):<{HEADER_SIZE}}", 'utf-8') + message

    return message
