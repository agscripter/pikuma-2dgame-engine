#include "Logger.h"
#include <iostream>
#include <iomanip>
#include <ctime>

std::vector<LogEntry> Logger::messages;

std::string getCurrentDateTime() {
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time (&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
	return buffer;
}

LogEntry createLogMessage(LogLevel level, const std::string& message) {
	std::string finalMessage = "[" + getCurrentDateTime() + "] " + message;

	LogEntry logEntry;
	logEntry.level = level;
	logEntry.message = message;

	return logEntry;
}

void printLog(LogEntry logEntry) {
	std::cout << logEntry.message << std::endl;
}

void Logger::info(const std::string& message) {
	LogEntry logEntry = createLogMessage(LogLevel::INFO, message);
	printLog(logEntry);
	Logger::messages.push_back(logEntry);
}

void Logger::error(const std::string& message) {
	LogEntry logEntry = createLogMessage(LogLevel::ERROR, message);
	printLog(logEntry);
	Logger::messages.push_back(logEntry);
}

