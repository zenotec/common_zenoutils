#ifndef UTEST_H_
#define UTEST_H_

//*****************************************************************************
//  Includes
//*****************************************************************************
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <string>
#include <iostream>
#include <sstream>
#include <queue>

//*****************************************************************************
//  Defines
//*****************************************************************************

#define DEBUG        1

#define TIMELOG_LEN     10000

#define TIMELOG_START   1
#define TIMELOG_STOP    2

//*****************************************************************************
//  Macros
//*****************************************************************************

#if DEBUG

#define DPRINTF(fmt, ...) \
{ \
   fprintf( stdout, "%s:%d: ", __FILE__, __LINE__ ); \
   fprintf( stdout, fmt, ##__VA_ARGS__ ); \
   fflush( stdout ); \
}

#else
#define DPRINTF(fmt, ...)
#endif

#define IPRINTF(fmt, ...) \
{ \
   fprintf( stdout, fmt, ##__VA_ARGS__ ); \
   fflush( stdout ); \
}

#define EPRINTF(fmt, ...) \
{ \
   fprintf( stdout, "%s:%d\n", __FILE__, __LINE__ ); \
   fprintf( stdout, fmt, ##__VA_ARGS__ ); \
   fflush( stdout ); \
}

#define DOTS(n) \
{ \
   unsigned int i; \
   for (i = 0; i < n; i++) IPRINTF("."); \
}

#define UTEST(fnc, arg) \
{ \
   int status = 0; \
   IPRINTF( "  %s()", #fnc ); \
   DOTS((50-sizeof(#fnc))); \
   if ( ( status = (fnc)(arg) ) ) \
   { \
      IPRINTF( "fail\n" ); \
      utest_exit = 1; \
   } \
   else \
   { \
      IPRINTF( "success\n" ); \
   } \
}

#define TEST_IS_NULL(x) \
{ \
  if( utest->Eq( __LINE__, (x), NULL, std::string( "" ) ) != true ) \
      return (1); \
}

#define TEST_ISNOT_NULL(x) \
{ \
  if( utest->Neq( __LINE__, (x), NULL, std::string( "" ) ) != true ) \
      return (1); \
}

#define TEST_IS_ZERO(x) \
{ \
  if( utest->Eq( __LINE__, (x), 0, std::string( "" ) ) != true ) \
      return (1); \
}

#define TEST_ISNOT_ZERO(x) \
{ \
  if( utest->Neq( __LINE__, (x), 0, std::string( "" ) ) != true ) \
      return (2); \
}

#define TEST_TRUE(x) \
{ \
  if( utest->True( __LINE__, (x), std::string( "" ) ) != true ) \
      return (1); \
}

#define TEST_FALSE(x) \
{ \
  if( utest->False( __LINE__, (x), std::string( "" ) ) != true ) \
      return (2); \
}

#define TEST_EQ(x, y) TEST_EQ_MSG(x, y, std::string( "" ))
#define TEST_EQ_MSG(x, y, msg) \
{ \
  if( utest->Eq( __LINE__, (x), (y), (msg) ) != true ) \
    return (3); \
}

#define TEST_NEQ(x, y) TEST_NEQ_MSG(x, y, std::string( "" ))
#define TEST_NEQ_MSG(x, y, msg) \
{ \
  if( utest->Neq( __LINE__, (x), (y), (msg) ) != true ) \
    return (4); \
}

#define TEST_LT(x, y) TEST_LT_MSG(x, y, std::string( "" ))
#define TEST_LT_MSG(x, y, msg) \
{ \
  if( utest->Lt( __LINE__, (x), (y), (msg) ) != true ) \
    return (5); \
}

#define TEST_GT(x, y) TEST_GT_MSG((x), (y), std::string( "" ) )
#define TEST_GT_MSG(x, y, msg) \
{ \
  if( utest->Gt( __LINE__, (x), (y), (msg) ) != true ) \
    return (6); \
}

#define INIT() \
  utest = new UnitTest( std::string(__FUNCTION__) );

#define FINI() \
  delete( utest ); \
  return( utest_exit );

//*****************************************************************************
//  Type Definitions
//*****************************************************************************

//*****************************************************************************
//  Globals
//*****************************************************************************

//*****************************************************************************
//  Classes
//*****************************************************************************

namespace Test
{

class Log
{
public:
  Log(const unsigned int line_, const std::string &msg_) :
      _line(line_), _msg(msg_)
  {
    clock_gettime(CLOCK_REALTIME, &this->_timestamp);
  }
  ~Log()
  {

  }

  std::string
  GetTimeStamp()
  {
    std::stringstream timestamp;
    timestamp << this->_timestamp.tv_sec << "." << this->_timestamp.tv_nsec;
    return (timestamp.str());
  }

  int
  GetLine()
  {
    return (this->_line);
  }

  std::string
  GetMsg()
  {
    return (this->_msg);
  }

protected:

private:
  struct timespec _timestamp;
  int _line;
  std::string _msg;
};

class UnitTest
{

public:
  UnitTest(const std::string &name_)
  {
    this->name = name_;
    IPRINTF("\nExecuting unit tests: %s\n", name_.c_str());
    clock_gettime(CLOCK_REALTIME, &this->start);
  }

  ~UnitTest()
  {
    double diff = 0;
    clock_gettime(CLOCK_REALTIME, &this->stop);
    diff = (stop.tv_sec - start.tv_sec)
        + (double) (stop.tv_nsec - start.tv_nsec) / (double) 1000000000ULL;
    IPRINTF("Complete: %lf seconds\n", diff);
    this->dumpLog();
  }

  // Boolean test
  bool
  True(const int line_, const bool &val_, const std::string &msg_)
  {
    if (val_ != true)
    {
      std::stringstream errmsg;
      errmsg << "Not true" << std::endl;
      this->LogMsg(line_, errmsg.str());
      return (false);
    } // end if
    else
    {
      return (true);
    } // end else
  }
  bool
  False(const int line_, const bool &val_, const std::string &msg)
  {
    if (val_ != false)
    {
      std::stringstream errmsg;
      errmsg << "Not false" << std::endl;
      this->LogMsg(line_, errmsg.str());
      return (false);
    } // end if
    else
    {
      return (true);
    } // end else
  }

  // Pointer comparison
  bool
  Eq(const int line_, void *s1_, void *s2_, const std::string &msg_)
  {
    if (s1_ != s2_)
    {
      std::stringstream errmsg;
      errmsg << s1_ << " != " << s2_ << std::endl;
      this->LogMsg(line_, errmsg.str());
      return (false);
    } // end if
    else
    {
      return (true);
    } // end else
  }
  bool
  Neq(const int line_, void *s1_, void *s2_, const std::string &msg_)
  {
    if (s1_ == s2_)
    {
      std::stringstream errmsg;
      errmsg << s1_ << " == " << s2_ << std::endl;
      this->LogMsg(line_, errmsg.str());
      return (false);
    } // end if
    else
    {
      return (true);
    } // end else
  }

  // String comparison
  bool
  Eq(const int line_, const std::string &s1_, const std::string &s2_, const std::string &msg_)
  {
    if (s1_ != s2_)
    {
      std::stringstream errmsg;
      errmsg << s1_ << " != " << s2_ << std::endl;
      this->LogMsg(line_, errmsg.str());
      return (false);
    } // end if
    else
    {
      return (true);
    } // end else
  }
  bool
  Neq(const int line_, const std::string &s1_, const std::string &s2_, const std::string &msg_)
  {
    if (s1_ == s2_)
    {
      std::stringstream errmsg;
      errmsg << s1_ << " == " << s2_ << std::endl;
      this->LogMsg(line_, errmsg.str());
      return (false);
    } // end if
    else
    {
      return (true);
    } // end else
  }

  // Integer comparison
  bool
  Eq(const int line_, const int &i1_, const int &i2_, const std::string &msg_)
  {
    if (i1_ != i2_)
    {
      std::stringstream errmsg;
      errmsg << i1_ << " != " << i2_ << std::endl;
      this->LogMsg(line_, errmsg.str());
      return (false);
    } // end if
    else
    {
      return (true);
    } // end else
  }
  bool
  Neq(const int line_, const int &i1_, const int &i2_, const std::string &msg_)
  {
    if (i1_ == i2_)
    {
      std::stringstream errmsg;
      errmsg << i1_ << " == " << i2_ << std::endl;
      this->LogMsg(line_, errmsg.str());
      return (false);
    } // end if
    else
    {
      return (true);
    } // end else
  }
  bool
  Lt(const int line_, const int &i1_, const int &i2_, const std::string &msg_)
  {
    if (i1_ >= i2_)
    {
      std::stringstream errmsg;
      errmsg << i1_ << " >= " << i2_ << std::endl;
      this->LogMsg(line_, errmsg.str());
      return (false);
    } // end if
    else
    {
      return (true);
    } // end else
  }
  bool
  Gt(const int line_, const int &i1_, const int &i2_, const std::string &msg_)
  {
    if (i1_ <= i2_)
    {
      std::stringstream errmsg;
      errmsg << i1_ << " <= " << i2_ << std::endl;
      this->LogMsg(line_, errmsg.str());
      return (false);
    } // end if
    else
    {
      return (true);
    } // end else
  }

  void
  LogMsg(const int line_, const std::string &msg_)
  {
    Log log(line_, msg_);
    this->_log.push(log);
  }

protected:
  void
  dumpLog()
  {
    std::cout.flush();
    std::cerr.flush();
    while (!this->_log.empty())
    {
      std::cerr << this->_log.front().GetTimeStamp() << ": ";
      std::cerr << "LINE #";
      std::cerr << this->_log.front().GetLine() << ": ";
      std::cerr << this->_log.front().GetMsg() << std::endl;
      std::cerr.flush();
      this->_log.pop();
    } // end while
    std::cout.flush();
    std::cerr.flush();
  }

private:
  std::string name;
  struct timespec start;
  struct timespec stop;
  std::queue<Log> _log;
};

static UnitTest *utest = 0;
static int utest_exit = 0;

}

#endif /* UTEST_H_ */
