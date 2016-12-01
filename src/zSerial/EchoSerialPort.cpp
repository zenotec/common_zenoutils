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

#include <zutils/zLog.h>
#include <zutils/zEchoSerialPort.h>

namespace zUtils
{
namespace zSerial
{

EchoSerialPort::EchoSerialPort() :
    _thread(this, NULL), _opened(false)
{
}

EchoSerialPort::~EchoSerialPort()
{
}

bool
EchoSerialPort::Open()
{
  if (!this->_opened && this->_thread.Start())
  {
    this->_opened = true;
  }
  return (this->_opened == true);
}

bool
EchoSerialPort::Close()
{
  if (this->_opened && this->_thread.Stop())
  {
    this->_opened = false;
  }
  return (this->_opened == false);
}

void
EchoSerialPort::Run(zThread::ThreadArg *arg_)
{

  char c = 0;

  while (!this->Exit())
  {
    if (this->txchar(&c, 100000))
    {
      this->rxchar(c);
    }
  }

  return;
}

}
}
