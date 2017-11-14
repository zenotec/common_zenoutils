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

#ifndef _ZCONFTEST_H_
#define _ZCONFTEST_H_

#include <unistd.h>

#include <list>
#include <mutex>
#include <memory>

#include <zutils/zData.h>
#include <zutils/zEvent.h>
#include <zutils/zConfig.h>

#include "UnitTest.h"

int
zConfigTest_PathDefaults(void* arg_);
int
zConfigTest_DataDefaults(void* arg_);
int
zConfigTest_ConfigurationDefaults(void* arg_);
int
zConfigTest_ConnectorDefaults(void* arg_);

int
zConfigTest_ConfigDataCtor(void* arg_);
int
zConfigTest_ConfigDataGetPutValue(void* arg_);
int
zConfigTest_ConfigDataGetPutChild(void* arg_);

int
zConfigTest_FileLoadStore(void* arg_);

int
zConfigTest_ConfigurationCtor(void* arg_);
int
zConfigTest_ConfigurationGetSetData(void* arg_);
int
zConfigTest_ConfigurationCompare(void* arg_);
int
zConfigTest_ConfigurationDataArray(void* arg_);
int
zConfigTest_ConfigurationLoadStore(void* arg_);

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
    std::string fileName = TESTDIR + "/" + TESTFILE;

    unlink(fileName.c_str());

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
