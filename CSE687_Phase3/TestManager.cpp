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
* File: TestManager.cpp
*
* Description: Implements a class to handle the
*              manage a group of tests
*
***********************************************/

#include "TestManager.h"
#include "TestClass.h"
#include "utils.h"
#include "Comm.h"
#include <iostream>
#include <thread>
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include <fstream>
#include <functional>
#include <algorithm>
#include <conio.h>

using namespace MsgPassingCommunication;

TestManager* TestManager::instance = 0;

TestManager* TestManager::getInstance()
{
	if (!instance)
	{
		instance = new TestManager();
	}
	return instance;
}

/*************************************************************************
	*
	* Constructor for the TestManager class
	*
	* Parameter: 	None
	*
	* return:	None
	*
	*************************************************************************/
TestManager::TestManager()
{
	_tests = std::vector<TestClass>();
	_logger = TestLogger();
	_numPass = 0;
	_numFail = 0;
	_numExc = 0;
	_timeElapsed = 0.0;
}


/*************************************************************************
*
* Creates a new test
*
* Parameter: 	testMethod: the test to run
*
* return:	None
*
*************************************************************************/
// Overload
void TestManager::CreateTest(TestClass::CallableObject testMethod,
	const LogLevel aLogLevel,
	const std::string& aName, const std::string& errorMessage)
{
	_tests.push_back(TestClass(testMethod, aLogLevel, aName, errorMessage));
}

void TestManager::CreateTest(TestClass::CallableObject testMethod,
	const LogLevel aLogLevel,
	const std::string& aName)
{
	_tests.push_back(TestClass(testMethod, aLogLevel, aName));
}




void TestMSG(TestResult status)
{
	Comm comm(EndPoint("localhost", 9892), "TestMSG");
	comm.start();

	EndPoint serverEP("localhost", 9893);
	EndPoint clientEP("localhost", 9892);

	Message msg(serverEP, clientEP);

	std::string classObject = removeNewLine(status.GetClassObject()); // getting our JSON Data and removes new lines found inside JSON OBJECT

	msg.attribute("JSON", classObject); // adding JSON attritubute 

	//set status
	comm.postMessage(msg); // posting message

	comm.stop();

}



/*************************************************************************
*
* Runs all of the tests
*
* Parameter: 	None
*
* return:	true if all tests pass, else false
*
*************************************************************************/
bool TestManager::ExecuteTests()
{

	//this will change to send a message to the server to run a particular test.
	//the message will also set the log parameterrs


	//if there are no tests return false (the user probally ment to do something)
	if (_tests.size() <= 0) {
		std::cerr << "Error: No tests have been queued to run!\n";
		return false;
	}

	// Reset the number of passes in failures from any previous runs
	_numPass = 0;
	_numFail = 0;

	// Start timer for overall time of test execution
	timing::hack testStart = timing::now();

	//loop through and execute each test
	for (int i = 0; i < _tests.size(); i++)
	{

		const TestResult* result = _tests[i].RunTest();

		//create the message reply and send
		TestResult r = *result;

		//	std::thread r1(TestMSG);
		//	r1.join();
		TestMSG(r);

		if (result->GetStatus() == TestResult::Status::PASS) {
			_numPass++;
		}
		else if (result->GetStatus() == TestResult::Status::FAIL_EXC) {
			_numExc++;
			_numFail++;
		}
		else {
			_numFail++;
		}
	}

	_timeElapsed = timing::duration_us(testStart);

	// Only return true if every test passed, false otherwise
	return _numFail == 0;
}


/*************************************************************************
*
* sets the name of the file to write test results to
*
* Parameter: 	filename: the name of the file
*
* return:	bool: successfully set (true) or not (false)
*
*************************************************************************/
bool TestManager::SetOutputFile(const std::string& filename)
{
	// If the filepath is valid, this will also set Logger::_outputToFile to true
	return _logger.SetOutputFile(filename);
}


/*************************************************************************
*
* sets the output destination for the console messages
*
* Parameter: 	stream: the output stream
*
* return:	None
*
*************************************************************************/
void TestManager::SetOutputStream(std::ostream stream)
{
	// TODO: determine if we want to allow this, or just use std::cout/cerr, etc.
	// Turns out it's a little complicated to pass a stream through a function, and
	// the large majority of cases will just use a std stream.
}

/*************************************************************************
*
* sets the Logger to output to a file or not based on the input
*
* Parameter: 	bool: output to file if true, do not if false
*
* return:	None
*
*************************************************************************/
void TestManager::SetOutputToFile(const bool bOutputToFile)
{
	_logger.SetOutputToFile(bOutputToFile);
}

/*************************************************************************
*
* sets the Logger to output to a stream or not based on the input
*
* Parameter: 	bool: output to stream if true, do not if false
*
* return:	None
*
*************************************************************************/
void TestManager::SetOutputToStream(const bool bOutputToStream)
{
	_logger.SetOutputToStream(bOutputToStream);
}

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
void TestManager::ReportResults()
{
	_logger.ClearContents(); // clear old test output in preparation for new run

	// Log the overview of the results || As well as using FormatTimeString from class utils
	_logger.LogMessage(
		"Completed " + std::to_string(_tests.size()) + " tests with "
		+ std::to_string(_numPass) + " passes and " + std::to_string(_numFail)
		+ " failures with " + std::to_string(_numExc) + " Exceptions\nTotal time elapsed: " + FormatTimeString(_timeElapsed) + "\n\n"
	);

	// Log the results per individual test
	// TODO: potentially add a LogResults(std::vector<TestResult>) function to the
	//   Logger class to reduce the number of times a file is opened and closed, etc.
	for (int i = 0; i < _tests.size(); i++) {
		_logger.LogMessage("Test " + std::to_string(i + 1) + " of " + std::to_string(_tests.size()) + ":\n");
		_logger.LogResult(_tests[i].GetTestResult());
	}
}


void TestManager::runTest(int testNumber)
{
	const TestResult* result = _tests[testNumber].RunTest();

	//create the message reply and send
	TestResult r = *result;

	TestMSG(r);
}


int TestManager::FindTestNumber(std::string name)
{
	int rtn = -1;
	//find the test in the list
	for (int i = 0; i < _tests.size(); i++)
	{
		if (_tests[i].GetName().compare(name) == 0)
		{
			rtn = i;
		}
	}

	return rtn;
}
