#pragma once
/***********************************************
* CSE678 Object Oriented Design
*
* Spring 2021
*
* Authors: Steve Brunjes, Zach Demers, Leo Garza
* Group 6
*
* Date: 04-22-2021
*
* File: TestManager.h
*
* Description: Implements a class to handle the
*              manage a group of tests
*
***********************************************/

#include <vector>
#include <string>
#include <ostream>

#include "TestClass.h"
#include "TestLogger.h"

class TestManager
{
public:

	
	static TestManager *getInstance();

	/*************************************************************************
	*
	* Creates a new test
	*
	* Parameter: 	testMethd: the test to run
	*
	* return:	None
	*
	*************************************************************************/
	void CreateTest(TestClass::CallableObject testMethod,
		const LogLevel aLogLevel,
		const std::string& aName, const std::string& error);

	void CreateTest(TestClass::CallableObject testMethod,
		const LogLevel aLogLevel,
		const std::string& aName);

	/*************************************************************************
	*
	* Runs all of the tests
	*
	* Parameter: 	None
	*
	* return:	true if all tests pass, else false
	*
	*************************************************************************/
	bool ExecuteTests();


	/*************************************************************************
	*
	* sets the name of the file to write test results to
	*
	* Parameter: 	filename: the name of the file
	*
	* return:	bool: successfully set (true) or not (false)
	*
	*************************************************************************/
	bool SetOutputFile(const std::string& filename);


	/*************************************************************************
	*
	* sets the output destination for the console messages
	*
	* Parameter: 	stream: the output stream
	*
	* return:	None
	*
	*************************************************************************/
	void SetOutputStream(std::ostream stream);

	/*************************************************************************
	*
	* sets the Logger to output to a file or not based on the input
	*
	* Parameter: 	bool: output to file if true, do not if false
	*
	* return:	None
	*
	*************************************************************************/
	void SetOutputToFile(const bool bOutputToFile);

	/*************************************************************************
	*
	* sets the Logger to output to a stream or not based on the input
	*
	* Parameter: 	bool: output to stream if true, do not if false
	*
	* return:	None
	*
	*************************************************************************/
	void SetOutputToStream(const bool bOutputToStream);

	/*************************************************************************
	*
	* Passes all TestResult objects contained as members in the _tests vector
	*   to the Logger for data output and logging
	*
	* Parameter: 	None
	*
	* return:	bool: successful or not
	*
	*************************************************************************/
	void ReportResults();
		
	void runTest(int testNumber);


	int FindTestNumber(std::string name);

private:
	/*************************************************************************
	*
	* Constructor for the TestManager class
	*
	* Parameter: 	None
	*
	* return:	None
	*
	*************************************************************************/
	TestManager();

	static TestManager* instance;
	std::vector<TestClass> _tests;	// series of tests to execute
	TestLogger _logger;					// the logger to handle logging test results
	int _numPass;					// number of tests that passed
	int _numFail;					// number of tests that failed
	int _numExc;						// number of tests that had exceptions
	double _timeElapsed;			// time it took to execute entire batch of tests in seconds
};
