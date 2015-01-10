/*
 * Ack.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: kmahoney
 */

#include "zutils/zMessage.h"

namespace zUtils
{
namespace zMessage
{

//**********************************************************************
// ackMessage
//**********************************************************************

const std::string AckMessage::TYPE = "Ack";

const std::string AckMessage::ID = "Id";

AckMessage::AckMessage()
{
  this->SetType(AckMessage::TYPE);
  zData::Data data(this->GetData());
  data.SetValue(AckMessage::ID, "");
  this->SetData(data);
}

AckMessage::~AckMessage()
{
}

}
}

