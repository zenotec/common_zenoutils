/*
 * Copyright (c) 2017 Cable Television Laboratories, Inc. ("CableLabs")
 *                    and others.  All rights reserved.
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

#ifndef __NL80211_BEACONHEADATTRIBUTE_H__
#define __NL80211_BEACONHEADATTRIBUTE_H__

// libc includes
#include <stdint.h>
#include <linux/nl80211.h>

// libc++ includes

// libzutils includes
#include <zutils/netlink/Attribute.h>

// local includes

namespace nl80211
{

//*****************************************************************************
// Class: BeaconHeadAttribute
//*****************************************************************************

class BeaconHeadAttribute : public Attribute
{

public:

  BeaconHeadAttribute() :
      Attribute(TYPE_BINARY, NL80211_ATTR_BEACON_HEAD), _buf { 0 }
  {
    this->SetValue(this->_buf, sizeof(this->_buf));
    this->ClrValid();
  }

  virtual
  ~BeaconHeadAttribute()
  {
  }

  void*
  GetBuffer() const
  {
    return ((void*) this->_buf);
  }

  bool
  PutBuffer(void* buf_, size_t len_)
  {
    bool status = false;
    //printf("BeaconHeadAttribute::PutBuffer(%p, %zd)\n", buf_, len_);
    if (buf_ && len_ && (len_ < sizeof(this->_buf)))
    {
      if (memcpy(this->_buf, buf_, len_) == this->_buf)
      {
        memset((this->_buf + len_), 0, (sizeof(this->_buf) - len_));
        this->SetValue(this->_buf, len_);
        status = true;
      }
    }
    return (status);
  }

protected:

private:

  uint8_t _buf[512];

};

}

#endif /* __NL80211_BEACONHEADATTRIBUTE_H__ */
