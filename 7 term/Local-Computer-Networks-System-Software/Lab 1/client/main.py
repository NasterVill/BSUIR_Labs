from shared.Utils.Ip import get_local_ip_address
from shared.Utils.Message import compose_message


def main():
    print(get_local_ip_address())

    data = {'payload': 'hello, world!'}

    message = compose_message(data)


if __name__ == '__main__':
    main()
