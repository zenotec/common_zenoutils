//*****************************************************************************
//    Copyright (C) 2015 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zConf.h
//    Description:
//
//*****************************************************************************

#ifndef __ZCONF_H__
#define __ZCONF_H__

#include <string>
#include <list>

#include <zutils/zData.h>

namespace zUtils
{
namespace zConf
{

class Observer;

class Data : public zData::Data
{

public:

  enum STATE
  {
    STATE_ERR = -1,
    STATE_NONE = 0,
    STATE_PREGET = 1,
    STATE_PRESET = 2,
    STATE_POSTGET = 3,
    STATE_POSTSET = 4,
    STATE_PRECOMMIT = 5,
    STATE_POSTCOMMIT = 6,
    STATE_LAST
  };

  Data();

  virtual
  ~Data();

  bool
  Register(zConf::Observer *obs_);

  bool
  Unregister(zConf::Observer *obs_);

protected:

private:

  void
  _notify(zConf::Data::STATE state_);

  std::list<zConf::Observer *> _obsList;

};

class Observer
{
public:
  virtual void
  Handle(Data::STATE state_, zData::Data &item_) = 0;
protected:

private:
};

class Connector
{
public:

  virtual bool
  Load() = 0;
  virtual bool
  Store() = 0;

protected:

private:

};

class FileConnector : public Connector
{
public:
  FileConnector();
  virtual
  ~FileConnector();

  virtual bool
  Load();

  virtual bool
  Store();

protected:

private:

};

}
}

#endif /* __ZCONF_H__ */
