import socket
from typing import List, Dict
from select import select
from server_package.commands.command import Command
from server_package.command_builder import build_command, build_udp_command
from shared.consts import TCP_SERVER_PORT, UDP_SERVER_PORT, UDP_MAX_PACKET_SIZE
from shared.consts import TCP_PACKET_SIZE, IS_LOCAL_HOST
from shared.utils import ip, socket as sock_utils
from shared.udp.udp_transport import receive
import pickle as pk
from shared.udp.compose_packets import compose_packets


class Server:
    MAX_ACTIVE_CLIENTS: int = 10
    TCP_PORT: int = TCP_SERVER_PORT
    UDP_PORT: int = UDP_SERVER_PORT

    # general socket used to listen for clients
    _tcp_main_sock: socket.socket
    _udp_sock: socket.socket

    # socket for specific connection
    _inputs: List[socket.socket] = []
    _outputs: List[socket.socket] = []
    _issued_commands: Dict[socket.socket, Command] = {}
    # dict : { connection: ip_address }
    _clients: Dict[socket.socket, str] = {}

    def __init__(self):
        self._init_sockets()

    def work(self):
        while True:
            try:
                readable, writable, exceptional = select(self._inputs, self._outputs, self._inputs)

                for sock in readable:
                    try:
                        # UDP segment
                        if sock is self._udp_sock:
                            message, address = receive(self._udp_sock)

                            command: Command = build_udp_command(self._udp_sock, message, address)

                            command.generate_message()
                            continue

                        # TCP segment
                        if sock is self._tcp_main_sock and len(self._inputs) <= self.MAX_ACTIVE_CLIENTS:
                            self._initialize_client()

                        else:
                            message = sock.recv(TCP_PACKET_SIZE)

                            if message:
                                if sock not in self._issued_commands:
                                    self._issued_commands[sock] = build_command(message)
                                else:
                                    self._issued_commands[sock].set_response(message)

                                if sock not in self._outputs:
                                    self._outputs.append(sock)
                            else:
                                self._free_socket(sock)

                                print(
                                    'Connection closed by client_package. ' +
                                    f'{self._clients[sock]} has disconnected from the server'
                                )

                    except FileNotFoundError:
                        print('Such file doesn\'t exist. Please try another one')

                    except socket.error as error:
                        if sock in self._clients:
                            print(self._clients[sock], error)

                            self._free_socket(sock)

                    except Exception as error:
                        print(f'Caught error: {error}')

                        continue

                for sock in writable:
                    message = next(self._issued_commands[sock].generate_message())

                    sock.send(message)

                    if self._issued_commands[sock].is_finished:
                        del self._issued_commands[sock]

                    self._outputs.remove(sock)

                for sock in exceptional:
                    print(f'Exception on {sock.getpeername()}')
                    self._free_socket(sock)

            except Exception as error:
                print(f'Unexpected error caught! {error}')

                break

    def _init_sockets(self):
        self._set_tcp_main_socket()
        self._set_udp_socket()

        print(f'Server created on address: {ip.get_local_ip_address(is_local_host=IS_LOCAL_HOST)}:{self.TCP_PORT}')

    def _set_tcp_main_socket(self):
        # creating socket that accepts IPv4 address and works with TCP protocol
        self._tcp_main_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._tcp_main_sock.bind((ip.get_local_ip_address(is_local_host=IS_LOCAL_HOST), self.TCP_PORT))
        self._tcp_main_sock.setblocking(False)
        self._inputs.append(self._tcp_main_sock)
        self._tcp_main_sock.listen(self.MAX_ACTIVE_CLIENTS)

    def _set_udp_socket(self):
        self._udp_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self._udp_sock.bind((ip.get_local_ip_address(is_local_host=IS_LOCAL_HOST), self.UDP_PORT))
        self._inputs.append(self._udp_sock)

    def _initialize_client(self):
        connection, address = self._tcp_main_sock.accept()

        connection.setblocking(False)
        sock_utils.set_socket_keep_alive(connection, keep_alive_time=5, keep_alive_interval=10, max_probes=10)
        self._inputs.append(connection)
        self._clients[connection] = address

        print(f'Client connected. Address {address}')

    def _free_socket(self, sock: socket.socket):
        if sock in self._inputs:
            self._inputs.remove(sock)
        if sock in self._outputs:
            self._outputs.remove(sock)
        if sock in self._issued_commands:
            del self._issued_commands[sock]
        if sock in self._clients:
            del self._clients[sock]

        sock.close()
