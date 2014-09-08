#ifndef REMU_EMULATIONEXCEPTION_H
#define REMU_EMULATIONEXCEPTION_H

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

namespace remu {

class EmulationException : public std::exception
{
public:
  EmulationException(const char *fmt, ...)
    : str(nullptr)
    , result(0)
  {
    va_list va;

    va_start(va, fmt);
    result = vasprintf(&str, fmt, va);
    va_end(va);
  }

  virtual ~EmulationException()
  {
    if (str) {
      free(str);
    }
  }

  EmulationException(const EmulationException &other)
    : str(strdup(other.str))
  { }

  EmulationException(EmulationException &&other)
    : str(other.str)
  {
    other.str = nullptr;
  }

  EmulationException& operator=(const EmulationException &other)
  {
    if (this != &other) {
      if (str) {
        free(str);
      }

      str = strdup(other.str);
    }

    return *this;
  }

  EmulationException& operator=(EmulationException &&other)
  {
    if (this != &other) {
      if (str) {
        free(str);
      }

      str = other.str;

      other.str = nullptr;
    }

    return *this;
  }

private:
  char *str;
  int result;

  virtual const char* what() const noexcept {
    return str;
  }
};

} /*namespace remu*/

#endif /*REMU_EMULATIONEXCEPTION_H*/


