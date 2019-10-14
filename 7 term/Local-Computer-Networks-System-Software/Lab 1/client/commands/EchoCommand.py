from client.commands.Command import Command


class EchoCommand(Command):
    def __init__(self, params: list, connection: socket.socket):
        data = ' '.join(params)

    def execute(self):
        pass
