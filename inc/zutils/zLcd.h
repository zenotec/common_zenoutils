/*
 * zLcd.h
 *
 *  Created on: Jan 16, 2016
 *      Author: kmahoney
 */

#ifndef __ZLCD_H__
#define __ZLCD_H__

namespace zUtils
{
namespace zLcd
{

class Lcd;

//*****************************************************************************
// zLcd::LcdUpdateCmd Class
//*****************************************************************************

class LcdCommand : public zCommand::Command
{

public:
  LcdCommand(const std::string &name_, zLcd::Lcd &lcd_);

  virtual
  ~LcdCommand();

protected:

  zLcd::Lcd &_lcd;

private:

};

//*****************************************************************************
// zLcd::LcdUpdateCmd Class
//*****************************************************************************

class LcdUpdateCmd : public LcdCommand
{

public:
  LcdUpdateCmd(zLcd::Lcd &lcd_);

  virtual
  ~LcdUpdateCmd();

  virtual bool
  Execute(const std::string &arg_ = "");

protected:

private:

};

//*****************************************************************************
// zLcd::LcdClearCmd Class
//*****************************************************************************

class LcdClearCmd : public LcdCommand
{

public:
  LcdClearCmd(zLcd::Lcd &lcd_);

  virtual
  ~LcdClearCmd();

  virtual bool
  Execute(const std::string &arg_ = "");

protected:

private:

};

//*****************************************************************************
// zLcd::LcdVar Class
//*****************************************************************************

class LcdVar
{

  friend Lcd;

public:

  LcdVar(const std::string &name_);

  virtual
  ~LcdVar();

  std::string
  GetName();

  std::string
  GetValue();

  bool
  SetValue(const std::string &value_);

  ssize_t
  GetCol();

  ssize_t
  GetRow();

  ssize_t
  GetLength();

protected:

  void
  setCol(size_t col);

  void
  setRow(size_t row);

  void
  setLength(size_t size);

private:

  zSem::Mutex _lock;

  size_t _row;
  size_t _col;
  size_t _len;

  std::string _name;
  std::string _value;

};

//*****************************************************************************
// zLcd::Lcd Class
//*****************************************************************************

class Lcd : public zEvent::EventObserver
{

public:

  Lcd();

  virtual
  ~Lcd();

  bool
  RegisterCommands(zCommand::CommandHandler *handler_);

  bool
  UnregisterCommands(zCommand::CommandHandler *handler_);

  bool
  SetLimits(const size_t rows_, const size_t cols_);

  bool
  SetRefresh(const size_t rate_);

  LcdVar *
  CreateVar(const std::string &name_, const size_t row_, const size_t col_, const size_t len_);

  bool
  DeleteVar(zLcd::LcdVar *var_);

  virtual bool
  Update(const std::string &str_, const size_t row_ = 0, const size_t col_ = 0);

  virtual bool
  Clear();

protected:

  virtual bool
  EventHandler(const zEvent::EventNotification *notification_);

  virtual bool
  _update(std::vector<std::vector<char> > &buf_) = 0;

  virtual bool
  _clear() = 0;

private:

  zSem::Mutex _lock;

  size_t _rows;
  size_t _cols;

  std::vector<std::vector<char> > _disp_buf;
  std::list<zLcd::LcdVar *> _vars;

  zTimer::Timer _timer;
  zEvent::EventHandler _timer_handler;

  LcdUpdateCmd _update_cmd;
  LcdClearCmd _clear_cmd;

};

}
}

#endif /* __ZLCD_H__ */
