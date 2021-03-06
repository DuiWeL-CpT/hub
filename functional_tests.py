import argparse
import random
import sys
from junit_xml import TestSuite, TestCase
from testing.mobile_test import *
from testing.telnet_test import *
from testing.telnet_command_test import *
from testing.test_runner import *
from testing.lib.environment import Environment

tests = [ TelnetConnectionTest(), TelnetUptimeCommandTest(), TelnetUptimeCommandAfterLoginTest(), TelnetTalkWithOtherSessionTest(), TelnetNotReadingTest(), 
    TelnetHelpLoginCommandTest(), TelnetHelpUptimeCommandTest(), TelnetHelpCommandSelfTest(), TelnetHelpCommandNoArgTest(), TelnetHelpCommandTooManyArgsTest(), 
    TelnetUnknownCommandTest(), TelnetUnknownCommandWithArgTest(), TelnetLoginNoArgTest(), TelnetLoginTooLessArgTest(), TelnetLoginTooManyArgTest(), 
    TelnetLoginWrongUserTest(), TelnetLoginWrongPasswordTest(), TelnetLoginAccessGrantedTest(), TelnetLsTest(), TelnetLogoutCommandTest(), 
    TelnetLogoutCommandAfterLoginTest(), TelnetWhoamiCommandTest(), TelnetWhoamiCommandAfterLoginTest(),
    MobileConnectionTest(), MobileHandshakeTest(), MobileUnknownMessageTest(), MobileRandomBytesTest()
    ]

parser = argparse.ArgumentParser()
parser.add_argument("-l", "--testlist", help="prints list of available tests",
                    action="store_true")
parser.add_argument("-t", "--test", help= "run selected test")
parser.add_argument("-r", "--random", type=int, help= "run tests in random order with seed (seed==0 will generate seed)")

args = parser.parse_args()
if args.testlist:
    print("Available tests:")
    for test in tests:
        print " " + test.getName()
    exit(0)

if args.test:
    found = False
    for key in tests:
        if key.getName() == args.test:
            tests = [ key ]
            found = True
            break

    if found is False:
        print "Test " + args.test + " not found"
        exit(1)

if args.random is not None:
    seed = args.random
    if seed == 0:
        seed = random.randint(1, 1024)
    
    random.seed(seed)
    print "Used seed: %d" % seed
    random.shuffle(tests)


runner = TestRunner()

Environment.use_ssl = False
runner.run(tests, "no_ssl_")
results_no_ssl = runner.getResults()

ssl_tests = []
for test in tests:
    if test.test_with_ssl is True:
        ssl_tests.append(test)

Environment.use_ssl = True
runner.run(ssl_tests, "ssl_")
results_with_ssl = runner.getResults()

results = results_no_ssl.copy()
results.update(results_with_ssl)

passed = 0
test_cases = []
for key in results:
    tc = TestCase(key, 'hub.linux.bot', 0, '', '')
    
    if results[key] is False:
        tc.add_failure_info('test failed')
    else:
        passed = passed + 1

    test_cases.append(tc)

suite = TestSuite("hub test suite", test_cases)

with open('functional_tests.xml', 'w') as result_file:
    TestSuite.to_file(result_file, [suite], prettyprint=False)

print "%d of %d passed" % (passed, len(results))
exit(0)

