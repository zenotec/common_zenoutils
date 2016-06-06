/*
 * zConfTest.h
 *
 *  Created on: Jan 11, 2015
 *      Author: kmahoney
 */

#ifndef _ZCONFTEST_H_
#define _ZCONFTEST_H_

#include <list>
#include <mutex>
#include <memory>

#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>

#include "UnitTest.h"

int
zConfigTest_DataDefaults(void* arg_);
int
zConfigTest_ConnectorDefaults(void* arg_);
int
zConfigTest_HandlerDefaults(void* arg_);

int
zConfigTest_ConfigurationCtor(void* arg_);
int
zConfigTest_ConfigurationGetSetData(void* arg_);
int
zConfigTest_ConfigurationCompare(void* arg_);
int
zConfigTest_ConfigurationDataArray(void* arg_);

int
zConfigTest_FileLoadStore(void* arg_);

int
zConfigTest_HandlerLoadStore(void* arg_);

using namespace Test;
using namespace zUtils;

static const std::string TESTDIR = "/tmp/zConfigTest";
static const std::string TESTFILE = "test.conf";

class TestConnector : public zConfig::ConfigurationFileConnector
{

public:
  TestConnector() :
      zConfig::ConfigurationFileConnector(TESTDIR + "/" + TESTFILE)
  {
    struct stat st = { 0 };
    std::fstream fs;
    std::string fileName;

    if (stat(TESTDIR.c_str(), &st) == -1)
    {
      mkdir(TESTDIR.c_str(), 0777);
    }

  }

  virtual
  ~TestConnector()
  {
    struct stat st = { 0 };
    std::string fileName = TESTDIR + "/" + TESTFILE;

    unlink(fileName.c_str());

    if (stat(TESTDIR.c_str(), &st) != -1)
    {
      rmdir(TESTDIR.c_str());
    }

  }

  void
  Display()
  {
    std::fstream fs;
    std::stringstream json;
    std::string fileName = TESTDIR + "/" + TESTFILE;

    // Open configuration file and read into local string
    fs.open(fileName.c_str(), std::fstream::in);
    if (fs.is_open())
    {
      json << fs.rdbuf();
      fs.close();
      std::cout << std::endl << json.str() << std::endl;
    }

  }

protected:

private:

};

class TestObserver : public zEvent::EventObserver
{

public:

  TestObserver();

  virtual
  ~TestObserver();

  virtual void
  Prepare(zConfig::Configuration *item_)
  {

  }

  virtual void
  Commit(zConfig::Configuration *item_)
  {

  }

private:
};

#endif /* _ZCONFTEST_H_ */
