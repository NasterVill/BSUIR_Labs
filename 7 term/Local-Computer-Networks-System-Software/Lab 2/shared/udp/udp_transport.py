import pickle
import socket
import select
from typing import List, Tuple
from shared.udp.packet import Packet, Acknowledgment
from shared.consts import UDP_MAX_PACKET_SIZE, WINDOWS_SIZE
from shared.udp.checksum import checksum
from shared.utils.console import progress


def send(sock: socket.socket, init_address: Tuple[str, int], packets: List[Packet], window_size: int = 0, show_progress: bool = False):
    if window_size == 0:
        window_size = len(packets) if len(packets) < WINDOWS_SIZE else WINDOWS_SIZE

    length = len(packets)
    timeout = 0.4
    sequence_number = 0
    unacknowledged_number = 0

    while sequence_number < length:
        try:
            ready = select.select([sock], [], [], timeout)
            if ready[0]:
                ack_data, address = sock.recvfrom(UDP_MAX_PACKET_SIZE)
                ack_data: Acknowledgment = pickle.loads(ack_data)
                if ack_data.ack_field == 0b1010101010101010:
                    if ack_data.sequence_number == sequence_number:
                        sequence_number += 1
                        unacknowledged_number -= 1
                        if show_progress:
                            progress(ack_data.sequence_number, length)
                    elif ack_data.sequence_number > sequence_number:
                        # in case client received part of the window
                        # but his ack was dropped (due to disconnect)
                        sequence_number = ack_data.sequence_number
                        unacknowledged_number = 0
                    else:
                        # window has probably been dropped
                        unacknowledged_number = 0
            else:
                # timed out
                unacknowledged_number = 0
            if unacknowledged_number < window_size and (unacknowledged_number + sequence_number) < length:
                sock.sendto(pickle.dumps(packets[unacknowledged_number + sequence_number]), init_address)
                unacknowledged_number += 1
                continue
        except socket.error:
            continue

    if show_progress:
        progress(length, length)
        print('\n')


def receive(sock: socket.socket, initial_address: Tuple[str, int] = ('', ''), file_name: str = ''):
    expected_sequence = 0
    prev_sequence = 0
    timeout = 0.2

    # just to count bytes
    sock.settimeout(timeout)

    address = ('127.0.0.1', 0)

    is_finished = False
    stub = 0
    full_data = b''
    try:
        while True:
            try:
                data, address = sock.recvfrom(UDP_MAX_PACKET_SIZE)

                if initial_address and address != address:
                    continue
                elif initial_address is ('', ''):
                    initial_address = address

                data: Packet = pickle.loads(data)
                if prev_sequence == data.sequence_number and is_finished:
                    # skip first ack in case sender just didn't catch up with the ack
                    if stub == 0:
                        stub += 1
                        continue
                    ack = Acknowledgment(prev_sequence)
                    sock.sendto(pickle.dumps(ack), address)
                    continue
                if expected_sequence > data.sequence_number:
                    # in case client received part of the window
                    # but his ack was dropped (due to disconnect)
                    ack = Acknowledgment(expected_sequence)
                    sock.sendto(pickle.dumps(ack), address)
                    continue
                elif expected_sequence < data.sequence_number:
                    # packet loss happened
                    continue
                if data.checksum != checksum(data.packet):
                    continue

                prev_sequence = data.sequence_number

                if file_name is not '':
                    file = open(file_name, 'ab')

                    file.write(data.packet)

                    file.close()
                else:
                    full_data += data.packet

            except socket.timeout:
                if is_finished:
                    break
                else:
                    continue

            except socket.error:
                continue

            ack = Acknowledgment(data.sequence_number)
            sock.sendto(pickle.dumps(ack), address)
            if data.fin:
                is_finished = True
            expected_sequence += 1

    except Exception as error:
        print(f'{error}')

    return full_data, address

