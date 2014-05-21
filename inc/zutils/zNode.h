/*
 * zNode.h
 *
 *  Created on: Jan 13, 2014
 *      Author: Kevin Mahoney
 */

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

class zNode : public zData
{

  static const std::string ROOT;
  static const std::string TYPE;
  static const std::string ID;

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

  uint32_t
  GetTardyCnt() const;
  void
  SetTardyCnt(const uint32_t &cnt_);

protected:

private:
  unsigned int _tardyCnt;

};

class zNodeTableObserver
{
public:

  enum Event
  {
    NONE = 0, NEW, STALE, LAST
  };

  virtual void
  EventHandler(zNodeTableObserver::Event event_, zNode &node_) = 0;

protected:

private:

};

class zNodeTable : public zTimerHandler
{
public:

  zNodeTable();
  virtual
  ~zNodeTable();

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

  bool
  _addNode(const zNode &node_);

  bool
  _removeNode(const std::string &id_);

  zNode *
  _findNode(const std::string &id_);

  void
  _notifyObservers(zNodeTableObserver::Event event_, zNode &node_);

  zMutex _lock;
  zTimer _timer;

  std::map<std::string, zNode> _nodeTable;
  std::list<zNodeTableObserver *>_observers;
};

}

#endif /* _ZNODE_H_ */
