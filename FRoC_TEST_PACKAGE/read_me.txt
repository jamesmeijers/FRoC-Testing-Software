FRoC auto-tester Information


This FRoC auto-tester does the following:
	-Builds FRoC
	-Runs timing files through the FRoC system
	-Compiles a Quartus project from the results of FRoC
	-Checks Quartus' compilation output for pertinent errors or information
	-Gets a timing report for FRoC's circuit
	-Compares FRoC's critical paths to the project's (placement and routing),
	 looking for matches
	-Develops a Verilog Testbench for the FRoC project
	-Simulates the Testbench using Altera Modelsim
	-Parses the simulation output for three things
		-That all the signals change value at least twice
		-That the error flag is never raised in simulation
		-That no more than 1 non-control LUT input changes in a non-reset phase

Setting up the auto-tester
	-Decide on a location for the FRoC_TEST_PACKAGE folder
	-open .../FRoC_TEST_PACKAGE/Scripts/testing_functions.py and edit the
	 following:
		-CONST_PATH_TO_TEST_DIR should be set the the location and name you
		 selected for the test package folder
		-CONST_PATH_TO_FRoC_DIR should be set to the location of your FRoC
		 folder (containing multiple_paths and meta_parser folders)
		-CONST_PATH_TO_QUARTUS should be set to the folder which contains
		 Quartus' executables, e.g. .../bin/quartus_sh
		-CONST_PATH_TO_MODELSIM should be set to the folder which contains
		 Modelsim executables, e.g. .../linualoem/vsim
	-open .../FRoC_TEST_PACKAGE/Scripts/FRoCTiming and edit the script so that 
	 it runs the quartus_sta executable on your machine with the given command
	-open .../FRoC_TEST_PACKAGE/Scripts/newFRoCProject and edit the script so that
	 it runs the quartus_sh execuatable on your machine with the given command
	-cd into the folder .../FRoC_TEST_PACKAGE/source_code and run the script 
	 build-all-tests to build all the tests, then move each testing executable into
	 its respective folder in .../FRoC_TEST_PACKAGE/Tests
	-Ensure FRoC, Quartus, Modelsim, and Python are all properly installed on
	 your computer
	-Ensure all the shell scripts in the scripts folder are executable (e.g. run
	 "chmod +x scriptName" to make scriptName executable)
	-Ensure all the folders in the test package are present and properly named,
	 the following are mandatory for the tester to run:
		-Reference_Files --contains constant files required to build a FRoC
		 project and to simulate the project
		-Scripts --contains the main python script that runs the test as well
		 as other scripts (Python, shell, and tcl) that perform other functions
		-Test_Reports --contains reports for tests run by the program
		-Tests --contains the tests (written in C++) that are used to confirm
		 functionality
		-Timing_Files --contains Quartus timing reports for benchmarks that
		 can be used to test FRoC (optional but neccassary for -a option)
		-Test_Reports --an empty folder to start, the test result files are 
		 saved in this folder during testing
	-The following are optional files/folders
		-source_code --C++ projects used to create executables found in the
		 tests folder
		-read_me.txt --This document
	-Any files/folders in the test package folder not mentioned above are created
	 by the test function and can be safely deleted when the tester completes
	 without affecting future tests, these files include but are not limited to:
		-FRoC_Project --the most recent FRoC project created for testing
		-Info_Out --contains files outputted by FRoC and by the tests
			-edgeCoverage.txt, FRoCOut_stats.txt, pathCoverage.txt,
			 VerilogFile.txt --Outputs of FRoC
			-labels.txt --A list of wire names from FRoC and their
			 corresponding numerical IDs used in simulation
			-constraints.txt --A list of LUTs and the simulation numerical
			 IDs of their input wires
		-compile_output.txt --Quartus' output from its most recent
		 compilation, parsed by the tester for neccassary info, but left for
		 reference
		-placement, routing, FRoCplacement, FRoCrouting --placment and routing
		 files produced by meta-parser
		-FRoC_timing --The Quartus timing report for the most recent tested
		 project
	-Note that these files will most likely be deleted/replaced after each test,
	 so ensure that you save externally them if needed

