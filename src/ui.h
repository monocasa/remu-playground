#ifndef REMU_UI_H
#define REMU_UI_H

class Ui
{
public:
  virtual void info(const char *fmt, ...) = 0;
  virtual void error(const char *fmt, ...) = 0;
};

#endif /*REMU_UI_H*/

