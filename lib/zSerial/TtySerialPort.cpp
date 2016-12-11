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

//*****************************************************************************
// Class: TtySerialConfiguration
//*****************************************************************************

const std::string TtySerialConfiguration::ConfigDevicePath("Device");

const std::string TtySerialConfiguration::ConfigBaudPath("Baud");
const std::string TtySerialConfiguration::ConfigBaud9600("9600");
const std::string TtySerialConfiguration::ConfigBaud19200("19200");
const std::string TtySerialConfiguration::ConfigBaud38400("38400");
const std::string TtySerialConfiguration::ConfigBaud57600("57600");
const std::string TtySerialConfiguration::ConfigBaud115200("115200");
const std::string TtySerialConfiguration::ConfigBaudDefault(ConfigBaud115200);

const std::string TtySerialConfiguration::ConfigDatabitsPath("Databits");
const std::string TtySerialConfiguration::ConfigDatabits5("5");
const std::string TtySerialConfiguration::ConfigDatabits6("6");
const std::string TtySerialConfiguration::ConfigDatabits7("7");
const std::string TtySerialConfiguration::ConfigDatabits8("8");
const std::string TtySerialConfiguration::ConfigDatabitsDefault(ConfigDatabits8);

const std::string TtySerialConfiguration::ConfigStopbitsPath("Stopbits");
const std::string TtySerialConfiguration::ConfigStopbits1("1");
const std::string TtySerialConfiguration::ConfigStopbits2("2");
const std::string TtySerialConfiguration::ConfigStopbitsDefault(ConfigStopbits1);

const std::string TtySerialConfiguration::ConfigParityPath("Parity");
const std::string TtySerialConfiguration::ConfigParityNone("None");
const std::string TtySerialConfiguration::ConfigParityOdd("Odd");
const std::string TtySerialConfiguration::ConfigParityEven("Even");
const std::string TtySerialConfiguration::ConfigParityDefault(ConfigParityNone);

const std::string TtySerialConfiguration::ConfigFlowPath("Flow");
const std::string TtySerialConfiguration::ConfigFlowNone("None");
const std::string TtySerialConfiguration::ConfigFlowHard("Hard");
const std::string TtySerialConfiguration::ConfigFlowDefault(ConfigFlowNone);

TtySerialConfiguration::TtySerialConfiguration()
{
  this->Type(SerialConfiguration::ConfigTypeTty);
  this->SetDevice(this->GetDevice());
  this->SetBaud(this->GetBaud());
  this->SetDataBits(this->GetDataBits());
  this->SetStopBits(this->GetStopBits());
  this->SetParity(this->GetParity());
  this->SetFlowControl(this->GetFlowControl());
}

TtySerialConfiguration::TtySerialConfiguration(zData::Data &data_) :
    SerialConfiguration(data_)
{
  this->Type(SerialConfiguration::ConfigTypeTty);
  this->SetDevice(this->GetDevice());
  this->SetBaud(this->GetBaud());
  this->SetDataBits(this->GetDataBits());
  this->SetStopBits(this->GetStopBits());
  this->SetParity(this->GetParity());
  this->SetFlowControl(this->GetFlowControl());
}

TtySerialConfiguration::TtySerialConfiguration(zConfig::Configuration &config_) :
    SerialConfiguration(config_)
{
  this->Type(SerialConfiguration::ConfigTypeTty);
  this->SetDevice(this->GetDevice());
  this->SetBaud(this->GetBaud());
  this->SetDataBits(this->GetDataBits());
  this->SetStopBits(this->GetStopBits());
  this->SetParity(this->GetParity());
  this->SetFlowControl(this->GetFlowControl());
}

TtySerialConfiguration::~TtySerialConfiguration()
{
}

std::string
TtySerialConfiguration::GetDevice() const
{
  std::string str;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot, ConfigDevicePath);
  this->Get(str, path);
  return (str);
}

