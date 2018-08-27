/*
 * Copyright (c) 2017 - 2018 Cable Television Laboratories, Inc. ("CableLabs")
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

#ifndef __IEEE8023_LLCFRAME_H__
#define __IEEE8023_LLCFRAME_H__

// libc includes
#include <stdio.h>

// libc++ includes
#include <map>

// libzutils includes

// local includes

namespace zUtils
{
namespace zSocket
{
namespace ieee8023
{

//*****************************************************************************
// Class: LlcFrame
//*****************************************************************************

class LlcFrame :
    public Frame
{

public:

  LlcFrame();

  virtual
  ~LlcFrame();

  virtual uint8_t*
  Assemble(uint8_t* p_, size_t& rem_, bool fcs_ = false);

  virtual uint8_t*
  Disassemble(uint8_t* p_, size_t& rem_, bool fcs_ = false);

  virtual void
  Display() const;

protected:

private:

};

}
}
}

#endif /* __IEEE8023_LLCFRAME_H__ */
