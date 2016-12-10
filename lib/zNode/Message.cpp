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

#include <zutils/zNode.h>
#include <zutils/zUuid.h>

namespace zUtils
{
namespace zNode
{

const std::string Message::STR_NODE = "zNode";

Message::Message(zMessage::Message::TYPE type_, zNode::Node &node_)
{
  zUuid::Uuid uuid;
  this->SetId(uuid());
  this->SetType(type_);
  this->SetNode(node_);
}

Message::Message(zMessage::Message &msg_)
{
  this->SetJson(msg_.GetJson());
}

Message::~Message()
{
}

zNode::Node
Message::GetNode()
{
  zData::Data data;
  this->GetData().GetChild(Message::STR_NODE, data);
  zNode::Node node(data);
  return (node);
}

bool
Message::SetNode(zNode::Node &node_)
{
  bool status = false;

  zData::Data data = this->GetData();
  status = data.PutChild(Message::STR_NODE, node_);
  status = this->SetData(data);

  return (status);
}

}
}
