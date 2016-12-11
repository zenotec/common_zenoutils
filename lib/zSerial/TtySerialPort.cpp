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

#include <string.h>
#include <errno.h>
#include <sys/poll.h>

#include <zutils/zLog.h>
#include <zutils/zTtySerialPort.h>

namespace zUtils
{
namespace zSerial
{

//**********************************************************************
// Class: TtySerialConfigPath
//**********************************************************************

const std::string TtySerialConfigPath::ConfigDevicePath("Device");
const std::string TtySerialConfigPath::ConfigBaudPath("Baud");
const std::string TtySerialConfigPath::ConfigDatabitsPath("Databits");
const std::string TtySerialConfigPath::ConfigStopbitsPath("Stopbits");
const std::string TtySerialConfigPath::ConfigParityPath("Parity");
const std::string TtySerialConfigPath::ConfigFlowCtrlPath("Flow");

TtySerialConfigPath::TtySerialConfigPath()
{
}

TtySerialConfigPath::~TtySerialConfigPath()
{
}

zConfig::ConfigPath
TtySerialConfigPath::Device()
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigDevicePath);
  return(path);
}

zConfig::ConfigPath
TtySerialConfigPath::Baud()
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigBaudPath);
  return(path);
}

zConfig::ConfigPath
TtySerialConfigPath::Databits()
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigDatabitsPath);
  return(path);
}

zConfig::ConfigPath
TtySerialConfigPath::Stopbits()
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigStopbitsPath);
  return(path);
}

zConfig::ConfigPath
TtySerialConfigPath::Parity()
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigParityPath);
  return(path);
}

zConfig::ConfigPath
TtySerialConfigPath::FlowCtrl()
{
  zConfig::ConfigPath path(*this);
  path.Append(ConfigFlowCtrlPath);
  return(path);
}

//*****************************************************************************
// Class: TtySerialConfiguration
//*****************************************************************************

const std::string TtySerialConfigData::ConfigDeviceDefault("");

const std::string TtySerialConfigData::ConfigBaud9600("9600");
const std::string TtySerialConfigData::ConfigBaud19200("19200");
const std::string TtySerialConfigData::ConfigBaud38400("38400");
const std::string TtySerialConfigData::ConfigBaud57600("57600");
const std::string TtySerialConfigData::ConfigBaud115200("115200");
const std::string TtySerialConfigData::ConfigBaudDefault(ConfigBaud115200);

const std::string TtySerialConfigData::ConfigDatabits5("5");
const std::string TtySerialConfigData::ConfigDatabits6("6");
const std::string TtySerialConfigData::ConfigDatabits7("7");
const std::string TtySerialConfigData::ConfigDatabits8("8");
const std::string TtySerialConfigData::ConfigDatabitsDefault(ConfigDatabits8);

const std::string TtySerialConfigData::ConfigStopbits1("1");
const std::string TtySerialConfigData::ConfigStopbits2("2");
const std::string TtySerialConfigData::ConfigStopbitsDefault(ConfigStopbits1);

const std::string TtySerialConfigData::ConfigParityNone("None");
const std::string TtySerialConfigData::ConfigParityOdd("Odd");
const std::string TtySerialConfigData::ConfigParityEven("Even");
const std::string TtySerialConfigData::ConfigParityDefault(ConfigParityNone);

const std::string TtySerialConfigData::ConfigFlowCtrlNone("None");
const std::string TtySerialConfigData::ConfigFlowCtrlHard("Hard");
const std::string TtySerialConfigData::ConfigFlowCtrlDefault(ConfigFlowCtrlNone);

TtySerialConfigData::TtySerialConfigData()
{
  this->SetType(SerialConfigData::ConfigTypeTty);
  this->SetDevice(this->GetDevice());
  this->SetBaud(this->GetBaud());
  this->SetDataBits(this->GetDataBits());
  this->SetStopBits(this->GetStopBits());
  this->SetParity(this->GetParity());
  this->SetFlowControl(this->GetFlowControl());
}

TtySerialConfigData::TtySerialConfigData(zData::Data &data_) :
    SerialConfigData(data_)
{
  this->SetType(SerialConfigData::ConfigTypeTty);
  this->SetDevice(this->GetDevice());
  this->SetBaud(this->GetBaud());
  this->SetDataBits(this->GetDataBits());
  this->SetStopBits(this->GetStopBits());
  this->SetParity(this->GetParity());
  this->SetFlowControl(this->GetFlowControl());
}

