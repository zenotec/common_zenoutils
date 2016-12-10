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

#include <string.h>

#include <fstream>

#include <zutils/zLed.h>

namespace zUtils
{
namespace zLed
{

Handler::Handler(Led::COLOR color_, Led* led_)
{
  memset(&this->_ledArray, 0, sizeof(this->_ledArray));
  this->AddLed(color_, led_);
}

Handler::~Handler()
{
}

Led*
Handler::GetLed(Led::COLOR color_)
{
  Led* led = NULL;
  switch (color_)
  {
  case Led::COLOR_GREEN:
    case Led::COLOR_RED:
    case Led::COLOR_AMBER:
    led = this->_ledArray[color_];
    break;
  default:
    led = NULL;
  }
  return (led);
}

bool
Handler::AddLed(Led::COLOR color_, Led* led_)
{
  bool status = false;
  switch (color_)
  {
  case Led::COLOR_GREEN:
    case Led::COLOR_RED:
    case Led::COLOR_AMBER:
    this->_ledArray[color_] = led_;
    break;
  default:
    return (false);
  }
  if (led_ && led_->_setMode(led_->GetMode()))
  {
    status = true;
  }
  return (status);
}

bool
Handler::RemoveLed(Led* led_)
{
  bool status = false;
  for (int i = Led::COLOR_GREEN; i <= Led::COLOR_AMBER; i++)
  {
    if (this->_ledArray[i] == led_)
    {
      this->_ledArray[i] = NULL;
      status = true;
    }
  }
  return (status);
}

bool
Handler::On(Led::COLOR color_)
{
  bool status = true;
  for (int i = Led::COLOR_GREEN; i <= Led::COLOR_AMBER; i++)
  {
    Led *led = this->_ledArray[i];
    // Test if the caller specified a color that was not registered
    if (!led && (color_ == i))
    {
      status = false;
      break;
    }
    if ((color_ == Led::COLOR_ALL) || (color_ == i))
    {
      if (led && !led->_setState(Led::STATE_ON))
      {
        status = false;
        break;
      }
    }
  }
  return (status);
}

bool
Handler::Off(Led::COLOR color_)
{
  bool status = true;
  for (int i = Led::COLOR_GREEN; i <= Led::COLOR_AMBER; i++)
  {
    Led *led = this->_ledArray[i];
    // Test if the caller specified a color that was not registered
    if (!led && (color_ == i))
    {
      status = false;
      break;
    }
    if ((color_ == Led::COLOR_ALL) || (color_ == i))
    {
      if (led && !led->_setState(Led::STATE_OFF))
      {
        status = false;
        break;
      }
    }
  }
  return (status);
}

bool
Handler::Toggle(Led::COLOR color_)
{
  bool status = true;
  for (int i = Led::COLOR_GREEN; i <= Led::COLOR_AMBER; i++)
  {
    Led *led = this->_ledArray[i];
    if (led && ((color_ == Led::COLOR_ALL) || (color_ == i)))
    {
      Led::STATE state = led->GetState();
      if ((state == Led::STATE_OFF) && !led->_setState(Led::STATE_ON))
      {
        status = false;
        break;
      }
      if ((state == Led::STATE_ON) && !led->_setState(Led::STATE_OFF))
      {
        status = false;
        break;
      }
    }
  }
  return (status);
}

}
}
