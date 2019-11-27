from shared.udp.packet import Packet
from shared.udp.checksum import checksum
from shared.consts import UDP_MSS
from typing import List
import os


def compose_packets(data: bytes, mss: int = UDP_MSS):
    if mss > len(data):
        mss = len(data)

    sequence_number = 0

    length = len(data)
    packets: List[Packet] = []
    i = 0
    while i < length:
        if i + mss >= length:
            mss = length - i

        data_portion = data[i:i+mss]
        _checksum = checksum(data_portion)
        if i + mss == length:
            packets.append(
                Packet(sequence_number, data_portion, _checksum, True))
        else:
            packets.append(Packet(sequence_number, data_portion, _checksum, False))
        i += mss
        sequence_number += 1

    return packets


def compose_packets_from_file(file_name: str, mss: int = UDP_MSS):
    sequence_number = 0
    packets: List[Packet] = []
    with open(file_name, "rb") as file:
        i = 0
        size = os.path.getsize(file_name)
        while i <= size:
            file.seek(i)
            file_portion = file.read(mss)
            _checksum = checksum(file_portion)
            if i + mss > size:
                packets.append(
                    Packet(sequence_number, file_portion, _checksum, True))
            else:
                packets.append(Packet(sequence_number, file_portion, _checksum, False))
            i += mss
            sequence_number += 1
    return packets
