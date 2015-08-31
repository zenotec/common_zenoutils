/*
 * GpioHandlerTest.h
 *
 *  Created on: Dec 23, 2014
 *      Author: freewave
 */

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
zGpioTest_PortDefaults( void* arg );
int
zGpioTest_HandlerDefaults( void* arg );
int
zGpioTest_SwitchDefaults( void* arg );

int
zGpioTest_HandlerAddRemove( void* arg );
int
zGpioTest_HandlerOnOff( void* arg );

int
zGpioTest_SwitchOnOff( void* arg );

using namespace zUtils;
using namespace Test;

static const std::string TESTDIR = "/tmp/zGpioHandlerTest";

class TestPortConf: public zConf::Data
{
public:
    TestPortConf( uint32_t id_ )
    {

        struct stat st = { 0 };
        std::fstream fs;
        std::string fileName;
        std::stringstream rootDir;

        this->Set( zGpio::Port::EXPORT_FILENAME_KEY, TESTDIR + "/export" );
        this->Set( zGpio::Port::UNEXPORT_FILENAME_KEY, TESTDIR + "/unexport" );
        this->Set( zGpio::Port::ID_VALUE_KEY, id_ );
        this->Set( zGpio::Port::DIR_FILENAME_KEY, TESTDIR + "/gpio%d/direction" );
        this->Set( zGpio::Port::DIR_VALUE_KEY, zGpio::Port::DIR_VALUE_DEF );
        this->Set( zGpio::Port::STATE_FILENAME_KEY, TESTDIR + "/gpio%d/value" );
        this->Set( zGpio::Port::EDGE_FILENAME_KEY, TESTDIR + "/gpio%d/edge" );
        this->Set( zGpio::Port::EDGE_VALUE_KEY, zGpio::Port::EDGE_VALUE_DEF );

        rootDir << TESTDIR;

        if (stat( rootDir.str().c_str(), &st ) == -1)
        {
            mkdir( rootDir.str().c_str(), 0777 );
        }

        fileName = rootDir.str() + "/export";
        fs.open( fileName.c_str(), std::fstream::out );
        fs << "" << std::endl;
        fs.flush();
        fs.close();

        fileName = rootDir.str() + "/unexport";
        fs.open( fileName.c_str(), std::fstream::out );
        fs << "" << std::endl;
        fs.flush();
        fs.close();

        rootDir << "/gpio" << id_;
        if (stat( rootDir.str().c_str(), &st ) == -1)
        {
            mkdir( rootDir.str().c_str(), 0777 );
        }

        fileName = rootDir.str() + "/direction";
        fs.open( fileName.c_str(), std::fstream::out );
        fs << zGpio::Port::DIR_VALUE_DEF << std::endl;
        fs.flush();
        fs.close();

        fileName = rootDir.str() + "/value";
        fs.open( fileName.c_str(), std::fstream::out );
        fs << zGpio::Port::STATE_VALUE_INACTIVE << std::endl;
        fs.flush();
        fs.close();

        fileName = rootDir.str() + "/edge";
        fs.open( fileName.c_str(), std::fstream::out );
        fs << zGpio::Port::EDGE_VALUE_DEF << std::endl;
        fs.flush();
        fs.close();

    };

    virtual
    ~TestPortConf()
    {
        struct stat st = { 0 };
        std::string id;
        std::fstream fs;
        std::string fileName;

        this->Get( zGpio::Port::ID_VALUE_KEY, id);

        fileName = TESTDIR + "/export";
        unlink(fileName.c_str());

        fileName = TESTDIR + "/unexport";
        unlink(fileName.c_str());

        fileName = TESTDIR + "/gpio" + id + "/direction";
        unlink(fileName.c_str());

        fileName = TESTDIR + "/gpio" + id + "/value";
        unlink(fileName.c_str());

        fileName = TESTDIR + "/gpio" + id + "/edge";
        unlink(fileName.c_str());

        fileName = TESTDIR + "/gpio" + id;
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

#endif /* GPIOHANDLERTEST_H_ */
