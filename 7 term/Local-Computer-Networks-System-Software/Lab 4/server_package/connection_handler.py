import socket
import threading
from server_package.executor import Executor
from server_package.client_descriptor import ClientDescriptor
from shared.errors.invalid_message_error import InvalidMessageError
from shared.errors.disconnected_exception import DisconnectedException
from shared.consts import PACKET_SIZE
from shared.utils.message import get_message


def listen(current_client: ClientDescriptor, dispose_of_connection: callable, mutex: threading.Lock):
    executor = Executor(current_client, mutex)

    while True:
        try:
            message = _get_message(current_client.connection)

            executor.build_command(message)

            executor.execute()

        except InvalidMessageError:
            mutex.acquire()
            print('Invalid message received! Waiting for a new one')
            mutex.release()

            continue

        except FileNotFoundError:
            mutex.acquire()
            print('Such file doesn\'t exist. Please try another one')
            mutex.release()

            continue

        # handle case with tcp keepalive timeout and
        # if client_package has disconnected softly during command execution
        except socket.timeout:
            _close_connection(current_client.connection, dispose_of_connection, mutex)

            mutex.acquire()
            print(f'Connection has timed out! Client {current_client.ip_address} was disconnected')
            mutex.release()

            break

        except DisconnectedException:
            _close_connection(current_client.connection, dispose_of_connection, mutex)

            mutex.acquire()
            print(
                'Connection closed by client_package. ' +
                f'{current_client.ip_address} has disconnected from the server'
            )
            mutex.release()

            break

        except Exception as error:
            _close_connection(current_client.connection, dispose_of_connection, mutex)

            mutex.acquire()
            print(f'Unexpected error! {error}')
            mutex.release()

            break


def _get_message(connection: socket.socket):
    message = get_message(connection, PACKET_SIZE)

    if not message:
        raise DisconnectedException
    else:
        return message


def _close_connection(connection: socket.socket, dispose_of_connection: callable, mutex: threading.Lock):
    mutex.acquire()
    dispose_of_connection(connection)
    mutex.release()
    
    connection.close()
