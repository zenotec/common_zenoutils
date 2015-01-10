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
namespace zNode
{

class NodeTable;

//**********************************************************************
// zNode Class
//**********************************************************************
class Node : public zData::Data
{

  static const std::string ROOT;
  static const std::string TYPE;
  static const std::string ID;

  friend class NodeTable;

public:
  Node(const zData::Data &node_);
  Node(const std::string &type_ = "", const std::string &id_ = "");
  virtual
  ~Node();

  bool
  operator==(const Node &other_) const;
  bool
  operator!=(const Node &other_) const;

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
class NodeTableObserver
{
public:

  enum Event
  {
    NONE = 0, NEW, REMOVED, TARDY, STALE, RETIRED, LAST
  };

  virtual void
  EventHandler(NodeTableObserver::Event event_, const Node *node_) = 0;

protected:

private:

};

//**********************************************************************
// zNodeTable Class
//**********************************************************************
class NodeTable : private zTimer::TimerObserver
{
public:

  NodeTable();
  virtual
  ~NodeTable();

  void
  GetConf(uint32_t &int_, uint32_t &tardy_, uint32_t &stale_, uint32_t &retire_);

  bool
  SetConf(uint32_t int_ = 500, uint32_t tardy_ = 3000, uint32_t stale_ = 5000, uint32_t retire_ =
      10000);

  bool
  AddNode(const Node &node_);

  bool
  UpdateNode(const std::string &id_);

  bool
  RemoveNode(const std::string &id_);

  Node *
  FindNode(const std::string &id_);

  void
  GetNodeList(std::list<Node> &nodes_);

  void
  Register(NodeTableObserver *obsvr_);

  void
  Unregister(NodeTableObserver *obsvr_);

  virtual void
  TimerTick();

protected:

private:

  NodeTable(const NodeTable &other_);

  void
  _notifyObservers(NodeTableObserver::Event event_, const Node *node_);

  zSem::Mutex _lock;
  zTimer::Timer _timer;

  std::map<std::string, Node> _nodeTable;
  std::list<NodeTableObserver *> _observers;

  uint32_t _interval; // msecs
  uint32_t _tardy; // cumulative msecs
  uint32_t _stale; // cumulative msecs
  uint32_t _retire; // cumulative msecs
};

}
}

#endif /* _ZNODE_H_ */
