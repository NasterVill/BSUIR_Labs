import datetime
from server_package.commands.command import Command
import pickle as pk


class TimeCommand(Command):
    _data = ''
    is_finished = False

    def __init__(self):
        date = datetime.datetime.now()

        self._data = pk.dumps({'payload': date})

        print(f'Client requested current time. Time is {date}')

    def generate_message(self):
        data = self._data

        self._data = ''

        self.is_finished = True

        yield data