TtySerialConfigData::TtySerialConfigData(zConfig::ConfigData& config_) :
    SerialConfigData(config_)
{
  this->SetType(SerialConfigData::ConfigTypeTty);
  this->SetDevice(this->GetDevice());
  this->SetBaud(this->GetBaud());
  this->SetDataBits(this->GetDataBits());
  this->SetStopBits(this->GetStopBits());
  this->SetParity(this->GetParity());
  this->SetFlowControl(this->GetFlowControl());
}

TtySerialConfigData::~TtySerialConfigData()
{
}

std::string
TtySerialConfigData::GetDevice() const
{
  std::string str;
  TtySerialConfigPath path;
  if (!this->Get(path.Device(), str))
  {
    str = ConfigDeviceDefault;
  }
  return (str);
}

bool
TtySerialConfigData::SetDevice(const std::string& dev_)
{
  TtySerialConfigPath path;
  return (this->Put(path.Device(), dev_));
}

std::string
TtySerialConfigData::GetBaud() const
{
  std::string str;
  TtySerialConfigPath path;
  if (!this->Get(path.Baud(), str))
  {
    str = ConfigBaudDefault;
  }
  return (str);
}

bool
TtySerialConfigData::SetBaud(const std::string& baud_)
{
  TtySerialConfigPath path;
  return (this->Put(path.Baud(), baud_));
}

std::string
TtySerialConfigData::GetDataBits() const
{
  std::string str;
  TtySerialConfigPath path;
  if (!this->Get(path.Databits(), str))
  {
    str = ConfigDatabitsDefault;
  }
  return (str);
}

bool
TtySerialConfigData::SetDataBits(const std::string& dbits_)
{
  TtySerialConfigPath path;
  return (this->Put(path.Databits(), dbits_));
}

std::string
TtySerialConfigData::GetStopBits() const
{
  std::string str;
  TtySerialConfigPath path;
  if (!this->Get(path.Stopbits(), str))
  {
    str = ConfigStopbitsDefault;
  }
  return (str);
}

bool
TtySerialConfigData::SetStopBits(const std::string& sbits_)
{
  TtySerialConfigPath path;
  return (this->Put(path.Stopbits(), sbits_));
}

std::string
TtySerialConfigData::GetParity() const
{
  std::string str;
  TtySerialConfigPath path;
  if (!this->Get(path.Parity(), str))
  {
    str = ConfigParityDefault;
  }
  return (str);
}

bool
TtySerialConfigData::SetParity(const std::string parity_)
{
  TtySerialConfigPath path;
  return (this->Put(path.Parity(), parity_));
}

std::string
TtySerialConfigData::GetFlowControl() const
{
  std::string str;
  TtySerialConfigPath path;
  if (!this->Get(path.FlowCtrl(), str))
  {
    str = ConfigFlowCtrlDefault;
  }
  return (str);
}

bool
TtySerialConfigData::SetFlowControl(const std::string& flowcntl_)
{
  TtySerialConfigPath path;
  return (this->Put(path.FlowCtrl(), flowcntl_));
}

//*****************************************************************************
// Class: TtyPortRecv
//*****************************************************************************

void
TtyPortRecv::Run(zThread::ThreadArg *arg_)

{

  char c = 0;
  TtySerialPort *port = (TtySerialPort *) arg_;

  if (!arg_)
  {
    return;
  }

  ZLOG_DEBUG("RX: Polling TTY for data");

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = port->_fd;
  fds[0].events = (POLLIN | POLLERR);

  while (!this->Exit())
  {

    int ret = poll(fds, 1, 100);
    if (ret > 0 && (fds[0].revents == POLLIN))
    {
      ZLOG_DEBUG(std::string("Receiving char"));
      ret = read(port->_fd, &c, 1);
      if (ret == 1)
      {
        ZLOG_DEBUG(std::string("Received char: ") + zLog::CharStr(c) +
            ": " + ((isprint(c)) ? c : ' '));
        port->rxchar(c);
      }
    }

  }

  return;
}

//*****************************************************************************
// Class: TtyPortSend
//*****************************************************************************

