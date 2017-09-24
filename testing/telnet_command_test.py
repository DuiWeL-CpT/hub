from telnet_test import *

class TelnetUptimeCommandTest(TelnetTest):
    def run(self):
        Test.run(self)
        self.connection.send(".uptime")
#        result = self.connection.expect([r".uptime: [0-9]+ hours, [0-9]+ minutes, [0-9]+ seconds, [0-9]+ milliseconds"])
        result = self.connection.expect([r"no access to run .uptime"])
        return result

class TelnetHelpCommandTest(TelnetTest):
    def run(self):
        Test.run(self)
        self.connection.send(".help .uptime")
        result = self.connection.expect([r".help: shows how long applications works"])
        return result

class TelnetHelpCommandSelfTest(TelnetTest):
    def run(self):
        Test.run(self)
        self.connection.send(".help .help")
        result = self.connection.expect([r".help: shows help info about command, i.e. .help <command>"])
        return result

class TelnetHelpCommandNoArgTest(TelnetTest):
    def run(self):
        Test.run(self)
        self.connection.send(".help")
        result = self.connection.expect([r".help: shows help info about command, i.e. .help <command>"])
        return result

class TelnetHelpCommandTooManyArgsTest(TelnetTest):
    def run(self):
        Test.run(self)
        self.connection.send(".help one two")
        result = self.connection.expect([r".help: wrong argument number"])
        return result

class TelnetUnknownCommandTest(TelnetTest):
    def run(self):
        Test.run(self)
        self.connection.send(".unknown")
        result = self.connection.expect([r".unknown: command does not exist!"])
        return result

class TelnetUnknownCommandWithArgTest(TelnetTest):
    def run(self):
        Test.run(self)
        self.connection.send(".unknown arg")
        result = self.connection.expect([r".unknown: command does not exist!"])
        return result