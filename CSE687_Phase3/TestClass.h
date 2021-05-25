#pragma once

/***********************************************
* CSE678 Object Oriented Design
*
* Spring 2021
*
* Authors: Steve Brunjes, Zach Demers, Leo Garza
* Group 6
*
* Date: 04-26-2021
*
* File: TestClass.h
*
* Data Defined: class TestResult
*
* Description: Implements a class to handle the
*              result of a test.
*
***********************************************/

#include <string>
#include <json/json.h>
#include "utils.h"		// common utility functions and constants


/* Class: TestResult
* Contains basic data describing the result of the test to include pass/fail status,
*	any error messages, test duration, and the desired log level
*/
class TestResult {
public:
	// Public enum for TestResult's current status
	enum class Status {
		PASS, FAIL, Fail_EXC, NOT_RUN
	};

private:

	// Private Member Variables (PMV)
	std::string  _name;			// name of the test that resulted in this result
	Status       _status;		// pass, fail, or not run status denotation
	std::string  _errorMessage;	// any/all error messages that occurred during test
	timing::hack _startTime;	// the time the test began to execute
	timing::hack _endTime;		// the time the test finished execution
	LogLevel     _logLevel;		// the level of logging desired
	Json::Value root; // JSON ROOT {}
	Json::Value data; // JSON DATA 'NAME' = '', 

	// Private Member Functions (PMF)
	// - none for now

public:
	// Constructors
	TestResult(const std::string& aName = "");	// default constructor with option of setting name


	// Setters
	void SetName(const std::string& aName);
	void SetStatus(const Status aStatus);
	void SetErrorMessage(const std::string& aErrMsg);
	void SetStartTime(const timing::hack aStartTime);
	void SetEndTime(const timing::hack aEndTime);
	void SetLogLevel(const LogLevel aLogLevel);
	void SetClassObject();

	// Getters
	std::string     GetName() const;
	Status          GetStatus() const;
	std::string     GetErrorMessage() const;
	double          GetDuration() const;	// returns the time taken to run the test in microseconds
	LogLevel		GetLogLevel() const;
	std::string		GetClassObject();
};
