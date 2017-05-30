#!/usr/bin/env python

import sys
import os
import subprocess
import time
from optparse import OptionParser
from testing_functions import buildFRoC
from testing_functions import runTest
from testing_functions import runAllTests
from testing_functions import runSpecificTest



parser = OptionParser()

parser.add_option("-o", "--output", action="store", type="string", dest="outputFileName", help="select a file to write info to")
parser.add_option("-a", "--all", action="store_true", dest="all", default=False, help="run all the available tests")
parser.add_option("-s", "--sim", "--simulation", action="store_true", dest="simulationOnly", default=False, help="Only run simulation tests")
parser.add_option("-p", "--rp", action="store_true", dest="rpOnly", default=False, help="Only run routing and placement tests")
parser.add_option("-b", "--build", action="store_true", dest="build", default=False, help="Build FRoC before testing")
parser.add_option("-f", "--file", action="store", type="string", dest="timingFileName", help="Location of timing file to test")
parser.add_option("-n", "--name", action="store", type="string", dest="timingName", help="name of timing file located in Timing_Files folder")


(options, sys.argv[1:]) = parser.parse_args()

if options.all and (options.timingFileName or options.timingName):
	parser.error("options -a, -f, and -n are mutually exclusive")
if options.timingFileName and (options.all or options.timingName):
	parser.error("options -a, -f, and -n are mutually exclusive")

if options.simulationOnly and options.rpOnly:
	parser.error("options -s and -p are mutually exclusive")

if options.outputFileName:
	#send print output to file
	sys.stdout = open(options.outputFileName, 'w')

if options.build:
	#build FRoC and meta parser
	buildFRoC()

if options.simulationOnly:
	#turn of rp option
	runRP = False
	runSim = True
elif options.rpOnly:
	#turn off sim option
	runRP = True
	runSim = False
else:
	#turn on both sim and rp options
	runRP = True
	runSim = True

if options.all:
	runAllTests(runSim, runRP)

elif options.timingFileName:
	runTest("FRoC", options.timingFileName, runSim, runRP)

elif options.timingName:
	runSpecificTest(options.timingName, runSim, runRP)






