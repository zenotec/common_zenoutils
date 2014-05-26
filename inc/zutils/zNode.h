//*****************************************************************************
//    Copyright (C) 2014 ZenoTec LLC (http://www.zenotec.net)
//
//    File: zNode.h
//    Description:
//
//*****************************************************************************

#ifndef _ZNODE_H_
#define _ZNODE_H_

#include <stdint.h>
#include <string>
#include <map>
#include <list>

#include "zutils/zData.h"
#include "zutils/zQueue.h"
#include "zutils/zTimer.h"

namespace zUtils
{

class zNodeTable;

//**********************************************************************
// zNode Class
//**********************************************************************
class zNode : public zData
{

  static const std::string ROOT;
  static const std::string TYPE;
  static const std::string ID;

  friend class zNodeTable;

public:
  zNode(const zData &node_);
  zNode(const std::string &type_ = "", const std::string &id_ = "");
  virtual
  ~zNode();

  bool
  operator==(const zNode &other_) const;
  bool
  operator!=(const zNode &other_) const;

  std::string
  GetType() const;
  void
  SetType(const std::string &type_);

  std::string
  GetId() const;
  void
  SetId(const std::string &id_);

protected:
  uint32_t _tardyCnt;

private:

};

//**********************************************************************
// zNodeTableObserver Class
//**********************************************************************
class zNodeTableObserver
{
public:

  enum Event
  {
    NONE = 0, NEW, REMOVED, TARDY, STALE, RETIRED, LAST
  };

  virtual void
  EventHandler(zNodeTableObserver::Event event_, const zNode *node_) = 0;

protected:

private:

};

//**********************************************************************
// zNodeTable Class
//**********************************************************************
class zNodeTable : private zTimerHandler
{
public:

  zNodeTable();
  virtual
  ~zNodeTable();

  void
  GetConf(uint32_t &int_, uint32_t &tardy_, uint32_t &stale_, uint32_t &retire_);

  bool
  SetConf(uint32_t int_ = 500, uint32_t tardy_ = 3000, uint32_t stale_ = 5000, uint32_t retire_ =
      10000);

  bool
  AddNode(const zNode &node_);

  bool
  UpdateNode(const std::string &id_);

  bool
  RemoveNode(const std::string &id_);

  zNode *
  FindNode(const std::string &id_);

  void
  GetNodeList(std::list<zNode> &nodes_);

  void
  Register(zNodeTableObserver *obsvr_);

  void
  Unregister(zNodeTableObserver *obsvr_);

  virtual void
  TimerTick();

protected:

private:

  zNodeTable(const zNodeTable &other_);

  void
  _notifyObservers(zNodeTableObserver::Event event_, const zNode *node_);

  zMutex _lock;
  zTimer _timer;

  std::map<std::string, zNode> _nodeTable;
  std::list<zNodeTableObserver *> _observers;

  uint32_t _interval; // msecs
  uint32_t _tardy; // cumulative msecs
  uint32_t _stale; // cumulative msecs
  uint32_t _retire; // cumulative msecs
};

}

#endif /* _ZNODE_H_ */
