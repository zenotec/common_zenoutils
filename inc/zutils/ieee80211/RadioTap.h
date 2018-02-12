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

#ifndef __IEEE80211_RADIOTAP_H__
#define __IEEE80211_RADIOTAP_H__

#include <stdint.h>

#include <vector>
#include <map>

#include <zutils/ieee80211/RadioTapField.h>

namespace zUtils
{
namespace zWireless
{
namespace ieee80211
{

//*****************************************************************************
// Class: RadioTap
//*****************************************************************************

class RadioTap
{

public:

  RadioTap();

  virtual
  ~RadioTap();

  uint8_t*
  Assemble(uint8_t* frame_, size_t& rem_);

  uint8_t*
  Disassemble(uint8_t* frame_, size_t& rem_);

  uint8_t*
  Peek(uint8_t* frame_, size_t& rem_);

  uint8_t
  GetVersion() const;

  uint16_t
  GetLength() const;

  uint32_t
  GetPresent(const int index_ = 0) const;

  bool
  GetField(RadioTapField& field_, const int index_ = 0);

  bool
  PutField(const RadioTapField& field_, const int index_ = 0);

  void
  Display() const;

protected:

private:

  uint8_t _version;
  uint16_t _length;
  std::vector<uint32_t> _present;
  std::map<uint32_t, RadioTapField> _fields;

  static uint8_t*
  _chklen(uint8_t* f_, size_t len_, size_t& rem_);

};

}
}
}

#endif // __IEEE80211_RADIOTAP_H__
