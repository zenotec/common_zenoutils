/*
 * Ack.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: kmahoney
 */

#include "zutils/zMessage.h"

namespace zUtils
{

//**********************************************************************
// ackMessage
//**********************************************************************

const std::string AckMessage::TYPE = "Ack";

const std::string AckMessage::ID = "Id";

AckMessage::AckMessage()
{
    this->SetType( AckMessage::TYPE );
    zData data(this->GetData());
    data.SetVal( AckMessage::ID, "" );
    this->SetData( data );
}

AckMessage::~AckMessage()
{
}

}

