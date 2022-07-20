#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <string>

enum LogLevel {
	INFO,
	WARN,
	ERROR
};

struct LogEntry {
	LogLevel level;
	std::string message;
};

class Logger {

public:
	static std::vector<LogEntry> messages;

	static void info(const std::string& message);
	static void error(const std::string& message);

};


#endif