//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: TtyPort.cpp
//    Description:
//
//*****************************************************************************

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include <iostream>

#include <zutils/zTty.h>

namespace zUtils
{
namespace zCom
{

//*****************************************************************************
// TtyPort Class
//*****************************************************************************
TtyPort::TtyPort(TtyPort::BAUD baud_, TtyPort::DATABITS dbits_, TtyPort::STOPBITS sbits_,
    TtyPort::PARITY parity_, TtyPort::FLOWCNTL flowcntl_, bool blocking_) :
    _rx_thread(&this->_rx_func, this), _tx_thread(&this->_tx_func, this), _fd(0),
        _baud(TtyPort::BAUD_NONE), _dbits(TtyPort::DATABITS_NONE), _sbits(TtyPort::STOPBITS_NONE),
        _parity(TtyPort::PARITY_NONE), _flowcntl(TtyPort::FLOWCNTL_NONE), _options(0)
{
  // Setup terminal I/O structure
  memset(&this->_termios, 0, sizeof(this->_termios));
  cfmakeraw(&this->_termios);
  this->_termios.c_cflag |= (CLOCAL | CREAD);

  // Configure the port
  this->SetBaud(baud_);
  this->SetDataBits(dbits_);
  this->SetStopBits(sbits_);
  this->SetParity(parity_);
  this->SetFlowControl(flowcntl_);
  this->SetBlocking(blocking_);

}

TtyPort::~TtyPort()
{
}

bool
TtyPort::Open(const std::string &dev_)
{
  bool status = false;

  if (!this->_fd)
  {
    this->_fd = open(dev_.c_str(), O_RDWR | O_NOCTTY | this->_options);
    if (this->_fd > 0)
    {
      tcgetattr(this->_fd, &this->_savedTermios);
      tcflush(this->_fd, TCIOFLUSH);
      if (tcsetattr(this->_fd, TCSANOW, &this->_termios) == 0)
      {
        if (this->_rx_thread.Run() && this->_tx_thread.Run())
        {
          this->_device = dev_;
          status = true;
        }
      }
    }
  }
  return (status);
}

void
TtyPort::Close()
{
  if (this->_fd)
  {
    this->_rx_thread.Join();
    this->_tx_thread.Join();
    tcsetattr(this->_fd, TCSANOW, &this->_savedTermios);
    close(this->_fd);
    this->_fd = 0;
  }
  return;
}

TtyPort::BAUD
TtyPort::GetBaud()
{
  return (this->_baud);
}

bool
TtyPort::SetBaud(TtyPort::BAUD baud_)
{
  bool status = false;

  // Don't support changing baud after port is open
  if (!this->_fd)
  {

    // Switch on baud, error on invalid baud
    switch (baud_)
    {

    case TtyPort::BAUD_230400:
      case TtyPort::BAUD_115200:
      case TtyPort::BAUD_57600:
      case TtyPort::BAUD_38400:
      case TtyPort::BAUD_19200:
      case TtyPort::BAUD_9600:
      case TtyPort::BAUD_4800:
      case TtyPort::BAUD_2400:
      case TtyPort::BAUD_1800:
      case TtyPort::BAUD_1200:
      case TtyPort::BAUD_600:
      case TtyPort::BAUD_300:
      this->_baud = baud_;
      cfsetspeed(&this->_termios, baud_);
      status = true;
      break;

    default:
      this->_baud = TtyPort::BAUD_ERR;

    }
  }

  return (status);

}

TtyPort::DATABITS
TtyPort::GetDataBits()
{
  return (this->_dbits);
}

bool
TtyPort::SetDataBits(TtyPort::DATABITS dbits_)
{

  bool status = false;

  // Don't support changing data bits after port is open
  if (!this->_fd)
  {

    switch (dbits_)
    {
    case TtyPort::DATABITS_5:
      this->_termios.c_cflag &= ~CSIZE;
      this->_termios.c_cflag |= CS5;
      this->_dbits = dbits_;
      status = true;
      break;
    case TtyPort::DATABITS_6:
      this->_termios.c_cflag &= ~CSIZE;
      this->_termios.c_cflag |= CS6;
      this->_dbits = dbits_;
      status = true;
      break;
    case TtyPort::DATABITS_7:
      this->_termios.c_cflag &= ~CSIZE;
      this->_termios.c_cflag |= CS7;
      this->_dbits = dbits_;
      status = true;
      break;
    case TtyPort::DATABITS_8:
      this->_termios.c_cflag &= ~CSIZE;
      this->_termios.c_cflag |= CS8;
      this->_dbits = dbits_;
      status = true;
      break;
    default:
      this->_dbits = TtyPort::DATABITS_ERR;
      status = false;
    }
  }

  return (status);

}

TtyPort::STOPBITS
TtyPort::GetStopBits()
{
  return (this->_sbits);
}

bool
TtyPort::SetStopBits(TtyPort::STOPBITS sbits_)
{

  bool status = false;

  // Don't support changing stop bits after port is open
  if (!this->_fd)
  {

    switch (sbits_)
    {
    case TtyPort::STOPBITS_1:
      this->_termios.c_cflag &= ~CSTOPB;
      this->_sbits = sbits_;
      status = true;
      break;
    case TtyPort::STOPBITS_2:
      this->_termios.c_cflag |= CSTOPB;
      this->_sbits = sbits_;
      status = true;
      break;
    default:
      this->_sbits = TtyPort::STOPBITS_ERR;
      status = false;
    }

  }

  return (status);

}

TtyPort::PARITY
TtyPort::GetParity()
{
  return (this->_parity);
}

bool
TtyPort::SetParity(TtyPort::PARITY parity_)
{

  bool status = false;

  // Do not support changing parity after port is open
  if (!this->_fd)
  {

    switch (parity_)
    {
    case TtyPort::PARITY_NONE:
      this->_termios.c_cflag &= ~(PARENB | PARODD);
      this->_parity = parity_;
      status = true;
      break;
    case TtyPort::PARITY_ODD:
      this->_termios.c_cflag &= ~(PARENB | PARODD);
      this->_termios.c_cflag |= (PARENB | PARODD);
      this->_parity = parity_;
      status = true;
      break;
    case TtyPort::PARITY_EVEN:
      this->_termios.c_cflag &= ~(PARENB | PARODD);
      this->_termios.c_cflag |= PARENB;
      this->_parity = parity_;
      status = true;
      break;
    default:
      this->_parity = TtyPort::PARITY_ERR;
      status = false;
    }

  }

  return (status);

}

TtyPort::FLOWCNTL
TtyPort::GetFlowControl()
{
  return (this->_flowcntl);
}

bool
TtyPort::SetFlowControl(TtyPort::FLOWCNTL flowcntl_)
{

  bool status = false;

  // Do not support changing flow control after port is open
  if (!this->_fd)
  {

    switch (flowcntl_)
    {
    case TtyPort::FLOWCNTL_NONE:
      this->_termios.c_cflag &= ~CRTSCTS;
      this->_flowcntl = flowcntl_;
      status = true;
      break;
    case TtyPort::FLOWCNTL_HARD:
      this->_termios.c_cflag |= CRTSCTS;
      this->_flowcntl = flowcntl_;
      status = true;
      break;
    default:
      this->_flowcntl = TtyPort::FLOWCNTL_ERR;
      status = false;
    }
  }

  return (status);

}

bool
TtyPort::GetBlocking()
{
  return ((this->_options & O_NONBLOCK) != O_NONBLOCK);
}

bool
TtyPort::SetBlocking(bool blocking_)
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

void *
TtyPortRecv::ThreadFunction(void *arg_)
{
  TtyPort *port = (TtyPort *) arg_;

  ZLOG_DEBUG("RX: Polling TTY for data");

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = port->_fd;
  fds[0].events = (POLLIN | POLLERR);
  int ret = poll(fds, 1, 100);
  if (ret > 0 && (fds[0].revents == POLLIN))
  {
    char c = 0;
    if ((read(port->_fd, &c, 1) == 1))
    {
      printf("1:%c\n", c);
      ZLOG_DEBUG(std::string("Received char: '") + zLog::IntStr(c) + ": " + c + "'");
      port->rxq.Push(c);
      port->rx_event.Notify(port);
    }
  }

  return (0);
}

void *
TtyPortSend::ThreadFunction(void *arg_)
{
  TtyPort *port = (TtyPort *) arg_;

  ZLOG_DEBUG("TX: Polling TTY for data");

  // Setup for poll loop
  struct pollfd fds[1];
  fds[0].fd = port->_fd;
  fds[0].events = (POLLOUT | POLLERR);
  int ret = poll(fds, 1, 100);
  if (ret > 0 && (fds[0].revents == POLLOUT) && port->txq.TimedWait(100000))
  {
    char c = port->txq.Front();
    ZLOG_DEBUG(std::string("Sending byte: '") + c + "'");
    if ((write(port->_fd, &c, 1) == 1))
    {
      ZLOG_DEBUG(std::string("Byte Sent: '") + c + "'");
      port->txq.Pop();
      port->tx_event.Notify(port);
    }
  }

  return (0);
}

}
}