void
TtyPortSend::Run(zThread::ThreadArg *arg_)
{

  char c = 0;
  TtySerialPort *port = (TtySerialPort *) arg_;

  if (!arg_)
  {
    return;
  }

  ZLOG_DEBUG("TX: Polling TTY for data");

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = port->_fd;
  fds[0].events = (POLLOUT | POLLERR);

  while (!this->Exit())
  {

    // Wait for data to send
    if (port->txchar(&c, 100))
    {
      int ret = poll(fds, 1, 100);
      if (ret > 0 && (fds[0].revents == POLLOUT))
      {
        ZLOG_DEBUG(std::string("Sending char: '") + ZLOG_UCHAR(c) +
            ": " + ((isprint(c)) ? c : ' ') + std::string("'"));
        if ((write(port->_fd, &c, 1) == 1))
        {
          ZLOG_DEBUG(std::string("Sent char: '") + ZLOG_UCHAR(c) +
              ": " + ((isprint(c)) ? c : ' ') + std::string("'"));
        }
      }
    }

  }

  return;
}

//*****************************************************************************
// Class: TtySerialPort
//*****************************************************************************

TtySerialPort::TtySerialPort() :
    _options(0), _fd(0), _rx_thread(&this->_rx_func, this), _tx_thread(&this->_tx_func, this)

{

  memset(&_termios, 0, sizeof(_termios));
  memset(&_savedTermios, 0, sizeof(_savedTermios));

  // Configure the port
  this->SetDevice(this->GetDevice());
  this->SetBaud(this->GetBaud());
  this->SetDataBits(this->GetDataBits());
  this->SetStopBits(this->GetStopBits());
  this->SetParity(this->GetParity());
  this->SetFlowControl(this->GetFlowControl());
  this->SetBlocking(this->GetBlocking());

  this->_rx_thread.Name(std::string("RxTTY"));
  this->_tx_thread.Name(std::string("TxTTY"));

}

TtySerialPort::TtySerialPort(const TtySerialConfigData& config_) :
    _options(0), _fd(0), _config(config_), _rx_thread(&this->_rx_func, this),
        _tx_thread(&this->_tx_func, this)
{

  memset(&_termios, 0, sizeof(_termios));
  memset(&_savedTermios, 0, sizeof(_savedTermios));

  // Configure the port
  this->SetDevice(this->GetDevice());
  this->SetBaud(this->GetBaud());
  this->SetDataBits(this->GetDataBits());
  this->SetStopBits(this->GetStopBits());
  this->SetParity(this->GetParity());
  this->SetFlowControl(this->GetFlowControl());
  this->SetBlocking(this->GetBlocking());

  this->_rx_thread.Name(std::string("RxTTY"));
  this->_tx_thread.Name(std::string("TxTTY"));

}

TtySerialPort::~TtySerialPort()
{
  this->Close();
}

bool
TtySerialPort::Open()
{

  bool status = false;

  if (!this->_fd)
  {

    // Setup terminal I/O structure
    memset(&this->_termios, 0, sizeof(this->_termios));
    cfmakeraw(&this->_termios);
    this->_termios.c_cflag |= (CLOCAL | CREAD);

    this->_fd = open(TtySerialConfigData::GetDevice().c_str(), O_RDWR | O_NOCTTY | this->_options);
    if (this->_fd > 0)
    {
      tcgetattr(this->_fd, &this->_savedTermios);
      tcflush(this->_fd, TCIOFLUSH);
      if (tcsetattr(this->_fd, TCSANOW, &this->_termios) == 0)
      {
        if (this->_rx_thread.Start() && this->_tx_thread.Start())
        {
          status = true;
        }
      }
    }
    else
    {
      ZLOG_ERR(
          "Cannot open TTY port " + TtySerialConfigData::GetDevice() + ": " + std::string(strerror(errno)));
    }
  }
  return (status);
}

bool
TtySerialPort::Close()
{
  if (this->_fd)
  {
    this->_rx_thread.Stop();
    this->_tx_thread.Stop();
    tcsetattr(this->_fd, TCSANOW, &this->_savedTermios);
    close(this->_fd);
    this->_fd = 0;
  }
  return (true);
}

TtySerialPort::BAUD
TtySerialPort::GetBaud() const
{
  TtySerialPort::BAUD baud = TtySerialPort::BAUD_DEF;
  std::string str = TtySerialConfigData::GetBaud();

  if (TtySerialConfigData::ConfigBaud9600 == str)
  {
    baud = TtySerialPort::BAUD_9600;
  }
  else if (TtySerialConfigData::ConfigBaud19200 == str)
  {
    baud = TtySerialPort::BAUD_19200;
  }
  else if (TtySerialConfigData::ConfigBaud38400 == str)
  {
    baud = TtySerialPort::BAUD_38400;
  }
  else if (TtySerialConfigData::ConfigBaud57600 == str)
  {
    baud = TtySerialPort::BAUD_57600;
  }
  else if (TtySerialConfigData::ConfigBaud115200 == str)
  {
    baud = TtySerialPort::BAUD_115200;
  }
  else
  {
    baud = TtySerialPort::BAUD_ERR;
  }
  return (baud);
}

