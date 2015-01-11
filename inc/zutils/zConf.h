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

class Data;

class Observer
{
public:
  virtual void
  Prepare(Data *item_) = 0;
  virtual void
  Commit(Data *item_) = 0;
protected:

private:
};

class Data : private zData::Data
{
  enum STATE
  {
    STATE_ERR = -1,
    STATE_NONE = 0,
    STATE_PREPARE = 1,
    STATE_COMMIT = 2,
    STATE_LAST
  };

public:
  Data();
  virtual
  ~Data();

  bool
  Register(Observer *obs_);
  bool
  Unregister(Observer *obs_);

protected:

private:

  void
  _notify(Data::STATE state_);

  std::list<Observer *> _obsList;

};

class Connector
{
public:
  Connector();
  virtual
  ~Connector();

  virtual bool
  Load() = 0;
  virtual bool
  Store() = 0;

protected:
private:
};

class FileConnector
{
public:
  FileConnector();
  virtual
  ~FileConnector();

  virtual bool
  Load() = 0;
  virtual bool
  Store() = 0;

private:
};

}
}

#endif /* __ZCONF_H__ */
