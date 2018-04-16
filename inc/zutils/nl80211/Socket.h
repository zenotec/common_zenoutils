/*
 * Copyright (c) 2018 Cable Television Laboratories, Inc. ("CableLabs")
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

#ifndef __NL80211_SOCKET_H__
#define __NL80211_SOCKET_H__

#include <zutils/netlink/GenericSocket.h>

namespace nl80211
{

//*****************************************************************************
// Class: GenericSocket
//*****************************************************************************

class Socket : public netlink::GenericSocket
{

public:

  Socket();

  virtual
  ~Socket();

  virtual SHARED_PTR(netlink::GenericMessage)
  CreateMsg();

protected:

private:

};

}

#endif /* __NL80211_SOCKET_H__ */