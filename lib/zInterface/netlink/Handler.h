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

#ifndef __NETLINK_HANDLER_H__
#define __NETLINK_HANDLER_H__

namespace netlink
{

//*****************************************************************************
// Class: Handler
//*****************************************************************************

class Handler
{

public:

  Handler(void* arg_ = NULL);

  virtual
  ~Handler();

  int
  Count() const;

  static int
  ValidCallback(struct nl_msg* msg_, void* arg_);

  static int
  FinishCallback(struct nl_msg* msg_, void* arg_);

  static int
  AckCallback(struct nl_msg* msg_, void* arg_);

  static int
  ErrorCallback(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_);

protected:

  virtual int
  valid_cb(struct nl_msg* msg_, void* arg_) = 0;

  virtual int
  finish_cb(struct nl_msg* msg_, void* arg_);

  virtual int
  ack_cb(struct nl_msg* msg_, void* arg_);

  virtual int
  err_cb(struct sockaddr_nl* nla_, struct nlmsgerr* nlerr_, void* arg_) = 0;

private:

  void* _arg;
  int _msgcnt;

};

}

#endif /* __NETLINK_HANDLER_H__ */
