/*
 * zConfTest.h
 *
 *  Created on: Jan 11, 2015
 *      Author: kmahoney
 */

#ifndef _ZCONFTEST_H_
#define _ZCONFTEST_H_

#include "UnitTest.h"

#include <zutils/zConf.h>

int
zConfTest_DataDefaults( void* arg_ );
int
zConfTest_ConnectorDefaults( void* arg_ );
int
zConfTest_DataGetSetKeyValue( void* arg_ );
int
zConfTest_DataGetSetData( void* arg_ );
int
zConfTest_DataCompare( void* arg_ );
int
zConfTest_DataLoadStore( void* arg_ );
int
zConfTest_FileLoadStore( void* arg_ );

using namespace Test;
using namespace zUtils;

static const std::string TESTDIR = "/tmp/zConfTest";
static const std::string TESTFILE = "test.conf";

class TestConnector: public zConf::FileConnector
{

public:
    TestConnector() :
            zConf::FileConnector( TESTDIR + "/" + TESTFILE )
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
        fs.open( fileName.c_str(), std::fstream::in );
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

class TestObserver: public zConf::Observer
{

public:

    TestObserver();

    virtual
    ~TestObserver();

    virtual void
    Prepare( zConf::Data *item_ )
    {

    }

    virtual void
    Commit( zConf::Data *item_ )
    {

    }

private:
};

#endif /* _ZCONFTEST_H_ */
