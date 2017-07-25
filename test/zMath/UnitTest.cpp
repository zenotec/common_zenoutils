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

#include "zMathTest.h"

int
main(int argc, const char **argv)
{

  // Test all classes
  UTEST_INIT();

  UTEST_TEST( zMathTest_Min, 0);
  UTEST_TEST( zMathTest_Max, 0);
  UTEST_TEST( zMathTest_Sum, 0);
  UTEST_TEST( zMathTest_SumSquares, 0);
  UTEST_TEST( zMathTest_Mean, 0);
  UTEST_TEST( zMathTest_Variance, 0);
  UTEST_TEST( zMathTest_StandardDeviation, 0);

  UTEST_FINI();

}
