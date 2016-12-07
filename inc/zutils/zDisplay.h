/*
 * zDisplay.h
 *
 *  Created on: Jan 16, 2016
 *      Author: kmahoney
 */

#ifndef __ZDISPLAY_H__
#define __ZDISPLAY_H__

namespace zUtils
{
namespace zDisplay
{

class Display;

//*****************************************************************************
// Class: zDisplay::DisplayCommand
//*****************************************************************************

class DisplayCommand : public zCommand::Command
{

public:
  DisplayCommand(const std::string &name_, zDisplay::Display &display_);

  virtual
  ~DisplayCommand();

protected:

  zDisplay::Display &_display;

private:

};

//*****************************************************************************
// Class: zDisplay::DisplayUpdateCmd
//*****************************************************************************

class DisplayUpdateCmd : public DisplayCommand
{

public:
  DisplayUpdateCmd(zDisplay::Display &display_);

  virtual
  ~DisplayUpdateCmd();

  virtual bool
  Execute(const std::string &arg_ = "");

protected:

private:

};

//*****************************************************************************
// Class: zDisplay::DisplayClearCmd
//*****************************************************************************

class DisplayClearCmd : public DisplayCommand
{

public:
  DisplayClearCmd(zDisplay::Display &display_);

  virtual
  ~DisplayClearCmd();

  virtual bool
  Execute(const std::string &arg_ = "");

protected:

private:

};

//*****************************************************************************
// Class: zDisplay::DisplayBuffer
//*****************************************************************************

class DisplayBuffer
{

public:

  DisplayBuffer(const size_t cols_, const size_t rows_ = 1);

  DisplayBuffer(DisplayBuffer &other_);

  DisplayBuffer(const DisplayBuffer &other_);

  virtual
  ~DisplayBuffer();

  bool
  operator==(const DisplayBuffer &other_) const;

  bool
  operator!=(const DisplayBuffer &other_) const;

  DisplayBuffer &
  operator=(DisplayBuffer &other_);

  DisplayBuffer&
  operator=(const DisplayBuffer &other_);

  char
  operator [](const size_t i_);

  ssize_t
  GetRows() const;

  ssize_t
  GetColumns() const;

  ssize_t
  GetSize() const;

  std::string
  GetString(const size_t col_ = 0, const size_t row_ = 0) const;

  ssize_t
  Update(const std::string& str_, const size_t col_ = 0, const size_t row_ = 0);

  void
  Clear();

protected:

private:

  mutable zSem::Mutex _buffer_lock;

  size_t _rows;
  size_t _cols;
  char* _buffer;

};

//*****************************************************************************
// Class: zDisplay::DisplayVar
//*****************************************************************************

class DisplayVar : public DisplayBuffer
{

public:

  DisplayVar(const std::string &name_, const size_t size_);

  virtual
  ~DisplayVar();

  bool
  operator==(const DisplayVar &other_) const;

  bool
  operator!=(const DisplayVar &other_) const;

  std::string
  GetName() const;

  ssize_t
  GetColumn() const;

  bool
  SetColumn(size_t col);

  ssize_t
  GetRow() const;

  bool
  SetRow(size_t row);

protected:

private:

  std::string _name;
  size_t _row;
  size_t _col;

};

//*****************************************************************************
// Class: zDisplay::Display
//*****************************************************************************

class Display : public DisplayBuffer, public zEvent::EventObserver
{

public:

  Display(const size_t cols_, const size_t rows_ = 1);

  virtual
  ~Display();

  Display&
  operator=(DisplayBuffer &other_);

  Display&
  operator=(const DisplayBuffer &other_);

  bool
  RegisterCommands(zCommand::CommandHandler *handler_);

  bool
  UnregisterCommands(zCommand::CommandHandler *handler_);

  bool
  SetRefresh(const size_t rate_);

  DisplayVar*
  CreateVar(const std::string &name_, const size_t len_);

  bool
  DeleteVar(zDisplay::DisplayVar* var_);

protected:

  virtual bool
  EventHandler(const zEvent::EventNotification *notification_);

  virtual bool
  update(const DisplayBuffer& buf_) = 0;

private:

  zSem::Mutex _disp_lock;

  std::list<DisplayVar*> _vars;

  zTimer::Timer _timer;
  zEvent::EventHandler _timer_handler;

  DisplayUpdateCmd _update_cmd;
  DisplayClearCmd _clear_cmd;

};

}
}

#endif /* __ZDISPLAY_H__ */
