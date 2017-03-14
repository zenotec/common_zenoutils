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

#include <stdint.h>
#include <netinet/in.h>
#include <string.h>

#include <string>
#include <list>
#include <mutex>
#include <memory>

#include <zutils/zLog.h>
#include <zutils/zSem.h>
#include <zutils/zThread.h>
#include <zutils/zQueue.h>
#include <zutils/zEvent.h>

#include <zutils/zSocket.h>
#include <zutils/zLoopSocket.h>

#include "zSocketTest.h"

int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zUtils::zLog::FileConnector conn("zSocketTest.zlog");
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::CRIT, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::ERROR, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::WARN, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::INFO, &conn);
  zUtils::zLog::Log::Instance().RegisterConnector(zUtils::zLog::DBG, &conn);
  zUtils::zLog::Log::Instance().SetMaxLevel(zUtils::zLog::DBG);

  // Test all classes
  UTEST_INIT();

  UTEST_TEST(zSocketTest_BufferDefaults, 0);
  UTEST_TEST(zSocketTest_AddressDefaults, 0);
  UTEST_TEST(zSocketTest_ObserverDefaults, 0);
  UTEST_TEST(zSocketTest_SocketDefaults, 0);

  UTEST_TEST(zSocketTest_BufferCompare, 0);
  UTEST_TEST(zSocketTest_BufferString, 0);

  UTEST_TEST(zSocketTest_AddressGetSet, 0);
  UTEST_TEST(zSocketTest_AddressCompare, 0);

  UTEST_TEST(zSocketTest_LoopAddressGetSet, 0);
  UTEST_TEST(zSocketTest_LoopAddressCompare, 0);

  UTEST_TEST(zSocketTest_LoopSocketDefault, 0);
  UTEST_TEST(zSocketTest_LoopSocketSendReceive, 0);

  UTEST_TEST(zSocketTest_UnixAddressGetSet, 0);
  UTEST_TEST(zSocketTest_UnixAddressCompare, 0);

  UTEST_TEST(zSocketTest_UnixSocketDefault, 0);
  UTEST_TEST(zSocketTest_UnixSocketSendReceive, 0);

  UTEST_TEST(zSocketTest_EthAddressGetSet, 0);
  UTEST_TEST(zSocketTest_EthAddressCompare, 0);

  UTEST_TEST(zSocketTest_EthSocketDefault, 0);
  UTEST_TEST(zSocketTest_EthSocketSendReceiveLoop, 0);
  UTEST_TEST(zSocketTest_EthSocketSendReceiveSock2Sock, 0);

  UTEST_TEST(zSocketTest_InetAddressGetSet, 0);
  UTEST_TEST(zSocketTest_InetAddressCompare, 0);

  UTEST_TEST(zSocketTest_InetSocketDefault, 0);
  UTEST_TEST(zSocketTest_InetSocketSendReceiveLoop, 0);
  UTEST_TEST(zSocketTest_InetSocketSendReceiveSock2Sock, 0);

  UTEST_FINI();

}
