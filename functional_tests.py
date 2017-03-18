import argparse
import sys
from junit_xml import TestSuite, TestCase
from testing.telnet_test import *
from testing.test_runner import *

tests = [ TelnetConnectionTest(), TelnetUptimeCommandTest() ]

parser = argparse.ArgumentParser()
parser.add_argument("-l", "--testlist", help="prints list of available tests",
                    action="store_true")
parser.add_argument("-t", "--test", help= "run selected test")

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

runner = TestRunner()
runner.run(tests)
results = runner.getResults()

test_cases = []
for key in results:
    tc = TestCase(key, 'hub.linux.bot', 0, '', '')
    
    if results[key] is False:
        tc.add_failure_info('test failed')    

    test_cases.append(tc)

suite = TestSuite("hub test suite", test_cases)

with open('functional_tests.xml', 'w') as result_file:
    TestSuite.to_file(result_file, [suite], prettyprint=False)

exit(0)

