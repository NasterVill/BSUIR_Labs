from client.Executor import Executor
from client.Errors.InvalidCommand import InvalidCommand
import sys


class Client:
    _executor: Executor

    def __init__(self):
        self._executor = Executor()

    def work(self):
        print('Client application is up and running. Enter \'help\' to get familiar wit the list of commands')

        while True:
            command = input()

            try:
                self._executor.build_command(command)

                self._executor.execute()

            except InvalidCommand:
                print('Provided command is invalid. Type \'help\' to get info about supported commands')

                continue

            except Exception as error:
                print(f'Unexpected error caught! {error}')

                sys.exit(1)
