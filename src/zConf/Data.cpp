//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: Data.cpp
//    Description:
//
//*****************************************************************************

#include <zutils/zConf.h>

namespace zUtils
{
namespace zConf
{

Data::Data()
{
}

Data::~Data()
{
}

bool
Data::Register(Observer* obs_)
{
  if (!obs_)
  {
    return (false);
  }
  this->_obsList.push_back(obs_);
  return(true);
}

bool
Data::Unregister(Observer* obs_)
{
  if (!obs_)
  {
    return (false);
  }
  this->_obsList.remove(obs_);
  return(true);
}

void
Data::_notify(Data::STATE state_)
{
  std::list<Observer*>::iterator it = this->_obsList.begin();
  std::list<Observer*>::iterator end = this->_obsList.end();
  for (; it != end; ++it)
  {
    switch (state_)
    {
    case Data::STATE_PRECOMMIT:
      (*it)->Handle(state_, *this);
      break;
    default:
      break;
    }
  }
}

}
}
