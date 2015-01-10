
#ifndef _ZLOGTEST_H_
#define _ZLOGTEST_H_

#include "UnitTest.h"
#include "zutils/zLog.h"


int
zLogTest_Defaults(void* arg_);
int
zLogTest_FileConnector(void* arg_);


using namespace zUtils;

class TestConnector : public zLog::Connector
{
public:

  virtual void
  Logger(std::string msg_)
  {
    this->logMsg += msg_;
  }
  std::string logMsg;
};



#endif /* _ZLOGTEST_H_ */
