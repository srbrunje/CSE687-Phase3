#pragma once
/***********************************************
* CSE678 Object Oriented Design
*
* Spring 2021
*
* Authors: Steve Brunjes, Zach Demers, Leo Garza
* Group 6
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

	static TestManager* GetInstance();

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


	/*************************************************************************
	*
	* Creates a new test
	*
	* Parameter: 	aTestMethod: the test to run
	*				aLogLevel: the level of logging desired
	*				aName: the name of the test
	*				aError: a default error message
	*
	* return:	None
	*
	*************************************************************************/
	void CreateTest(TestClass::CallableObject aTestMethod,
		const LogLevel aLogLevel,
		const std::string& aName,
		const std::string& aError);

	void CreateTest(TestClass::CallableObject aTestMethod,
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
	* Sets the name of the file to write test results to
	*
	* Parameter: 	aFilePath: the path to the file to log data to
	*
	* return:	bool: successfully set (true) or not (false)
	*
	*************************************************************************/
	bool SetOutputFile(const std::string& aFilePath);


	/*************************************************************************
	*
	* Sets the output destination for the console messages
	*
	* Parameter: 	aStream: the output stream
	*
	* return:	None
	*
	*************************************************************************/
	void SetOutputStream(std::ostream& aStream);

	/*************************************************************************
	*
	* Sets the TestLogger to output to a file or not based on the input
	*
	* Parameter: 	bool: output to file if true, do not if false
	*
	* return:	None
	*
	*************************************************************************/
	void SetOutputToFile(const bool bOutputToFile);

	/*************************************************************************
	*
	* Sets the TestLogger to output to a stream or not based on the input
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
	*   to the TestLogger for data output and logging
	*
	* Parameter: 	None
	*
	* return:	None
	*
	*************************************************************************/
	void ReportResults();

	/*************************************************************************
	*
	* Runs a specified test based on the index number in the _tests vector or
	*    returns false if the index given is out of bounds.
	*
	* Parameter: aTestNumber: index in _tests vector denoting the test to run
	*
	* return:	bool: valid test number input (true) or not (false)
	*
	*************************************************************************/
	bool RunTest(int aTestNumber);

	/*************************************************************************
	*
	* Runs a specified test based on the index number in the _tests vector or
	*    returns false if the index given is out of bounds.
	*
	* Parameter: aTestNumber: index in _tests vector denoting the test to run
	*
	* return:	bool: valid test number input (true) or not (false)
	*
	*************************************************************************/
	int FindTestNumber(const std::string& aName);


private:
	static TestManager*  instance;
	std::vector<TestClass> _tests;	// series of tests to execute
	TestLogger _logger;				// the logger to handle logging test results
	int _numPass;					// number of tests that passed
	int _numFail;					// number of tests that failed
	int _numExc;					// number of tests that had exceptions
	double _timeElapsed;			// time it took to execute entire batch of tests in microseconds
};
