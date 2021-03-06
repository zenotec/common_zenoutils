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

#ifndef __IEEE8023_VLANFRAME_H__
#define __IEEE8023_VLANFRAME_H__

// libc includes
#include <stdio.h>

// libc++ includes
#include <map>

// libzutils includes

// local includes
#include <zutils/ieee8023/Frame.h>

namespace zUtils
{
namespace zSocket
{
namespace ieee8023
{

//*****************************************************************************
// Class: VlanFrame
//*****************************************************************************

class VlanFrame :
    public Frame
{

public:

  VlanFrame();

  virtual
  ~VlanFrame();

  virtual bool
  Assemble(zSocket::Buffer& sb_, bool fcs_);

  virtual bool
  Disassemble(zSocket::Buffer& sb_, bool fcs_);

  virtual void
  Display(const std::string& prefix_ = "") const;

protected:

private:

};

}
}
}

#endif /* __IEEE8023_VLANFRAME_H__ */