bool
TtySerialConfiguration::SetDevice(const std::string& dev_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot, ConfigDevicePath);
  if (this->Put(dev_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
TtySerialConfiguration::GetBaud() const
{
  std::string str;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot, ConfigBaudPath);
  if (!this->Get(str, path))
  {
    str = ConfigBaudDefault;
  }
  return (str);
}

bool
TtySerialConfiguration::SetBaud(const std::string& baud_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot, ConfigBaudPath);
  if (this->Put(baud_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
TtySerialConfiguration::GetDataBits() const
{
  std::string str;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot, ConfigDatabitsPath);
  if (!this->Get(str, path))
  {
    str = ConfigDatabitsDefault;
  }
  return (str);
}

bool
TtySerialConfiguration::SetDataBits(const std::string& dbits_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot, ConfigDatabitsPath);
  if (this->Put(dbits_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
TtySerialConfiguration::GetStopBits() const
{
  std::string str;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot, ConfigStopbitsPath);
  if (!this->Get(str, path))
  {
    str = ConfigStopbitsDefault;
  }
  return (str);
}

bool
TtySerialConfiguration::SetStopBits(const std::string& sbits_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot, ConfigStopbitsPath);
  if (this->Put(sbits_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
TtySerialConfiguration::GetParity() const
{
  std::string str;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot, ConfigParityPath);
  if (!this->Get(str, path))
  {
    str = ConfigParityDefault;
  }
  return (str);
}

bool
TtySerialConfiguration::SetParity(const std::string parity_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot, ConfigParityPath);
  if (this->Put(parity_, path))
  {
    status = this->Commit();
  }
  return (status);
}

std::string
TtySerialConfiguration::GetFlowControl() const
{
  std::string str;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot, ConfigFlowPath);
  if (!this->Get(str, path))
  {
    str = ConfigFlowDefault;
  }
  return (str);
}

bool
TtySerialConfiguration::SetFlowControl(const std::string& flowcntl_)
{
  bool status = false;
  std::string path = zData::Data::PathConcat(SerialConfiguration::ConfigRoot, ConfigFlowPath);
  if (this->Put(flowcntl_, path))
  {
    status = this->Commit();
  }
  return (status);
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

TtySerialPort::TtySerialPort(const TtySerialConfiguration& config_) :
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

    this->_fd = open(this->_config.GetDevice().c_str(), O_RDWR | O_NOCTTY | this->_options);
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
          "Cannot open TTY port " + this->_config.GetDevice() + ": " + std::string(strerror(errno)));
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

std::string
TtySerialPort::GetDevice() const
{
  return (this->_config.GetDevice());
}

bool
TtySerialPort::SetDevice(const std::string &dev_)
{
  return (this->_config.SetDevice(dev_));
}

TtySerialPort::BAUD
TtySerialPort::GetBaud() const
{
  TtySerialPort::BAUD baud = TtySerialPort::BAUD_DEF;
  std::string str = this->_config.GetBaud();

  if (TtySerialConfiguration::ConfigBaud9600 == str)
  {
    baud = TtySerialPort::BAUD_9600;
  }
  else if (TtySerialConfiguration::ConfigBaud19200 == str)
  {
    baud = TtySerialPort::BAUD_19200;
  }
  else if (TtySerialConfiguration::ConfigBaud38400 == str)
  {
    baud = TtySerialPort::BAUD_38400;
  }
  else if (TtySerialConfiguration::ConfigBaud57600 == str)
  {
    baud = TtySerialPort::BAUD_57600;
  }
  else if (TtySerialConfiguration::ConfigBaud115200 == str)
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
      status = this->_config.SetBaud(TtySerialConfiguration::ConfigBaud9600);
    }
  }
  else if (TtySerialPort::BAUD_19200 == baud_)
  {
    if (!cfsetspeed(&this->_termios, B19200))
    {
      status = this->_config.SetBaud(TtySerialConfiguration::ConfigBaud19200);
    }
  }
  else if (TtySerialPort::BAUD_38400 == baud_)
  {
    if (!cfsetspeed(&this->_termios, B38400))
    {
      status = this->_config.SetBaud(TtySerialConfiguration::ConfigBaud38400);
    }
  }
  else if (TtySerialPort::BAUD_57600 == baud_)
  {
    if (!cfsetspeed(&this->_termios, B57600))
    {
      status = this->_config.SetBaud(TtySerialConfiguration::ConfigBaud57600);
    }
  }
  else if (TtySerialPort::BAUD_115200 == baud_)
  {
    if (!cfsetspeed(&this->_termios, B115200))
    {
      status = this->_config.SetBaud(TtySerialConfiguration::ConfigBaud115200);
    }
  }

  return (status);

}

