/*
 * Hello.cpp
 *
 *  Created on: Feb 4, 2014
 *      Author: kmahoney
 */

#include "zutils/zMessage.h"

namespace zUtils
{

//**********************************************************************
// HelloMessage
//**********************************************************************

const std::string HelloMessage::TYPE = "Hello";

HelloMessage::HelloMessage()
{
    this->SetType( HelloMessage::TYPE );
}

HelloMessage::~HelloMessage()
{
}

}

