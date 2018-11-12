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
#include <signal.h>

#include <mutex>
#include <list>
#include <map>

#include <zutils/zEvent.h>

#include <zutils/zState.h>

namespace zUtils
{
namespace zState
{

//**********************************************************************
// Class: Handler
//**********************************************************************

Handler::Handler()
{
}

Handler::~Handler()
{
}

SHARED_PTR(zState::State)
Handler::GetState() const
{
  return (this->_state);
}

bool
Handler::SetState(SHARED_PTR(zState::State) state_)
{
  this->_state = state_;
  return (true);
}

bool
Handler::Notify(SHARED_PTR(Notification) n_)
{
  bool status = false;
  if (this->_state.get())
  {
    n_->SetState(this->GetState());
    status = this->_state->ObserveEvent(n_);
    this->SetState(n_->GetState());
  }
  return (status);
}

}
}
