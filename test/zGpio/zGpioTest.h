
#ifndef GPIOHANDLERTEST_H_
#define GPIOHANDLERTEST_H_

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <sstream>
#include <fstream>

#include <zutils/zGpio.h>

#include "UnitTest.h"

int
zGpioTest_ConfigurationDefaults(void* arg);
int
zGpioTest_ConfigurationCtor(void* arg);
int
zGpioTest_ConfigurationGetSet(void* arg);

int
zGpioTest_PortDefaults(void* arg);
int
zGpioTest_PortGetSet(void* arg);

int
zGpioTest_HandlerDefaults(void* arg);
int
zGpioTest_HandlerAddPort(void* arg);
int
zGpioTest_HandlerOnOff(void* arg);

int
zGpioTest_SwitchDefaults(void* arg);
int
zGpioTest_SwitchOnOff(void* arg);

using namespace zUtils;
using namespace Test;

static const std::string TESTDIR = "/tmp/zGpioHandlerTest";

class TestPortConfig : public zGpio::GpioConfiguration
{
public:
  TestPortConfig(uint32_t id_)
  {

    struct stat st = { 0 };
    std::fstream fs;
    std::string fileName;
    std::stringstream rootDir;

    this->Identifier(id_);
    this->ExportFilename(TESTDIR + "/export");
    this->UnexportFilename(TESTDIR + "/unexport");
    this->DirectionFilename(TESTDIR + "/gpio%d/direction");
    this->StateFilename(TESTDIR + "/gpio%d/value");
    this->EdgeFilename(TESTDIR + "/gpio%d/edge");

    rootDir << TESTDIR;

    if (stat(rootDir.str().c_str(), &st) == -1)
    {
      mkdir(rootDir.str().c_str(), 0777);
    }

    fileName = rootDir.str() + "/export";
    fs.open(fileName.c_str(), std::fstream::out);
    fs << "" << std::endl;
    fs.flush();
    fs.close();

    fileName = rootDir.str() + "/unexport";
    fs.open(fileName.c_str(), std::fstream::out);
    fs << "" << std::endl;
    fs.flush();
    fs.close();

    rootDir << "/gpio" << id_;
    if (stat(rootDir.str().c_str(), &st) == -1)
    {
      mkdir(rootDir.str().c_str(), 0777);
    }

    fileName = rootDir.str() + "/direction";
    fs.open(fileName.c_str(), std::fstream::out);
    fs << zGpio::GpioConfiguration::ConfigDirectionValueDefault << std::endl;
    fs.flush();
    fs.close();

    fileName = rootDir.str() + "/value";
    fs.open(fileName.c_str(), std::fstream::out);
    fs << zGpio::GpioConfiguration::ConfigStateValueDefault << std::endl;
    fs.flush();
    fs.close();

    fileName = rootDir.str() + "/edge";
    fs.open(fileName.c_str(), std::fstream::out);
    fs << zGpio::GpioConfiguration::ConfigEdgeValueDefault << std::endl;
    fs.flush();
    fs.close();

  }

  virtual
  ~TestPortConfig()
  {
    struct stat st = { 0 };
    int id;
    std::stringstream gpio_id;
    std::fstream fs;
    std::string fileName;

    this->Identifier(id);
    gpio_id << "/gpio" << id;

    fileName = TESTDIR + "/export";
    unlink(fileName.c_str());

    fileName = TESTDIR + "/unexport";
    unlink(fileName.c_str());

    fileName = TESTDIR + gpio_id.str() + "/direction";
    unlink(fileName.c_str());

    fileName = TESTDIR + gpio_id.str() + "/value";
    unlink(fileName.c_str());

    fileName = TESTDIR + gpio_id.str() + "/edge";
    unlink(fileName.c_str());

    fileName = TESTDIR + gpio_id.str();
    if (stat(fileName.c_str(), &st) != -1)
    {
      rmdir(fileName.c_str());
    }

    if (stat(TESTDIR.c_str(), &st) != -1)
    {
      rmdir(TESTDIR.c_str());
    }

  }

protected:

private:

};

class TestObserver : public zEvent::EventObserver
{
public:
  TestObserver()
  {
  }

  virtual
  ~TestObserver()
  {
  }

  zSem::Semaphore HiSem;
  zSem::Semaphore LoSem;
  zSem::Semaphore ErrSem;

protected:

  virtual bool
  EventHandler(const zEvent::EventNotification* notification_)
  {
    bool status = false;
    if (notification_ && (notification_->Type() == zEvent::Event::TYPE_GPIO))
    {
      zGpio::GpioNotification *n = (zGpio::GpioNotification *)notification_;
      if (n->State() == zGpio::GpioPort::STATE_ACTIVE)
      {
        this->HiSem.Post();
      }
      else if (n->State() == zGpio::GpioPort::STATE_INACTIVE)
      {
        this->LoSem.Post();
      }
      else
      {
        this->ErrSem.Post();
      }
    }
    return (status);
  }

private:


};

#endif /* GPIOHANDLERTEST_H_ */
