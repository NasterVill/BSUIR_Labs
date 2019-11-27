class Packet:
    def __init__(self, sequence_number: int, packet: bytes, checksum: int, fin: bool = False):
        self.sequence_number = sequence_number
        self.data_id = 0b0101010101010101
        self.packet = packet
        self.checksum = checksum
        self.fin = fin


class Acknowledgment:
    def __init__(self, sequence_number):
        self.sequence_number = sequence_number
        self.zeros = 0
        self.ack_field = 0b1010101010101010
