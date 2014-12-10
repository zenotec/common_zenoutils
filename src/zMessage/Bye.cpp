/*
 * Bye.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: kmahoney
 */

#include "zutils/zMessage.h"

namespace zUtils
{

//**********************************************************************
// byeMessage
//**********************************************************************

const std::string ByeMessage::TYPE = "Bye";

ByeMessage::ByeMessage()
{
    this->SetType( ByeMessage::TYPE );
}

ByeMessage::~ByeMessage()
{
}

}

