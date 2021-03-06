
import sys
import os
import subprocess
import time

CONST_PATH_TO_TEST_DIR = "/home/frocbot/FRoC_TEST_PACKAGE/"
CONST_PATH_TO_FRoC_DIR = "/home/frocbot/buildbot/worker/FRoC_build/build/"

PATH_TO_FRoC = CONST_PATH_TO_FRoC_DIR + "multiple_paths/build"
PATH_TO_META = CONST_PATH_TO_FRoC_DIR + "meta_parser"



def buildFRoC():



	print "Building FRoC"

	i = 0;
	os.chdir(PATH_TO_FRoC)
	i = subprocess.call(["make"], shell=True)

	if(i != 0):
		print "FRoC failed to build, exiting test"
		sys.exit(1)
	print "FRoC Built"

	print "Building Meta Parser"
	os.chdir(PATH_TO_META)
	i = i + subprocess.call(["make"], shell=True)

	if(i != 0):
		print "Meta Parser failed to build, exiting test"
		sys.exit(1)
	print "Meta Parser Built"



def runTest(timingFileName, timingFile, runSim, runRP):

	OUTPUT_FILE =  CONST_PATH_TO_TEST_DIR + "/Test_Reports/" + timingFileName + ".test_report.txt"

	subprocess.call(["echo FRoC Test Report >" + OUTPUT_FILE], shell = True)




	#Run meta parser on the given Timing Report


	print "running meta-parser on Timing Report"

	subprocess.call(["printf '\n\nRunning meta-parser on Timing Report\n\n' &>>" + OUTPUT_FILE], shell = True)
	os.chdir(PATH_TO_META)
	i = subprocess.call(["./generate " + timingFile + " placement routing >>" + OUTPUT_FILE], shell = True)
	if(i != 0):
		print "Meta Parser failure, exiting test"
		#sys.exit(1)

	else: 
		print "Meta Parser has run successfully"

	subprocess.call(["cp " + PATH_TO_META + "/placement " + CONST_PATH_TO_TEST_DIR + "placement"], shell = True)
	subprocess.call(["cp " + PATH_TO_META + "/routing " + CONST_PATH_TO_TEST_DIR + "routing"], shell = True)



	#Run FRoC on the outputs of the meta parser
	print "running FRoC"
	subprocess.call(["printf '\n\nRunning FRoC\n\n' >>" + OUTPUT_FILE], shell = True)
	os.chdir(CONST_PATH_TO_TEST_DIR)
	i = subprocess.call([PATH_TO_FRoC + "/main -fp " + PATH_TO_META + "/placement -fr " + CONST_PATH_TO_TEST_DIR + "/routing -o FRoCOut >>" + OUTPUT_FILE], shell = True)

	if(i != 0):
		print "FRoC failure, exiting test"
		sys.exit(1)
	else:
		print "FRoC has run successfully"

	#call shell script to move all the files to their proper directories
	subprocess.call([CONST_PATH_TO_TEST_DIR + "/Scripts/runFRoC " + CONST_PATH_TO_TEST_DIR], shell = True)

	if runRP:
		#Create the FRoC quartus project & build
		#~/altera/15.1/quartus/bin/quartus_sh -t./routing.tcl &> ~/Documents/compile_output.txt

		os.chdir(CONST_PATH_TO_TEST_DIR + "/FRoC_Project")

		subprocess.call([CONST_PATH_TO_TEST_DIR + "Scripts/newFRoCProject " + CONST_PATH_TO_TEST_DIR], shell = True)

		print "Done FRoC Project Compilation"

		#parse compilation file for any problems
		subprocess.call(["printf '\n\nParsing Compilation File\n\n' >>" + OUTPUT_FILE], shell = True)

		i = subprocess.call([CONST_PATH_TO_TEST_DIR + "Tests/CompilationParser/compileparser " + CONST_PATH_TO_TEST_DIR + "compile_output.txt >>" + OUTPUT_FILE], shell = True)

		if(i != 0):
			print "FRoC project failed to compile, exiting test"
			sys.exit(1)
		else:
			print "FRoC compiled properly, check output files for any warnings"



		#if the project compiled properly, get the timing report
		subprocess.call(["printf '\n\nRetrieving Timing Report\n\n' >>" + OUTPUT_FILE], shell = True)
		subprocess.call([CONST_PATH_TO_TEST_DIR + "Scripts/FRoCTiming " + CONST_PATH_TO_TEST_DIR + " >>" + OUTPUT_FILE], shell = True)

		#Run meta parser on the new Timing Report

		subprocess.call(["printf '\n\nRunning meta-parser on FRoC Timing Report\n\n' >>" + OUTPUT_FILE], shell = True)
		print "running meta-parser on FRoC Timing Report"
		os.chdir(PATH_TO_META)
		i = subprocess.call(["./generate " + CONST_PATH_TO_TEST_DIR + "FRoC_timing" + " FRoCplacement FRoCrouting >>" + OUTPUT_FILE], shell = True)
		if(i != 0):
			print "Meta Parser failure, exiting test"
			#sys.exit(1)

		else: 
			print "Meta Parser has run successfully"

		subprocess.call(["mv " + PATH_TO_META + "/FRoCplacement " + CONST_PATH_TO_TEST_DIR + "FRoCplacement"], shell = True)
		subprocess.call(["mv " + PATH_TO_META + "/FRoCrouting " + CONST_PATH_TO_TEST_DIR + "FRoCrouting"], shell = True)

		#Get the number of paths

		numPaths = subprocess.call([CONST_PATH_TO_TEST_DIR + "Tests/PathNumber/pathcount " + CONST_PATH_TO_TEST_DIR + "routing_summary.txt"], shell = True)
		paths = str(numPaths)

		#Run the placement parser
		print "running placement parser"
		subprocess.call(["printf '\n\nRunning Placement Parser\n\n' >>" + OUTPUT_FILE], shell = True)
		i = subprocess.call([CONST_PATH_TO_TEST_DIR + "Tests/PlacementParser/placementparser " + paths + " " + CONST_PATH_TO_TEST_DIR + "placement " + CONST_PATH_TO_TEST_DIR + "FRoCplacement >>" + OUTPUT_FILE], shell = True)

		if(i == 0):
			print "enough placement matches found"
		else:
			print ["%d matches not found" , i]

		print "placement parser done"

		#Run the routing parser
		print "running routing parser"
		subprocess.call(["printf '\n\nRunning Routing Parser\n\n' >>" + OUTPUT_FILE], shell = True)
		i = subprocess.call([CONST_PATH_TO_TEST_DIR + "Tests/RoutingParser/routingparser " + paths + " " + CONST_PATH_TO_TEST_DIR + "routing " + CONST_PATH_TO_TEST_DIR + "FRoCrouting >>" + OUTPUT_FILE], shell = True)

		if(i == 0):
			print "enough routing matches found"
		else:
			print ["%d matches not found" , i]

		print "routing parser done"


	if runSim:
		#Run the DUT Maker
		print "making test bench"
		subprocess.call([CONST_PATH_TO_TEST_DIR + "Tests/DUTMaker/dutmaker " + CONST_PATH_TO_TEST_DIR + "FRoC_Project/top.v " + CONST_PATH_TO_TEST_DIR + "FRoC_Project/controller.v " + CONST_PATH_TO_TEST_DIR + "FRoC_Project/DUT.sv " + CONST_PATH_TO_TEST_DIR + "FRoC_Project/constants.sv " + CONST_PATH_TO_TEST_DIR + "Info_Out/constraints.txt " + CONST_PATH_TO_TEST_DIR + "Info_Out/labels.txt"], shell = True)

		subprocess.call(["cp " + CONST_PATH_TO_TEST_DIR + "Reference_Files/SimFiles/Controller.sv " + CONST_PATH_TO_TEST_DIR + "FRoC_Project/Controller.sv"], shell = True)

		subprocess.call(["cp " + CONST_PATH_TO_TEST_DIR + "Reference_Files/SimFiles/Interface.sv " + CONST_PATH_TO_TEST_DIR + "FRoC_Project/Interface.sv"], shell = True)

		subprocess.call(["cp " + CONST_PATH_TO_TEST_DIR + "Reference_Files/SimFiles/test_top.sv " + CONST_PATH_TO_TEST_DIR + "FRoC_Project/test_top.sv"], shell = True)

		subprocess.call(["cp " + CONST_PATH_TO_TEST_DIR + "Reference_Files/SimFiles/test_top.do " + CONST_PATH_TO_TEST_DIR + "FRoC_Project/test_top.do"], shell = True)
		print "test bench done"

		#Run Simulation 
		print "running simulation"
		os.chdir(CONST_PATH_TO_TEST_DIR + "FRoC_Project/")
		i = subprocess.call(["vsim -do test_top.do -c >/dev/null"], shell = True)

		if(i != 0):
			print "Simulation failed, exiting"
			sys.exit(1)
		else:
			print "done simulation"

		#Run Simulation Parser
		print "parsing simulation"
		subprocess.call(["printf '\n\nParsing Simulation\n\n' >>" + OUTPUT_FILE], shell = True)
		i = subprocess.call([CONST_PATH_TO_TEST_DIR + "Tests/SimulationParser/simparser " + CONST_PATH_TO_TEST_DIR + "FRoC_Project/output.txt " + CONST_PATH_TO_TEST_DIR + "Info_Out/constraints.txt >>" + OUTPUT_FILE], shell = True)

		if(i == 7):
			print "Wires failed to change, multiple LUT inputs changed in a single cycle, and the error flag was raised, see ", OUTPUT_FILE, " for details"
		elif(i == 6):
			print "Wires failed to change and multiple LUT inputs changed in a single cycle, see ", OUTPUT_FILE, " for details"
		elif(i == 5):
			print "Wires failed to change and the error flag was raised, see ", OUTPUT_FILE, " for details"
		elif(i == 4):
			print "Multiple LUT inputs changed in a single cycle, and the error flag was raised, see ", OUTPUT_FILE, " for details"
		elif(i == 3):
			print "Multiple LUT inputs changed in a single cycle, see  ", OUTPUT_FILE, " for details"
		elif(i == 2):
			print "Wires failed to change, see ", OUTPUT_FILE, " for details"
		elif(i == 1):
			print "The error flag was raised, see ", OUTPUT_FILE, " for details"
		else:
			print "No errors found in simulation"
		print "done parsing simulation"





def runAllTests(runSim, runRP):
	files = CONST_PATH_TO_TEST_DIR + "Timing_Files/"
	for f in os.listdir(files):
		currFile = files + f
		print currFile
		runTest(f, currFile, runSim, runRP)


def runSpecificTest(fileName, runSim, runRP):
	file_path = CONST_PATH_TO_TEST_DIR + "Timing_Files/" + fileName
	print file_path
	runTest(fileName, file_path, runSim, runRP)











		
	
	