bool
TtySerialPort::SetBaud(TtySerialPort::BAUD baud_)
{
  bool status = false;

  if (TtySerialPort::BAUD_9600 == baud_)
  {
    if (!cfsetspeed(&this->_termios, B9600))
    {
      status = TtySerialConfigData::SetBaud(TtySerialConfigData::ConfigBaud9600);
    }
  }
  else if (TtySerialPort::BAUD_19200 == baud_)
  {
    if (!cfsetspeed(&this->_termios, B19200))
    {
      status = TtySerialConfigData::SetBaud(TtySerialConfigData::ConfigBaud19200);
    }
  }
  else if (TtySerialPort::BAUD_38400 == baud_)
  {
    if (!cfsetspeed(&this->_termios, B38400))
    {
      status = TtySerialConfigData::SetBaud(TtySerialConfigData::ConfigBaud38400);
    }
  }
  else if (TtySerialPort::BAUD_57600 == baud_)
  {
    if (!cfsetspeed(&this->_termios, B57600))
    {
      status = TtySerialConfigData::SetBaud(TtySerialConfigData::ConfigBaud57600);
    }
  }
  else if (TtySerialPort::BAUD_115200 == baud_)
  {
    if (!cfsetspeed(&this->_termios, B115200))
    {
      status = TtySerialConfigData::SetBaud(TtySerialConfigData::ConfigBaud115200);
    }
  }

  return (status);

}

TtySerialPort::DATABITS
TtySerialPort::GetDataBits() const
{
  TtySerialPort::DATABITS bits = TtySerialPort::DATABITS_DEF;
  std::string str = TtySerialConfigData::GetDataBits();

  if (TtySerialConfigData::ConfigDatabits5 == str)
  {
    bits = TtySerialPort::DATABITS_5;
  }
  else if (TtySerialConfigData::ConfigDatabits6 == str)
  {
    bits = TtySerialPort::DATABITS_6;
  }
  else if (TtySerialConfigData::ConfigDatabits7 == str)
  {
    bits = TtySerialPort::DATABITS_7;
  }
  else if (TtySerialConfigData::ConfigDatabits8 == str)
  {
    bits = TtySerialPort::DATABITS_8;
  }
  else
  {
    bits = TtySerialPort::DATABITS_ERR;
  }

  return (bits);
}

bool
TtySerialPort::SetDataBits(TtySerialPort::DATABITS dbits_)
{

  bool status = false;

  if (TtySerialPort::DATABITS_5 == dbits_)
  {
    this->_termios.c_cflag &= ~CSIZE;
    this->_termios.c_cflag |= CS5;
    status = TtySerialConfigData::SetDataBits(TtySerialConfigData::ConfigDatabits5);
  }
  else if (TtySerialPort::DATABITS_6 == dbits_)
  {
    this->_termios.c_cflag &= ~CSIZE;
    this->_termios.c_cflag |= CS6;
    status = TtySerialConfigData::SetDataBits(TtySerialConfigData::ConfigDatabits6);
  }
  else if (TtySerialPort::DATABITS_7 == dbits_)
  {
    this->_termios.c_cflag &= ~CSIZE;
    this->_termios.c_cflag |= CS7;
    status = TtySerialConfigData::SetDataBits(TtySerialConfigData::ConfigDatabits7);
  }
  else if (TtySerialPort::DATABITS_8 == dbits_)
  {
    this->_termios.c_cflag &= ~CSIZE;
    this->_termios.c_cflag |= CS8;
    status = TtySerialConfigData::SetDataBits(TtySerialConfigData::ConfigDatabits8);
  }

  return (status);

}

TtySerialPort::STOPBITS
TtySerialPort::GetStopBits() const
{
  TtySerialPort::STOPBITS bits = TtySerialPort::STOPBITS_DEF;
  std::string str = TtySerialConfigData::GetStopBits();

  if (TtySerialConfigData::ConfigStopbits1 == str)
  {
    bits = TtySerialPort::STOPBITS_1;
  }
  else if (TtySerialConfigData::ConfigStopbits2 == str)
  {
    bits = TtySerialPort::STOPBITS_2;
  }
  else
  {
    bits = TtySerialPort::STOPBITS_ERR;
  }

  return (bits);
}

