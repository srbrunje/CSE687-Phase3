#ifndef __TESTLOGGER_H
#define __TESTLOGGER_H


/*******************************************************************
* TODO: Remember to insert a file header eventually
*******************************************************************/


#include <string>
#include <ostream>
#include <filesystem> // requires C++17 or later

#include "TestClass.h"

class TestLogger {
private:
	// Private Member Variables (PMV)
	bool                  _outputToFile;     // logs to file if true, does not if false
	bool                  _outputToStream;   // logs to stream if true, does not if false
	std::filesystem::path _outputFile;       // file path to where data is logged
	//std::ostream          _outputStream;	 // ostream to pipe data to

	// Private Member Functions (PMF)
	// - none for now

public:
	// Constructors
	TestLogger(const std::string& aPathToOutputFile = "", const bool bOutputToStream = false);

	// Setters
	void SetOutputToFile(const bool bOutputToFile);
	void SetOutputToStream(const bool bOutputToStream);
	bool SetOutputFile(const std::string& aPathToFile);
	//void SetOutputStream(std::ostream& aStream);

	// Getters
	bool IsOutputtingToFile() const;
	bool IsOutputtingToStream() const;
	std::string GetOutputFile() const;
	//std::ostream GetOutputStream() const;

	// Support Functions
	void LogResult(const TestResult& aTestResultPtr);
	void LogMessage(const std::string& aMessage);
	void ClearContents();
};

#endif // !__TESTLOGGER_H
