/*
 * Copyright (c) 2014-2017 ZenoTec LLC (http://www.zenotec.net)
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

#ifndef __ZMATH_H__
#define __ZMATH_H__

#include <math.h>

#include <iostream>
#include <numeric>
#include <list>
#include <vector>
#include <stdexcept>

#include <zutils/zUtils.h>
#include <zutils/zCompatibility.h>

using namespace std;

namespace zUtils
{
namespace zMath
{

//**********************************************************************
// Class: Sum
//**********************************************************************

template<class T>
  class Sum
  {

  public:

    Sum() :
      _sum(0), _num(0)
    {
    }

    Sum(std::vector<T> other) :
      _sum(0), _num(0)
    {
      FOREACH (auto& item, other)
      {
        this->Add(item);
      }
    }

    Sum(std::list<T> other) :
      _sum(0), _num(0)
    {
      FOREACH (auto& item, other)
      {
        this->Add(item);
      }
    }

    virtual
    ~Sum()
    {
    }

    T
    operator()()
    {
      return (this->_sum);
    }

    void
    Add(T item)
    {
      if ((this->_num + 1) < this->_num)
      {
        throw std::out_of_range("zMath::Sum(): Out of Range");
      }
      this->_sum += item;
      this->_num += 1;
    }

    void
    Subtract(T item)
    {
      if (this->_num == 0)
      {
        throw std::out_of_range("zMath::Sum(): Out of Range");
      }
      this->_sum -= item;
      this->_num -= 1;
    }

    virtual T
    Value() const
    {
      return(this->_sum);
    }

    T
    Size() const
    {
      return(this->_num);
    }

    bool
    Empty() const
    {
      return(this->_num == 0);
    }

  protected:

  private:

    T _sum;
    T _num;

  };

//**********************************************************************
// Class: SumSquares
//**********************************************************************

template<class T>
  class SumSquares : public Sum<T>
  {

  public:

    SumSquares()
    {
    }

    SumSquares(std::vector<T> other)
    {
      FOREACH (auto& item, other)
      {
        this->Add(item);
      }
    }

    SumSquares(std::list<T> other)
    {
      FOREACH (auto& item, other)
      {
        this->Add(item);
      }
    }

    virtual
    ~SumSquares()
    {
    }

    void
    Add(T item)
    {
      Sum<T>::Add(item * item);
    }

    void
    Subtract(T item)
    {
      Sum<T>::Subtract(item * item);
    }

  protected:

  private:

  };

//**********************************************************************
// Class: Mean
//**********************************************************************

template<class T>
  class Mean : public Sum<T>
  {

  public:

    Mean()
    {
    }

    Mean(std::vector<T> other) :
      Sum<T>(other)
    {
    }

    Mean(std::list<T> other) :
      Sum<T>(other)
    {
    }

    virtual
    ~Mean()
    {
    }

    T
    operator()()
    {
      return (this->Value());
    }

    virtual T
    Value() const
    {
      return (Sum<T>::Value() / Sum<T>::Size());
    }

  protected:

  private:

  };

//**********************************************************************
// Class: Variance
//**********************************************************************

template<class T>
  class Variance : public std::vector<T>
  {

  public:

    Variance()
    {
    }

    Variance(std::vector<T> other) :
      std::vector<T>(other)
    {
    }

    T
    operator()()
    {
      return (this->Value());
    }

    T
    Value()
    {
      T mean = Mean<T>(*this)();
      std::vector<T> sq_dist;
      FOREACH (auto& item, *this)
      {
        sq_dist.push_back((item - mean) * (item - mean));
      }
      T sum = Sum<T>(sq_dist)();
      return (sum / sq_dist.size());
    }

  protected:

  private:

  };

//**********************************************************************
// Class: StandardDeviation
//**********************************************************************

template<class T>
  class StandardDeviation
  {

  public:

    StandardDeviation()
    {
    }

    StandardDeviation(std::vector<T> other) :
      _mean(other), _sum(other)
    {
    }

    StandardDeviation(std::list<T> other) :
      _mean(other), _sum(other)
    {
    }

    T
    operator()()
    {
      return (this->Value());
    }

    void
    Add(T item)
    {
      this->_mean.Add(item);
      this->_sum.Add(item);
    }

    void
    Subtract(T item)
    {
      this->_mean.Subtract(item);
      this->_sum.Subtract(item);
    }

    T
    Value()
    {
      T stddev = (this->_sum.Value() / (T)this->_mean.Size()) -
          (this->_mean.Value() * this->_mean.Value());
      return (sqrt(stddev));
    }

    T
    Size() const
    {
      return(this->_mean.Size());
    }

    bool
    Empty() const
    {
      return(this->_mean.Empty());
    }

  protected:

  private:

    Mean<T> _mean;
    SumSquares<T> _sum;

  };

//**********************************************************************
// Class: FrequencyDistribution
//**********************************************************************


template<class T>
  class FrequencyDistribution
  {

  public:

    FrequencyDistribution()
    {
    }

    FrequencyDistribution(std::vector<T> other)
    {
    }

    T
    operator()()
    {
    }

    T
    Value()
    {
    }

  protected:

  private:

  };


}
}

#endif // __ZMATH_H__
