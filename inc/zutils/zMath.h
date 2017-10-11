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
#include <algorithm> // std::min/max_element
#include <numeric> // std::accumulate
#include <stdexcept>

#include <zutils/zUtils.h>
#include <zutils/zCompatibility.h>

using namespace std;

namespace zUtils
{
namespace zMath
{

//**********************************************************************
// Class: Min
//**********************************************************************

template<class T>
  class Min
  {

  public:

    Min(size_t len = (size_t)-1) :
        _len(len)
    {
    }

    Min(std::vector<T> other) :
        _len(other.size())
    {
      FOREACH (auto& item, other)
      {
        this->Add(item);
      }
    }

    Min(std::list<T> other) :
        _len(other.size())
    {
      FOREACH (auto& item, other)
      {
        this->Add(item);
      }
    }

    virtual
    ~Min()
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
      if ((this->_data.size() + 1) > this->_len)
      {
        this->_data.pop_front();
      }
      this->_data.push_back(item);
    }

    virtual T
    Value() const
    {
      return (*std::min_element(this->_data.begin(), this->_data.end()));
    }

    size_t
    Size() const
    {
      return (this->_data.size());
    }

    bool
    Empty() const
    {
      return (this->_data.empty());
    }

  protected:

  private:

    std::list<T> _data;
    size_t _len;

  };

//**********************************************************************
// Class: Max
//**********************************************************************

template<class T>
  class Max
  {

  public:

    Max(size_t len = (size_t) -1) :
        _len(len)
    {
    }

    Max(std::vector<T> other) :
        _len(other.size())
    {
      FOREACH (auto& item, other)
      {
        this->Add(item);
      }
    }

    Max(std::list<T> other) :
        _len(other.size())
    {
      FOREACH (auto& item, other)
      {
        this->Add(item);
      }
    }

    virtual
    ~Max()
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
      if ((this->_data.size() + 1) > this->_len)
      {
        this->_data.pop_front();
      }
      this->_data.push_back(item);
    }

    virtual T
    Value() const
    {
      return (*std::max_element(this->_data.begin(), this->_data.end()));
    }

    size_t
    Size() const
    {
      return (this->_data.size());
    }

    bool
    Empty() const
    {
      return (this->_data.empty());
    }

  protected:

  private:

    std::list<T> _data;
    size_t _len;

  };

//**********************************************************************
// Class: Sum
//**********************************************************************

template<class T>
  class Sum
  {

  public:

    Sum(size_t len = (size_t) -1) :
        _len(len)
    {
    }

    Sum(std::vector<T> other) :
        _len(other.size())
    {
      FOREACH (auto& item, other)
      {
        this->Add(item);
      }
    }

    Sum(std::list<T> other) :
        _len(other.size())
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
      return (this->Value());
    }

    void
    Add(T item)
    {
      if ((this->_data.size() + 1) > this->_len)
      {
        this->_data.pop_front();
      }
      this->_data.push_back(item);
    }

    virtual T
    Value() const
    {
      T sum = 0;
      FOREACH (auto& item, this->_data)
      {
        sum += item;
      }
      return (sum);
    }

    size_t
    Size() const
    {
      return (this->_data.size());
    }

    bool
    Empty() const
    {
      return (this->_data.empty());
    }

  protected:

  private:

    std::list<T> _data;
    size_t _len;

  };

//**********************************************************************
// Class: SumSquares
//**********************************************************************

template<class T>
  class SumSquares : public Sum<T>
  {

  public:

    SumSquares(size_t len = (size_t)-1)
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

    Mean(size_t len = (size_t)-1) :
      Sum<T>(len)
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
  class Variance
  {

  public:

    Variance(size_t len = (size_t)-1) :
      _len(len)
    {
    }

    Variance(std::vector<T> other) :
        _len(other.size())
    {
      FOREACH (auto& item, other)
      {
        this->Add(item);
      }
    }

    Variance(std::list<T> other) :
        _len(other.size())
    {
      FOREACH (auto& item, other)
      {
        this->Add(item);
      }
    }

    T
    operator()()
    {
      return (this->Value());
    }

    void
    Add(T item)
    {
      if ((this->_data.size() + 1) > this->_len)
      {
        this->_data.pop_front();
      }
      this->_data.push_back(item);
    }

    T
    Value()
    {
      T mean = Mean<T>(this->_data)();
      std::vector<T> sq_dist;
      FOREACH (auto& item, this->_data)
      {
        sq_dist.push_back((item - mean) * (item - mean));
      }
      T sum = Sum<T>(sq_dist)();
      return (sum / sq_dist.size());
    }

    size_t
    Size() const
    {
      return (this->_data.size());
    }

    bool
    Empty() const
    {
      return (this->_data.empty());
    }

  protected:

  private:

    std::list<T> _data;
    size_t _len;

  };

//**********************************************************************
// Class: StandardDeviation
//**********************************************************************

template<class T>
  class StandardDeviation
  {

  public:

    StandardDeviation(size_t len = (size_t)-1) :
      _mean(len), _sum(len)
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

    T
    Value()
    {
      T stddev = (this->_sum.Value() / (T)this->_mean.Size()) -
          (this->_mean.Value() * this->_mean.Value());
      return (sqrt(stddev));
    }

    size_t
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

    FrequencyDistribution(size_t len = (size_t)-1)
    {
    }

    FrequencyDistribution(std::vector<T> other)
    {
    }

    FrequencyDistribution(std::list<T> other)
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
