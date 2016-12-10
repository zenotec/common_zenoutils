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

#ifndef __ZLED_H__
#define __ZLED_H__

#include <string>
#include <vector>
#include <fstream>

using namespace std;

namespace zUtils
{
namespace zLed
{


class Handler;

class Led
{
  friend class Handler;

  static const std::string TRIGGER_FILENAME;
  static const std::string TRIGGER_NONE;
  static const std::string TRIGGER_ONESHOT;
  static const std::string TRIGGER_HEARTBEAT;

public:
  static const std::string LED_COM1_TX;
  static const std::string LED_COM1_RX;
  static const std::string LED_COM2_TX;
  static const std::string LED_COM2_RX;
  static const std::string LED_TX_GREEN;
  static const std::string LED_TX_RED;
  static const std::string LED_TX_AMBER;
  static const std::string LED_CD_GREEN;
  static const std::string LED_CD_RED;
  static const std::string LED_CD_AMBER;
  static const std::string LED_CTS_GREEN;
  static const std::string LED_CTS_RED;
  static const std::string LED_CTS_AMBER;

  static const std::string DEVROOT_DEFAULT;

  // Note, class implementation is order/value dependent
  enum COLOR
  {
    COLOR_NONE = 0,
    COLOR_GREEN = 1,
    COLOR_RED = 2,
    COLOR_AMBER = 3,
    COLOR_ALL = 4
  };

  enum STATE
  {
    STATE_NONE = -1,
    STATE_OFF = 0,
    STATE_ON = 1
  };

  enum MODE
  {
    MODE_NONE = 0,
    MODE_ONESHOT = 1,
    MODE_HEARTBEAT = 2
  };

  Led(const std::string& name_,
      Led::COLOR color_ = Led::COLOR_NONE,
      Led::MODE mode_ = Led::MODE_NONE,
      Led::STATE defState_ = Led::STATE_OFF,
      const std::string& root_ = Led::DEVROOT_DEFAULT);

  virtual
  ~Led();

  std::string
  GetName();

  Led::COLOR
  GetColor();

  Led::MODE
  GetMode();

  Led::STATE
  GetState();

protected:
  static const std::string ACTIONFILE_NONE;
  static const std::string ACTIONFILE_ONESHOT;

  std::string
  _getDevRoot();

  std::string
  _getActionFile();

  bool
  _setColor(Led::COLOR color_);

  bool
  _setMode(Led::MODE mode_);

  bool
  _setState(Led::STATE state_);

private:

  bool
  _setModeNone();

  bool
  _setModeOneShot();

  bool
  _setModeHeartbeat();

  std::string _root;
  std::string _name;
  std::string _actionFile;
  Led::COLOR _color;
  Led::MODE _mode;
  Led::STATE _defState;
  Led::STATE _state;

};

class Handler
{
public:

  Handler(Led::COLOR color_ = Led::COLOR_NONE, Led *led_ = NULL);

  virtual
  ~Handler();

  Led*
  GetLed(Led::COLOR color_);

  bool
  AddLed(Led::COLOR color_, Led *led_);

  bool
  RemoveLed(Led *led_);

  bool
  On(Led::COLOR color_);

  bool
  Off(Led::COLOR color_);

  bool
  Toggle(Led::COLOR color_);

private:
  Led *_ledArray[Led::COLOR_ALL];

};

}
}

#endif // __ZLED_H__
