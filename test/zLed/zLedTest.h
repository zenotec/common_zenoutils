#ifndef _ZLEDTEST_H_
#define _ZLEDTEST_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <string>

#include <zutils/zLed.h>

#include "UnitTest.h"

int
zLedTest_LedDefaults(void* arg);
int
zLedTest_HandlerDefaults(void* arg);
int
zLedTest_HandlerAddRemove(void* arg);
int
zLedTest_HandlerOnOffToggle(void* arg);

using namespace zUtils;
using namespace Test;

static const std::string TESTDIR = "/tmp/LedClassTest";

class LedTest : public zLed::Led
{
public:
  LedTest(const std::string& name_, zLed::Led::COLOR color_ = zLed::Led::COLOR_NONE,
      zLed::Led::MODE mode_ = zLed::Led::MODE_NONE, zLed::Led::STATE defState_ =
          zLed::Led::STATE_OFF) :
      Led(name_, color_, mode_, defState_, TESTDIR)
  {
    struct stat st = { 0 };
    std::fstream fs;

    this->_devRoot = TESTDIR + std::string("/") + this->GetName() + std::string("/");
    if (stat(this->_devRoot.c_str(), &st) == -1)
    {
      mkdir(this->_devRoot.c_str(), 0777);
    }

    std::string triggerFile = this->_devRoot + std::string("trigger");
    fs.open(triggerFile.c_str(), std::fstream::out);
    fs << "" << std::endl;
    fs.flush();
    fs.close();

    std::string brightFile = this->_devRoot + std::string("brightness");
    fs.open(brightFile.c_str(), std::fstream::out);
    fs << "" << std::endl;
    fs.flush();
    fs.close();

    std::string shotFile = this->_devRoot + std::string("shot");
    fs.open(shotFile.c_str(), std::fstream::out);
    fs << "" << std::endl;
    fs.flush();
    fs.close();
  }

  ~LedTest()
  {
    struct stat st = { 0 };
    std::fstream fs;

    std::string triggerFile = this->_devRoot + std::string("trigger");
    unlink(triggerFile.c_str());

    std::string brightFile = this->_devRoot + std::string("brightness");
    unlink(brightFile.c_str());

    std::string shotFile = this->_devRoot + std::string("shot");
    unlink(shotFile.c_str());

    if (stat(this->_devRoot.c_str(), &st) != -1)
    {
      rmdir(this->_devRoot.c_str());
    }
    this->_devRoot = "";
  }

  std::string
  GetDevRoot()
  {
    return (this->_getDevRoot());
  }

  bool
  SetMode(zLed::Led::MODE mode_)
  {
    return (this->_setMode(mode_));
  }

  bool
  SetState(zLed::Led::STATE state_)
  {
    return (this->_setState(state_));
  }

  bool
  TestRealState(zLed::Led::STATE state_)
  {

    // Read the action file and compare to expected
    std::ifstream fs;
    int state = 0;
    fs.open(this->_getActionFile().c_str());
    if (!fs.is_open())
    {
      return (false);
    }
    fs >> state;
    fs.close();

    return ((zLed::Led::STATE) state == state_);

  }

protected:
private:
  std::string _devRoot;
};

#endif /* _ZLEDTEST_H_ */
