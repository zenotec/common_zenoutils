//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Port.cpp
//    Description:
//
//*****************************************************************************

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include <iostream>

#include <zutils/zCom.h>

namespace zUtils
{
namespace zCom
{

//*****************************************************************************
// Port Class
//*****************************************************************************
Port::Port(const std::string& device_, Port::BAUD baud_,
    Port::DATABITS dbits_, Port::STOPBITS sbits_, Port::PARITY parity_,
    Port::FLOWCNTL flowcntl_) :
    _fd(0), _device(device_), _baud(Port::BAUD_NONE),
        _dbits(Port::DATABITS_NONE), _sbits(Port::STOPBITS_NONE),
        _parity(Port::PARITY_NONE), _flowcntl(Port::FLOWCNTL_NONE)
{
  // Setup terminal I/O structure
  memset(&this->_termios, 0, sizeof(this->_termios));
  cfmakeraw(&this->_termios);
  this->_termios.c_cflag |= (CLOCAL | CREAD);

  this->_mutex.Unlock();

  // Configure the port
  this->SetBaud(baud_);
  this->SetDataBits(dbits_);
  this->SetStopBits(sbits_);
  this->SetParity(parity_);
  this->SetFlowControl(flowcntl_);

}

Port::~Port()
{
  this->_close();
}

Port::BAUD
Port::GetBaud()
{
  return (this->_baud);
}

bool
Port::SetBaud(Port::BAUD baud_)
{
  bool status = false;

  this->_mutex.Lock();

  // Don't support changing baud after port is open
  if (!this->_fd)
  {

    // Switch on baud, error on invalid baud
    switch (baud_)
    {

    case Port::BAUD_230400:
      case Port::BAUD_115200:
      case Port::BAUD_57600:
      case Port::BAUD_38400:
      case Port::BAUD_19200:
      case Port::BAUD_9600:
      case Port::BAUD_4800:
      case Port::BAUD_2400:
      case Port::BAUD_1800:
      case Port::BAUD_1200:
      case Port::BAUD_600:
      case Port::BAUD_300:
      this->_baud = baud_;
      cfsetspeed(&this->_termios, baud_);
      status = true;
      break;

    default:
      this->_baud = Port::BAUD_ERR;

    }
  }

  this->_mutex.Unlock();

  return (status);

}

Port::DATABITS
Port::GetDataBits()
{
  return (this->_dbits);
}

bool
Port::SetDataBits(Port::DATABITS dbits_)
{

  bool status = false;

  this->_mutex.Lock();

  // Don't support changing data bits after port is open
  if (!this->_fd)
  {

    switch (dbits_)
    {
    case Port::DATABITS_5:
      this->_termios.c_cflag &= ~CSIZE;
      this->_termios.c_cflag |= CS5;
      this->_dbits = dbits_;
      status = true;
      break;
    case Port::DATABITS_6:
      this->_termios.c_cflag &= ~CSIZE;
      this->_termios.c_cflag |= CS6;
      this->_dbits = dbits_;
      status = true;
      break;
    case Port::DATABITS_7:
      this->_termios.c_cflag &= ~CSIZE;
      this->_termios.c_cflag |= CS7;
      this->_dbits = dbits_;
      status = true;
      break;
    case Port::DATABITS_8:
      this->_termios.c_cflag &= ~CSIZE;
      this->_termios.c_cflag |= CS8;
      this->_dbits = dbits_;
      status = true;
      break;
    default:
      this->_dbits = Port::DATABITS_ERR;
      status = false;
    }
  }

  this->_mutex.Unlock();

  return (status);

}

Port::STOPBITS
Port::GetStopBits()
{
  return (this->_sbits);
}

bool
Port::SetStopBits(Port::STOPBITS sbits_)
{

  bool status = false;

  this->_mutex.Lock();

  // Don't support changing stop bits after port is open
  if (!this->_fd)
  {

    switch (sbits_)
    {
    case Port::STOPBITS_1:
      this->_termios.c_cflag &= ~CSTOPB;
      this->_sbits = sbits_;
      status = true;
      break;
    case Port::STOPBITS_2:
      this->_termios.c_cflag |= CSTOPB;
      this->_sbits = sbits_;
      status = true;
      break;
    default:
      this->_sbits = Port::STOPBITS_ERR;
      status = false;
    }

  }

  this->_mutex.Unlock();

  return (status);

}

Port::PARITY
Port::GetParity()
{
  return (this->_parity);
}

bool
Port::SetParity(Port::PARITY parity_)
{

  bool status = false;

  this->_mutex.Lock();

  // Do not support changing parity after port is open
  if (!this->_fd)
  {

    switch (parity_)
    {
    case Port::PARITY_NONE:
      this->_termios.c_cflag &= ~(PARENB | PARODD);
      this->_parity = parity_;
      status = true;
      break;
    case Port::PARITY_ODD:
      this->_termios.c_cflag &= ~(PARENB | PARODD);
      this->_termios.c_cflag |= (PARENB | PARODD);
      this->_parity = parity_;
      status = true;
      break;
    case Port::PARITY_EVEN:
      this->_termios.c_cflag &= ~(PARENB | PARODD);
      this->_termios.c_cflag |= PARENB;
      this->_parity = parity_;
      status = true;
      break;
    default:
      this->_parity = Port::PARITY_ERR;
      status = false;
    }

  }

  this->_mutex.Unlock();

  return (status);

}

Port::FLOWCNTL
Port::GetFlowControl()
{
  return (this->_flowcntl);
}

bool
Port::SetFlowControl(Port::FLOWCNTL flowcntl_)
{

  bool status = false;

  this->_mutex.Lock();

  // Do not support changing flow control after port is open
  if (!this->_fd)
  {

    switch (flowcntl_)
    {
    case Port::FLOWCNTL_NONE:
      this->_termios.c_cflag &= ~CRTSCTS;
      this->_flowcntl = flowcntl_;
      status = true;
      break;
    case Port::FLOWCNTL_HARD:
      this->_termios.c_cflag |= CRTSCTS;
      this->_flowcntl = flowcntl_;
      status = true;
      break;
    default:
      this->_flowcntl = Port::FLOWCNTL_ERR;
      status = false;
    }
  }

  this->_mutex.Unlock();

  return (status);

}

bool
Port::_open()
{
  bool status = false;

  this->_mutex.Lock();
  if (!this->_fd)
  {
    this->_fd = open(this->_device.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK | O_NDELAY);
    if (this->_fd > 0)
    {
      tcgetattr(this->_fd, &this->_savedTermios);
      tcflush(this->_fd, TCIOFLUSH);
      if (tcsetattr(this->_fd, TCSANOW, &this->_termios) == 0)
      {
        status = true;
      }
    }
  }
  this->_mutex.Unlock();
  return (status);
}

bool
Port::_close()
{
  bool status = false;
  this->_mutex.Lock();
  if (this->_fd)
  {
    tcsetattr(this->_fd, TCSANOW, &this->_savedTermios);
    close(this->_fd);
    this->_fd = 0;
    status = true;
  }
  this->_mutex.Unlock();
  return (status);
}

ssize_t
Port::RecvBuf(void* buf_, uint32_t len_, uint32_t usecs_)
{
  fd_set rxFdSet;
  struct timeval to = { 0 };
  struct timeval *to_p = (usecs_) ? &to : 0;
  ssize_t bytes = -1;

  this->_mutex.Lock();
  if (this->_fd)
  {

    FD_ZERO(&rxFdSet);
    FD_SET(this->_fd, &rxFdSet);
    to.tv_sec = 0;
    to.tv_usec = usecs_;

    int ret = select(this->_fd + 1, &rxFdSet, NULL, NULL, to_p);
    if (ret == 1 && FD_ISSET(this->_fd, &rxFdSet))
    {
      bytes = read(this->_fd, buf_, len_);
    }
  }
  this->_mutex.Unlock();
  return (bytes);
}

bool
Port::RecvChar(char* c_, uint32_t usecs_)
{
  return (this->RecvBuf(c_, 1, usecs_) == 1);
}

ssize_t
Port::SendBuf(const void* buf_, uint32_t len_, uint32_t usecs_)
{
  fd_set txFdSet;
  struct timeval to = { 0 };
  struct timeval *to_p = (usecs_) ? &to : 0;
  ssize_t bytes = -1;

  this->_mutex.Lock();
  if (!this->_fd)
  {
    this->_mutex.Unlock();
    return (false);
  }

  FD_ZERO(&txFdSet);
  FD_SET(this->_fd, &txFdSet);
  to.tv_sec = 0;
  to.tv_usec = usecs_;

  int ret = select(this->_fd + 1, NULL, &txFdSet, NULL, to_p);
  if (ret == 1 && FD_ISSET(this->_fd, &txFdSet))
  {
    bytes = write(this->_fd, buf_, len_);
  }
  this->_mutex.Unlock();
  return (bytes);
}

bool
Port::SendChar(const char c_, uint32_t usecs_)
{
  return (this->SendBuf(&c_, 1, usecs_) == 1);
}

bool
Port::SendString(const std::string& str_, uint32_t usecs_)
{
  return (this->SendBuf(str_.c_str(), str_.size(), usecs_) == str_.size());
}

}
}
