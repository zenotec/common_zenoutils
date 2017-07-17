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

  sum.Subtract(6);
  TEST_FALSE(sum.Empty());
  TEST_EQ(sum.Size(), 2);
  TEST_EQ(sum(), 16);

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

  sum.Subtract(6);
  TEST_FALSE(sum.Empty());
  TEST_EQ(sum.Size(), 2);
  TEST_EQ(sum(), 128);

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

  mean.Subtract(2);
  TEST_FALSE(mean.Empty());
  TEST_EQ(mean.Size(), 2);
  TEST_EQ(mean(), 8);

  // Return success
  return (0);
}

int
zMathTest_Variance(void* arg_)
{

  zMath::Variance<int> var;

  TEST_TRUE(var.empty());

  var.push_back(8);
  TEST_FALSE(var.empty());
  TEST_EQ(var.size(), 1);
  TEST_EQ(var(), 0);

  var.push_back(8);
  TEST_FALSE(var.empty());
  TEST_EQ(var.size(), 2);
  TEST_EQ(var(), 0);

  var.push_back(2);
  TEST_FALSE(var.empty());
  TEST_EQ(var.size(), 3);
  TEST_EQ(var(), 8);

  var.pop_back();
  TEST_FALSE(var.empty());
  TEST_EQ(var.size(), 2);
  TEST_EQ(var(), 0);

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

  stddev.Subtract(2);
  TEST_FALSE(stddev.Empty());
  TEST_EQ(stddev.Size(), 2);
  TEST_EQ(stddev(), 0);

  // Return success
  return (0);
}
