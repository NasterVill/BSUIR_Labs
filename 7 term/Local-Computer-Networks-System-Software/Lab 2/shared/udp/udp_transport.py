import pickle
import socket
import select
import math
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
            if unacknowledged_number < window_size and (unacknowledged_number + sequence_number) < length:
                # dummy packet
                packet_to_send = Packet(0, b'', 0, False)

                for packet in packets:
                    if packet.sequence_number == unacknowledged_number + sequence_number:
                        packet_to_send = packet

                sock.sendto(pickle.dumps(packet_to_send), init_address)
                unacknowledged_number += 1
                continue
            else:
                ready = select.select([sock], [], [], timeout)
                if ready[0]:
                    ack_data, address = sock.recvfrom(UDP_MAX_PACKET_SIZE)
                    ack_data: Acknowledgment = pickle.loads(ack_data)
                    if ack_data.ack_field != 0b1010101010101010:
                        continue
                    if ack_data.sequence_number == sequence_number:
                        sequence_number += 1
                        unacknowledged_number -= 1
                        if show_progress:
                            progress(ack_data.sequence_number, length)
                        continue
                    else:
                        unacknowledged_number = 0
                        continue
                else:
                    # print('Timeout, sequence number = ', sequence_number)
                    unacknowledged_number = 0
                    continue
        except socket.error as error:
            continue

    if show_progress:
        progress(length, length)
        print('\n')

    # # handle second part of "connection" termination
    # fin_received = False
    # while True:
    #     ready = select.select([sock], [], [], timeout)
    #     if ready[0]:
    #         data, address = sock.recvfrom(UDP_MAX_PACKET_SIZE)
    #
    #         if address != init_address:
    #         continue
    #
    #         fin: Packet = pickle.loads(data)
    #
    #         if fin.fin:
    #             fin_received = True
    #
    #         sock.sendto(pickle.dumps(Acknowledgment(0)), init_address)
    #     elif fin_received:
    #         print('Timeout')
    #         break

    # print('Transfer end')


def receive(sock: socket.socket, initial_address: Tuple[str, int] = ('', ''), file_name: str = ''):
    expected_sequence = 0
    prev_sequence = 0
    timeout = 0.2

    sock.settimeout(timeout)

    is_finished = False
    address = ('127.0.0.1', 9092)

    stub = 0

    full_data = b''
    try:
        while True:
            try:
                # print('hi', expected_sequence)
                data, address = sock.recvfrom(UDP_MAX_PACKET_SIZE)

                if initial_address and address != address:
                    continue
                elif initial_address is ('', ''):
                    initial_address = address

                data: Packet = pickle.loads(data)
                if prev_sequence == data.sequence_number and is_finished:
                    # print('bbbb')
                    # skip first ack in case sender just didn't catch up with the ack
                    if stub == 0:
                        stub += 1
                        continue
                    ack = Acknowledgment(prev_sequence)
                    sock.sendto(pickle.dumps(ack), address)
                    continue
                if expected_sequence != data.sequence_number:
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
                # print('timeOut')
                if is_finished:
                    break
                else:
                    continue

            except socket.error:
                continue

            ack = Acknowledgment(data.sequence_number)
            sock.sendto(pickle.dumps(ack), address)
            # print('sent ack', data.sequence_number)
            if data.fin:
                # print('aaaaaa')
                is_finished = True
            expected_sequence += 1

        # print('terminating')

        # fin = Packet(0, b'', 0, True)
        # while True:
        #     try:
        #         sock.sendto(pickle.dumps(fin), address)
        #
        #         ack_data, address = sock.recvfrom(UDP_MAX_PACKET_SIZE)
        #
        #         if address != address:
        #             continue
        #
        #         ack_data: Acknowledgment = pickle.loads(ack_data)
        #
        #         if ack_data.ack_field == 0b1010101010101010:
        #             break
        #     except socket.timeout:
        #         continue

    except Exception as error:
        print(f'{error}')

    return full_data, address