TtySerialPort::DATABITS
TtySerialPort::GetDataBits() const
{
  TtySerialPort::DATABITS bits = TtySerialPort::DATABITS_DEF;
  std::string str = this->_config.GetDataBits();

  if (TtySerialConfiguration::ConfigDatabits5 == str)
  {
    bits = TtySerialPort::DATABITS_5;
  }
  else if (TtySerialConfiguration::ConfigDatabits6 == str)
  {
    bits = TtySerialPort::DATABITS_6;
  }
  else if (TtySerialConfiguration::ConfigDatabits7 == str)
  {
    bits = TtySerialPort::DATABITS_7;
  }
  else if (TtySerialConfiguration::ConfigDatabits8 == str)
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
    status = this->_config.SetDataBits(TtySerialConfiguration::ConfigDatabits5);
  }
  else if (TtySerialPort::DATABITS_6 == dbits_)
  {
    this->_termios.c_cflag &= ~CSIZE;
    this->_termios.c_cflag |= CS6;
    status = this->_config.SetDataBits(TtySerialConfiguration::ConfigDatabits6);
  }
  else if (TtySerialPort::DATABITS_7 == dbits_)
  {
    this->_termios.c_cflag &= ~CSIZE;
    this->_termios.c_cflag |= CS7;
    status = this->_config.SetDataBits(TtySerialConfiguration::ConfigDatabits7);
  }
  else if (TtySerialPort::DATABITS_8 == dbits_)
  {
    this->_termios.c_cflag &= ~CSIZE;
    this->_termios.c_cflag |= CS8;
    status = this->_config.SetDataBits(TtySerialConfiguration::ConfigDatabits8);
  }

  return (status);

}

TtySerialPort::STOPBITS
TtySerialPort::GetStopBits() const
{
  TtySerialPort::STOPBITS bits = TtySerialPort::STOPBITS_DEF;
  std::string str = this->_config.GetStopBits();

  if (TtySerialConfiguration::ConfigStopbits1 == str)
  {
    bits = TtySerialPort::STOPBITS_1;
  }
  else if (TtySerialConfiguration::ConfigStopbits2 == str)
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
    status = this->_config.SetStopBits(TtySerialConfiguration::ConfigStopbits1);
  }
  else if (TtySerialPort::STOPBITS_2 == sbits_)
  {
    this->_termios.c_cflag |= CSTOPB;
    status = this->_config.SetStopBits(TtySerialConfiguration::ConfigStopbits2);
  }

  return (status);

}

TtySerialPort::PARITY
TtySerialPort::GetParity() const
{
  TtySerialPort::PARITY parity = TtySerialPort::PARITY_DEF;
  std::string str = this->_config.GetParity();

  if (TtySerialConfiguration::ConfigParityNone == str)
  {
    parity = TtySerialPort::PARITY_NONE;
  }
  else if (TtySerialConfiguration::ConfigParityEven == str)
  {
    parity = TtySerialPort::PARITY_EVEN;
  }
  else if (TtySerialConfiguration::ConfigParityOdd == str)
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
    status = this->_config.SetParity(TtySerialConfiguration::ConfigParityNone);
  }
  else if (TtySerialPort::PARITY_EVEN == parity_)
  {
    this->_termios.c_cflag &= ~(PARENB | PARODD);
    this->_termios.c_cflag |= (PARENB | PARODD);
    status = this->_config.SetParity(TtySerialConfiguration::ConfigParityEven);
  }
  else if (TtySerialPort::PARITY_ODD == parity_)
  {
    this->_termios.c_cflag &= ~(PARENB | PARODD);
    this->_termios.c_cflag |= PARENB;
    status = this->_config.SetParity(TtySerialConfiguration::ConfigParityOdd);
  }

  return (status);

}

TtySerialPort::FLOWCNTL
TtySerialPort::GetFlowControl() const
{
  TtySerialPort::FLOWCNTL flow = TtySerialPort::FLOWCNTL_DEF;
  std::string str = this->_config.GetFlowControl();

  if (TtySerialConfiguration::ConfigFlowNone == str)
  {
    flow = TtySerialPort::FLOWCNTL_NONE;
  }
  else if (TtySerialConfiguration::ConfigFlowHard == str)
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
    status = this->_config.SetFlowControl(TtySerialConfiguration::ConfigFlowNone);
  }
  else if (TtySerialPort::FLOWCNTL_HARD == flowcntl_)
  {
    this->_termios.c_cflag |= CRTSCTS;
    status = this->_config.SetFlowControl(TtySerialConfiguration::ConfigFlowHard);
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
