from client_package.commands.command import Command


class HelpCommand(Command):
    def execute(self):
        print('All the params are marked with \'@\' symbol', '\n')
        print('List of supported commands:')
        print('connect @ip:@port        -- connects to a server_package by a given address')
        print('disconnect               -- disconnects client_package from the server_package')
        print('echo @text               -- send @text to server_package, receive it back and print out')
        print('time                     -- returns current time from the server_package')
        print('upload @path_to_file     -- upload the given file to a server_package')
        print('download @path_to_file   -- download the given file from a server_package')
        print('\n')