Running the auto-tester
	-The auto-tester can by run from inside .../FRoC_TEST_PACKAGE/Scripts/ by
	 running the following command:
		python run_FRoC_tester.py
	-This will however do nothing, one or more arguments are needed to achieve
	 results, the arguements are below (brought up by -h option)
	  -h, --help            show this help message and exit
	  -o, --output          select a file to write info to, requires path to
				output file (file must exist)
	  -a, --all             run all the available tests
	  -s, --sim, 		Only run simulation tests
	  -p, --rp              Only run routing and placement tests
	  -b, --build           Build FRoC before testing
	  -f, --file            Location of timing file to test, requires path to file
	  -n, --name		name of timing file located in Timing_Files folder
	-One of -a, -n, and -f must be entered (but no more than one) for the test to run
		-a runs on every timing file in the timing files folder
		-f simply runs on the file entered (which can be located in the timing
		 files folder if desired)
		-n runs the file with the specified name located in the Timing_Files folder
	-Only one of -s and -p can be selected
		-s causes only the simulation to run (significantly faster)
		-p causes only the route and place check to run (much slower)
		-if neither argument is entered, both the simulation and route and
		 place check are run (about the same time as -p)
	-b builds FRoC before completing the other tests (if any tests are selected,
	 it can just build and exit if desired)
	-o outputs the tester status messages to a given text file 
		-this file doesn't contain very much information, but can be stored
		 for future reference
		-if all tests are run with this option, their outputs will be appended
		 after one another

Inspecting Results
	-If the testing tool reports an problem, you can examine its output files for
	 more information
		-If there is a compilation problem:
			-check .../Test_Reports/(testName OR FRoC).test_report.txt
			 under "Parsing Compilation File" for errors & place/route info
			-check .../compile_output for the full compilation output if
			 needed
		-If not enough path matches are found:
			-check .../Test_Reports/(testName OR FRoC).test_report.txt
			 under "Parsing Compilation File" place/route information
			 given by Quartus
			-check .../Test_Reports/(testName OR FRoC).test_report.txt
			 under "Running Placement Parser" for a list of paths with the
			 same placement
			-check .../Test_Reports/(testName OR FRoC).test_report.txt
			 under "Running Routing Parser" for a list of paths with the
			 same routing
			-check .../FRoCrouting, .../FRoCPlacement, .../routing, 
			 .../placement for more info
			-if no compilation problems are reported and still not enough 
			 matches are found, check to see what matches are found. The 
			 tool checks 2500 FRoC paths. If in the line in the test Report:
				FRoC path X matches circuit path Y
			 X approaches 2500, it may be that not enough FRoC paths were 
			 tested, in which case you can increase the number in 
			 .../Scripts/getTiming.tcl by -npaths until enough matches are found

		-If the simulation parser reports a problem:
			-check .../FRoC_Project/output.txt for the simulation output
			-check .../Info_Out/constraints.txt for a list of LUTs and
			 their inputs
			-check .../Info_Out/labels.txt to convert the numerical values
			 given the simulation parser output to names in the FRoC
			 project
			-if the issue is simply that some signals fail to change value twice,
			 the simulation may just need to run for longer, currently it is set 
			 to 10000 cycles. This can be increased in the DUTMaker source code by
			 setting CYCLES_TO_RUN to a new value. Then build the file and replace
			 the executable in the tests folder


Troubleshooting
	-If the project is building, but the simulation fails to run:
		-Look at the simulator output, it should be running all executables
		 from the .../linuxaloem/ director in the modelsim directory 
		-Due to what seems to be a modelsim error, it may be trying to run 
		 the file from a directory that doesn't exist, e.g. linuxap
		-This can be solved by either copying the contents of linuxaloem to 
		 a new folder of the name it is looking for, or by
		-Making the folder modelsim is looking for forward to linuxaloem
		-Both solutions work for this tester, though the second may be 
		 preferable for your machine
	-The test software makes use of a lot of parsers that are fairly delicate
	 but also fairly straightforward and should be simple to fix
	-If changes were made to the meta-parser and the test broke:
		-Look at the source code for the routing and placement parsers
		-Ensure that the assumptions made about the meta-parser output are 
		 still valid
		-If they are no longer valid, edit the parser, build it, and move 
		 its executable to its corresponding folder in .../Tests
	-If changes were made to FRoC's projects and the test broke:
		-Look at the files in .../Reference_Files/Project_Files_To_Add 
		 and ensure they are all correct
		-Look at .../Scripts/runFRoC and ensure that it's tcl file creation 
		 is still correct
		-Look at the DUTMaker and ensure its assumptions about FRoC's Verilog
		 files are still correct
		-If they are not correct edit the project, build it, and move its 
		 executable to its corresponding folder in .../Tests
		-Look at the files in .../Reference_Files/SimFiles and ensure they are
		 still correct


