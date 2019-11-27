import socket


def get_local_ip_address(is_local_host: bool = False):
    local_host = '127.0.0.1'

    if is_local_host:
        return local_host

    else:
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        try:
            # doesn't even have to be reachable
            sock.connect(('10.255.255.255', 1))
            ip = sock.getsockname()[0]
        except:
            ip = local_host
        finally:
            sock.close()

        return ip
