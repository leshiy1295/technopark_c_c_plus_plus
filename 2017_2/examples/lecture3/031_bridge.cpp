#include <iostream>

#include <string>

using namespace std;

// logger.h
// --------------------------------
struct LoggerImpl;
struct Logger {
  Logger(LoggerImpl *p) : pimpl(p) {};
  virtual ~Logger() = default;
  virtual void log(string &str) = 0;

protected:
  LoggerImpl *pimpl = nullptr;
};
// --------------------------------

// logger_impl.h
// --------------------------------
struct LoggerImpl {
  virtual ~LoggerImpl() {}
  virtual void console_log(string &str) = 0;

  virtual void file_log(const std::string& str) = 0;

  virtual void socket_log(const std::string& str) = 0;
};

struct ST_LoggerImpl : public LoggerImpl {
  void console_log(string &str) { std::cout << "ST == " << str; };

  virtual void file_log(const std::string& str) {};
};

struct MT_LoggerImpl : public LoggerImpl {
  void console_log(string &str) { std::cout << "MT == " << str; };;
};
// --------------------------------


// console_logger.h
// --------------------------------

#define MT

struct ConsoleLogger : public Logger {
  ConsoleLogger()
      : Logger(
#ifdef MT
            new MT_LoggerImpl()
#else
            new ST_LoggerImpl()
#endif
        ){};
  void log(string &str){ pimpl->console_log(str); };
};

struct FileLogger {
  void log(string &str){ pimpl->file_log(str); };
};
// --------------------------------

int main() {
  Logger * p = new ConsoleLogger();
  std::string mess = "some message";
  p->log(mess);
  return 0;
}
