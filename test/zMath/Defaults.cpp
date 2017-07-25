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

using namespace zUtils;

int
zMathTest_Min(void* arg_)
{

  zMath::Min<int> min;

  TEST_TRUE(min.Empty());

  min.Add(8);
  TEST_FALSE(min.Empty());
  TEST_EQ(min.Size(), 1);
  TEST_EQ(min(), 8);

  min.Add(10);
  TEST_FALSE(min.Empty());
  TEST_EQ(min.Size(), 2);
  TEST_EQ(min(), 8);

  min.Add(6);
  TEST_FALSE(min.Empty());
  TEST_EQ(min.Size(), 3);
  TEST_EQ(min(), 6);

  // Return success
  return (0);
}

int
zMathTest_Max(void* arg_)
{

  zMath::Max<int> max;

  TEST_TRUE(max.Empty());

  max.Add(8);
  TEST_FALSE(max.Empty());
  TEST_EQ(max.Size(), 1);
  TEST_EQ(max(), 8);

  max.Add(10);
  TEST_FALSE(max.Empty());
  TEST_EQ(max.Size(), 2);
  TEST_EQ(max(), 10);

  max.Add(6);
  TEST_FALSE(max.Empty());
  TEST_EQ(max.Size(), 3);
  TEST_EQ(max(), 10);

  // Return success
  return (0);
}

int
zMathTest_Sum(void* arg_)
{

  zMath::Sum<int> sum;

  TEST_TRUE(sum.Empty());

  sum.Add(8);
  TEST_FALSE(sum.Empty());
  TEST_EQ(sum.Size(), 1);
  TEST_EQ(sum(), 8);

  sum.Add(8);
  TEST_FALSE(sum.Empty());
  TEST_EQ(sum.Size(), 2);
  TEST_EQ(sum(), 16);

  sum.Add(6);
  TEST_FALSE(sum.Empty());
  TEST_EQ(sum.Size(), 3);
  TEST_EQ(sum(), 22);

  // Return success
  return (0);
}

int
zMathTest_SumSquares(void* arg_)
{

  zMath::SumSquares<int> sum;

  TEST_TRUE(sum.Empty());

  sum.Add(8);
  TEST_FALSE(sum.Empty());
  TEST_EQ(sum.Size(), 1);
  TEST_EQ(sum(), 64);

  sum.Add(8);
  TEST_FALSE(sum.Empty());
  TEST_EQ(sum.Size(), 2);
  TEST_EQ(sum(), 128);

  sum.Add(6);
  TEST_FALSE(sum.Empty());
  TEST_EQ(sum.Size(), 3);
  TEST_EQ(sum(), 164);

  // Return success
  return (0);
}

int
zMathTest_Mean(void* arg_)
{

  zMath::Mean<int> mean;

  TEST_TRUE(mean.Empty());

  mean.Add(8);
  TEST_FALSE(mean.Empty());
  TEST_EQ(mean.Size(), 1);
  TEST_EQ(mean(), 8);

  mean.Add(8);
  TEST_FALSE(mean.Empty());
  TEST_EQ(mean.Size(), 2);
  TEST_EQ(mean(), 8);

  mean.Add(2);
  TEST_FALSE(mean.Empty());
  TEST_EQ(mean.Size(), 3);
  TEST_EQ(mean(), 6);

  // Return success
  return (0);
}

int
zMathTest_Variance(void* arg_)
{

  zMath::Variance<int> var;

  TEST_TRUE(var.Empty());

  var.Add(8);
  TEST_FALSE(var.Empty());
  TEST_EQ(var.Size(), 1);
  TEST_EQ(var(), 0);

  var.Add(8);
  TEST_FALSE(var.Empty());
  TEST_EQ(var.Size(), 2);
  TEST_EQ(var(), 0);

  var.Add(2);
  TEST_FALSE(var.Empty());
  TEST_EQ(var.Size(), 3);
  TEST_EQ(var(), 8);

  // Return success
  return (0);
}

int
zMathTest_StandardDeviation(void* arg_)
{

  zMath::StandardDeviation<int> stddev;

  TEST_TRUE(stddev.Empty());

  stddev.Add(8);
  TEST_FALSE(stddev.Empty());
  TEST_EQ(stddev.Size(), 1);
  TEST_EQ(stddev(), 0);

  stddev.Add(8);
  TEST_FALSE(stddev.Empty());
  TEST_EQ(stddev.Size(), 2);
  TEST_EQ(stddev(), 0);

  stddev.Add(2);
  TEST_FALSE(stddev.Empty());
  TEST_EQ(stddev.Size(), 3);
  TEST_EQ(stddev(), 2);

  // Return success
  return (0);
}
