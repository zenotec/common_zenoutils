/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
  Execute(CommandData& data_);

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
  Execute(CommandData& data_);

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

  DisplayBuffer(DisplayBuffer& other_);

  DisplayBuffer(const DisplayBuffer& other_);

  virtual
  ~DisplayBuffer();

  bool
  operator==(const DisplayBuffer& other_) const;

  bool
  operator!=(const DisplayBuffer& other_) const;

  DisplayBuffer&
  operator=(DisplayBuffer& other_);

  DisplayBuffer&
  operator=(const DisplayBuffer& other_);

  char
  operator [](const size_t i_) const;

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

  void
  Print() const;

protected:

private:

  mutable zSem::Mutex _buffer_lock;

  size_t _rows;
  size_t _cols;
  char* _buffer;

  inline bool
  _index2rowcol(size_t index_, size_t* row_ = NULL, size_t* col_ = NULL) const;

  inline bool
  _rowcol2index(size_t row_, size_t col_, size_t* index_ = NULL) const;

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
// Class: zDisplay::DisplayPage
//*****************************************************************************

class DisplayPage : public DisplayBuffer
{

public:

  DisplayPage(const std::string &name_, const size_t cols_, const size_t rows_);

  virtual
  ~DisplayPage();

  bool
  operator==(const DisplayPage &other_) const;

  bool
  operator!=(const DisplayPage &other_) const;

  std::string
  GetName() const;

  DisplayVar*
  CreateVar(const std::string &name_, const size_t len_);

  bool
  DeleteVar(zDisplay::DisplayVar* var_);

  bool
  Refresh();

  const DisplayBuffer&
  GetBuffer() const;

protected:

private:

  std::string _name;
  std::list<DisplayVar*> _vars;

};

//*****************************************************************************
// Class: zDisplay::Display
//*****************************************************************************

class Display : public zEvent::EventObserver
{

public:

  Display(const size_t cols_, const size_t rows_ = 1);

  virtual
  ~Display();

//  bool
//  RegisterCommands(zCommand::CommandHandler *handler_);
//
//  bool
//  UnregisterCommands(zCommand::CommandHandler *handler_);

  DisplayPage*
  CreatePage(const std::string& name_);

  bool
  DeletePage(DisplayPage* page_);

  bool
  SetRefreshRate(const size_t rate_);

  bool
  SetPageTimeout(const size_t sec_);

  ssize_t
  GetRows() const;

  ssize_t
  GetColumns() const;

  ssize_t
  GetSize() const;

  void
  Flush();

  void
  Clear();

protected:

  virtual bool
  EventHandler(zEvent::EventNotification *notification_);

  virtual bool
  update(const DisplayBuffer& buf_) = 0;

private:

  size_t _cols;
  size_t _rows;

  std::list<DisplayPage*> _pages;

  zTimer::Timer _refresh_timer;
  zTimer::Timer _page_timer;

  DisplayUpdateCmd _update_cmd;
  DisplayClearCmd _clear_cmd;

};

}
}

#endif /* __ZDISPLAY_H__ */