bool
TtySerialPort::SetStopBits(TtySerialPort::STOPBITS sbits_)
{

  bool status = false;

  if (TtySerialPort::STOPBITS_1 == sbits_)
  {
    this->_termios.c_cflag &= ~CSTOPB;
    status = TtySerialConfigData::SetStopBits(TtySerialConfigData::ConfigStopbits1);
  }
  else if (TtySerialPort::STOPBITS_2 == sbits_)
  {
    this->_termios.c_cflag |= CSTOPB;
    status = TtySerialConfigData::SetStopBits(TtySerialConfigData::ConfigStopbits2);
  }

  return (status);

}

TtySerialPort::PARITY
TtySerialPort::GetParity() const
{
  TtySerialPort::PARITY parity = TtySerialPort::PARITY_DEF;
  std::string str = TtySerialConfigData::GetParity();

  if (TtySerialConfigData::ConfigParityNone == str)
  {
    parity = TtySerialPort::PARITY_NONE;
  }
  else if (TtySerialConfigData::ConfigParityEven == str)
  {
    parity = TtySerialPort::PARITY_EVEN;
  }
  else if (TtySerialConfigData::ConfigParityOdd == str)
  {
    parity = TtySerialPort::PARITY_ODD;
  }
  else
  {
    parity = TtySerialPort::PARITY_ERR;
  }

  return (parity);
}

bool
TtySerialPort::SetParity(TtySerialPort::PARITY parity_)
{

  bool status = false;

  if (TtySerialPort::PARITY_NONE == parity_)
  {
    this->_termios.c_cflag &= ~(PARENB | PARODD);
    status = TtySerialConfigData::SetParity(TtySerialConfigData::ConfigParityNone);
  }
  else if (TtySerialPort::PARITY_EVEN == parity_)
  {
    this->_termios.c_cflag &= ~(PARENB | PARODD);
    this->_termios.c_cflag |= (PARENB | PARODD);
    status = TtySerialConfigData::SetParity(TtySerialConfigData::ConfigParityEven);
  }
  else if (TtySerialPort::PARITY_ODD == parity_)
  {
    this->_termios.c_cflag &= ~(PARENB | PARODD);
    this->_termios.c_cflag |= PARENB;
    status = TtySerialConfigData::SetParity(TtySerialConfigData::ConfigParityOdd);
  }

  return (status);

}

TtySerialPort::FLOWCNTL
TtySerialPort::GetFlowControl() const
{
  TtySerialPort::FLOWCNTL flow = TtySerialPort::FLOWCNTL_DEF;
  std::string str = TtySerialConfigData::GetFlowControl();

  if (TtySerialConfigData::ConfigFlowCtrlNone == str)
  {
    flow = TtySerialPort::FLOWCNTL_NONE;
  }
  else if (TtySerialConfigData::ConfigFlowCtrlHard == str)
  {
    flow = TtySerialPort::FLOWCNTL_HARD;
  }
  else
  {
    flow = TtySerialPort::FLOWCNTL_ERR;
  }

  return (flow);
}

bool
TtySerialPort::SetFlowControl(TtySerialPort::FLOWCNTL flowcntl_)
{

  bool status = false;

  if (TtySerialPort::FLOWCNTL_NONE == flowcntl_)
  {
    this->_termios.c_cflag &= ~CRTSCTS;
    status = TtySerialConfigData::SetFlowControl(TtySerialConfigData::ConfigFlowCtrlNone);
  }
  else if (TtySerialPort::FLOWCNTL_HARD == flowcntl_)
  {
    this->_termios.c_cflag |= CRTSCTS;
    status = TtySerialConfigData::SetFlowControl(TtySerialConfigData::ConfigFlowCtrlHard);
  }

  return (status);

}

bool
TtySerialPort::GetBlocking() const
{
  return ((this->_options & O_NONBLOCK) != O_NONBLOCK);
}

bool
TtySerialPort::SetBlocking(bool blocking_)
{
  if (blocking_)
  {
    this->_options &= ~(O_NONBLOCK | O_NDELAY);
  }
  else
  {
    this->_options |= (O_NONBLOCK | O_NDELAY);
  }
  return (true);
}

}
}
