from shared.Utils.Ip import get_local_ip_address
from shared.Consts import HEADER_SIZE
import pickle



def main():
    print(2222, get_local_ip_address())

    msg = pickle.dumps('')
    msg = bytes(f"{len(msg):<{HEADER_SIZE}}", 'utf-8') + msg


if __name__ == '__main__':
    main()