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

#ifndef __ZECHOSERIALPORT_H__
#define __ZECHOSERIALPORT_H__

#include <zutils/zThread.h>
#include <zutils/zSerial.h>

namespace zUtils
{
namespace zSerial
{

//**********************************************************************
// Class: EchoSerialPort
//**********************************************************************

class EchoSerialPort :
    public SerialPort, public zThread::ThreadFunction, public zThread::ThreadArg
{

public:

  EchoSerialPort();

  virtual
  ~EchoSerialPort();

  virtual bool
  Open();

  virtual bool
  Close();

  virtual void
  Run(zThread::ThreadArg *arg_);

protected:

private:

  zThread::Thread _thread;
  bool _opened;

};

}
}

#endif /* __ZECHOSERIALPORT_H__ */
