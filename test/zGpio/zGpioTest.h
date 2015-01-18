/*
 * GpioHandlerTest.h
 *
 *  Created on: Dec 23, 2014
 *      Author: freewave
 */

#ifndef COMHANDLERTEST_H_
#define COMHANDLERTEST_H_

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sstream>
#include <fstream>

#include <zutils/zGpio.h>

#include "UnitTest.h"

int
zGpioTest_PortDefaults(void* arg);
int
zGpioTest_HandlerDefaults(void* arg);
int
zGpioTest_HandlerAddRemove(void* arg);
int
zGpioTest_HandlerOnOff(void* arg);

using namespace zUtils;
using namespace Test;

static const std::string TESTDIR = "/tmp/GpioHandlerTest/";

class TestGpioPort : public zGpio::Port
{
public:
  TestGpioPort(uint32_t id_, zGpio::Port::DIR dir_ = zGpio::Port::DIR_DEF,
      zGpio::Port::STATE initialState_ = zGpio::Port::STATE_DEF) :
        zGpio::Port(id_, dir_, initialState_, TESTDIR)
  {
    struct stat st = { 0 };
    std::fstream fs;
    std::string fileName;

    if (stat(this->_getRootDir().c_str(), &st) == -1)
    {
      mkdir(this->_getRootDir().c_str(), 0777);
    }

    if (stat(this->_getClassDir().c_str(), &st) == -1)
    {
      mkdir(this->_getClassDir().c_str(), 0777);
    }

    fileName = this->_getRootDir() + zGpio::Port::EXPORT_FILENAME;
    fs.open(fileName.c_str(), std::fstream::out);
    fs << "" << std::endl;
    fs.flush();
    fs.close();

    fileName = this->_getRootDir() + zGpio::Port::UNEXPORT_FILENAME;
    fs.open(fileName.c_str(), std::fstream::out);
    fs << "" << std::endl;
    fs.flush();
    fs.close();

    fileName = this->_getClassDir() + zGpio::Port::STATE_FILENAME;
    fs.open(fileName.c_str(), std::fstream::out);
    if (initialState_ == zGpio::Port::STATE_ACTIVE)
    {
      fs << zGpio::Port::STATE_ACTIVE_STR << std::endl;
    }
    else if (initialState_ == zGpio::Port::STATE_INACTIVE)
    {
      fs << zGpio::Port::STATE_INACTIVE_STR << std::endl;
    }
    else
    {
      fs << "" << std::endl;
    }
    fs.flush();
    fs.close();

    fileName = this->_getClassDir() + zGpio::Port::DIR_FILENAME;
    fs.open(fileName.c_str(), std::fstream::out);
    if (dir_ == zGpio::Port::DIR_IN)
    {
      fs << zGpio::Port::DIR_IN_STR << std::endl;
    }
    else if (dir_ == zGpio::Port::DIR_OUT)
    {
      fs << zGpio::Port::DIR_OUT_STR << std::endl;
    }
    else
    {
      fs << "" << std::endl;
    }

    fs.flush();
    fs.close();

  }

  virtual
  ~TestGpioPort()
  {
    struct stat st = { 0 };
    std::fstream fs;
    std::string fileName;

    fileName = this->_getRootDir() + zGpio::Port::EXPORT_FILENAME;
    unlink(fileName.c_str());

    fileName = this->_getRootDir() + zGpio::Port::UNEXPORT_FILENAME;
    unlink(fileName.c_str());

    fileName = this->_getClassDir() + zGpio::Port::STATE_FILENAME;
    unlink(fileName.c_str());

    fileName = this->_getClassDir() + zGpio::Port::DIR_FILENAME;
    unlink(fileName.c_str());

    if (stat(this->_getRootDir().c_str(), &st) != -1)
    {
      rmdir(this->_getRootDir().c_str());
    }

    if (stat(this->_getClassDir().c_str(), &st) != -1)
    {
      rmdir(this->_getClassDir().c_str());
    }

  }

  zGpio::Port::DIR
  GetDirection()
  {
    return (this->_getDirection());
  }
  bool
  SetDirection(zGpio::Port::DIR dir_)
  {
    return (this->_setDirection(dir_));
  }

  zGpio::Port::STATE
  GetState()
  {
    return (this->_getState());
  }
  bool
  SetState(zGpio::Port::STATE state_)
  {
    return (this->_setState(state_));
  }

protected:

private:
};

#endif /* COMHANDLERTEST_H_ */
