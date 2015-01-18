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

#include <zutils/zData.h>
#include <zutils/zQueue.h>
#include <zutils/zTimer.h>

namespace zUtils
{
namespace zNode
{

class Table;

//**********************************************************************
// zNode::Node Class
//**********************************************************************
class Node : public zData::Data
{

  static const std::string ROOT;
  static const std::string TYPE;
  static const std::string ID;

  friend class Table;

public:

  Node(const zData::Data &node_);

  Node(const std::string &type_ = "");

  virtual
  ~Node();

  bool
  operator==(const Node &other_) const;
  bool
  operator!=(const Node &other_) const;
  bool
  operator<(const Node &other_) const;
  bool
  operator>(const Node &other_) const;

  std::string
  GetType() const;
  bool
  SetType(const std::string &type_);

  std::string
  GetId() const;
  bool
  SetId(const std::string &id_);

protected:
  uint32_t _tardyCnt;

private:

};

//**********************************************************************
// zNode::Observer Class
//**********************************************************************
class Observer
{
public:

  enum EVENT
  {
    EVENT_ERR = -1,
    EVENT_NONE = 0,
    EVENT_NEW = 1,
    EVENT_REMOVED = 2,
    EVENT_TARDY = 3,
    EVENT_STALE = 4,
    EVENT_RETIRED = 5,
    EVENT_LAST
  };

  virtual void
  EventHandler(Observer::EVENT event_, const Node &node_) = 0;

protected:

private:

};

//**********************************************************************
// zNode::Table Class
//**********************************************************************
class Table : private zTimer::Observer
{
public:

  Table();
  virtual
  ~Table();

  void
  GetConf(uint32_t &int_, uint32_t &tardy_, uint32_t &stale_, uint32_t &retire_);

  bool
  SetConf(uint32_t int_ = 500, uint32_t tardy_ = 3000, uint32_t stale_ = 5000, uint32_t retire_ =
      10000);

  bool
  AddNode(const zNode::Node &node_);

  bool
  UpdateNode(const std::string &id_);

  bool
  RemoveNode(const std::string &id_);

  Node *
  FindNode(const std::string &id_);

  void
  GetNodeList(std::list<zNode::Node> &nodes_);

  void
  Register(zNode::Observer *obsvr_);

  void
  Unregister(zNode::Observer *obsvr_);

  virtual void
  TimerTick();

protected:

private:

  Table(const Table &other_);

  void
  _notifyObservers(zNode::Observer::EVENT event_, const zNode::Node &node_);

  zSem::Mutex _lock;
  zTimer::Timer _timer;

  std::map<std::string, zNode::Node> _nodeTable;
  std::list<zNode::Observer *> _observers;

  uint32_t _interval; // msecs
  uint32_t _tardy; // cumulative msecs
  uint32_t _stale; // cumulative msecs
  uint32_t _retire; // cumulative msecs
};

}
}

#endif /* _ZNODE_H_ */
