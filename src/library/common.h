#ifndef _SRC_COMMON_H
#define _SRC_COMMON_H

#include "json.hpp"

namespace xprofiler {
using nlohmann::json;
using std::function;
using std::string;

// xprofiler logger
enum LOG_LEVEL { LOG_INFO, LOG_ERROR, LOG_DEBUG };
enum LOG_TYPE { LOG_TO_FILE, LOG_TO_TTL };

// global variables
void InitGlobalVariables();

// uptime
unsigned long GetUptime();

// commands
#define COMMAND_CALLBACK(cb)                                                   \
  void cb(json command, string (*format)(const char *, ...),                   \
          function<void(json)> success, function<void(string)> error)
} // namespace xprofiler

#endif