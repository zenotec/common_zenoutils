/*
 * Copyright (c) 2014-2018 ZenoTec LLC (http://www.zenotec.net)
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

// libc includes

// libc++ includes

// libzutils includes
#include <zutils/zLog.h>
using namespace zUtils;
#include <zutils/zState.h>

// local includes

ZLOG_MODULE_INIT(zLog::Log::MODULE_STATE);

namespace zUtils
{
namespace zState
{

//**********************************************************************
// Class: State
//**********************************************************************

State::State(SHARED_PTR(Context) context_, const uint32_t id_) :
    _ctx(context_), _id(id_)
{
}

State::~State()
{
}

SHARED_PTR(Context)
State::GetContext()
{
  return (this->_ctx);
}

uint32_t
State::GetId() const
{
  return (this->_id);
}

}
}
