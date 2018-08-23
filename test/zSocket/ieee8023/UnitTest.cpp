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

#include <string>

#include <zutils/zLog.h>
using namespace zUtils;
ZLOG_MODULE_INIT(zLog::Log::MODULE_TEST);

#include "UnitTest.h"
#include "Ieee8023Test.h"

/* Frame (60 bytes) */
const uint8_t arp_pkt[60] =
{
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, /* ........ */
    0x02, 0x03, 0x04, 0x05, 0x08, 0x06, 0x00, 0x01, /* ..t..... */
    0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x00, 0x04, /* ........ */
    0xf2, 0x9f, 0x74, 0x86, 0x0a, 0x46, 0x0b, 0xa0, /* ..t..F.. */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x46, /* .......F */
    0x0b, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /* ........ */
    0x00, 0x00, 0x00, 0x00                          /* .... */
};
const size_t arp_pkt_len = sizeof(arp_pkt);



int
main(int argc, const char **argv)
{

  // Setup logging for testing
  zLog::FileConnector conn("UnitTest.zlog");
  zLog::Manager::Instance().RegisterConnector(zLog::Log::LEVEL_ALL, &conn);
  zLog::Manager::Instance().SetMaxLevel(zLog::Log::MODULE_TEST, zLog::Log::LEVEL_DEBUG);

  // Test all classes
  UTEST_INIT();

  // Frame class unit tests
  UTEST_TEST(Ieee8023Test_Defaults, 0);
  UTEST_TEST(Ieee8023Test_GetSet, 0);
  UTEST_TEST(Ieee8023Test_Assemble, 0);
  UTEST_TEST(Ieee8023Test_Disassemble, 0);

  zLog::Manager::Instance().UnregisterConnector(zLog::Log::LEVEL_ALL);

  UTEST_FINI();

}
