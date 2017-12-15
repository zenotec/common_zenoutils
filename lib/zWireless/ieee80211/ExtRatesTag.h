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

#ifndef __IEEE80211_EXTRATESTAG_H__
#define __IEEE80211_EXTRATESTAG_H__

#include "Tag.h"

namespace zUtils
{
namespace ieee80211
{

//*****************************************************************************
// Class: ExtRatesTag
//*****************************************************************************

class ExtRatesTag : public RatesTag
{

public:

  ExtRatesTag() :
    RatesTag(Tag::ID_EXT_RATES)
  {
  }

  virtual
  ~ExtRatesTag()
  {
  }

protected:

private:

};

}
}

#endif /* __IEEE80211_EXTRATESTAG_H__ */
