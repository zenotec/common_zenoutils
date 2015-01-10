/*
 * Node.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: kmahoney
 */

#include <errno.h>
#include <string.h>

#include "zutils/zLog.h"
#include "zutils/zTimer.h"
#include "zutils/zNode.h"

namespace zUtils
{
namespace zNode
{

//**********************************************************************
// Node
//**********************************************************************

const std::string Node::ROOT = "zNode";
const std::string Node::TYPE = "Type";
const std::string Node::ID = "Id";

Node::Node(const zData::Data &node_) :
    zData::Data(node_), _tardyCnt(0)
{
}

Node::Node(const std::string &type_, const std::string &id_) :
    zData::Data(Node::ROOT), _tardyCnt(0)
{
  this->SetType(type_);
  this->SetId(id_);
  ZLOG_DEBUG("Creating new node: " + type_ + "[" + id_ + "]");
}

Node::~Node()
{
}

bool
Node::operator ==(const Node &other_) const
{
  return (this->GetId() == other_.GetId());
}

bool
Node::operator !=(const Node &other_) const
{
  return (this->GetId() != other_.GetId());
}

std::string
Node::GetType() const
{
  return (this->GetValue(Node::TYPE));
}

void
Node::SetType(const std::string &type_)
{
  this->SetValue(Node::TYPE, type_);
}

std::string
Node::GetId() const
{
  return (this->GetValue(Node::ID));
}

void
Node::SetId(const std::string &id_)
{
  this->SetValue(Node::ID, id_);
}

//**********************************************************************
// NodeTable Class
//**********************************************************************
NodeTable::NodeTable() :
    _interval(500), _tardy(3000), _stale(5000), _retire(10000)
{
  // Start timer
  this->_timer.Register(this);
  this->_timer.Start(this->_interval * 1000);
  this->_lock.Unlock();
}

NodeTable::~NodeTable()
{
  // Signal the Management thread to stop by stopping the timer thread
  if (this->_lock.Lock())
  {
    this->_timer.Start(0);
  } // end if
}

void
NodeTable::GetConf(uint32_t &int_, uint32_t &tardy_, uint32_t &stale_, uint32_t &retire_)
{
  if (this->_lock.Lock())
  {
    int_ = this->_interval;
    tardy_ = this->_tardy;
    stale_ = this->_stale;
    retire_ = this->_retire;
    this->_lock.Unlock();
  } // end if
}

bool
NodeTable::SetConf(uint32_t int_, uint32_t tardy_, uint32_t stale_, uint32_t retire_)
{
  bool stat = false;
  if (this->_lock.Lock())
  {
    if ((tardy_ < stale_) || (stale_ < retire_))
    {
      this->_interval = int_;
      this->_tardy = tardy_;
      this->_stale = stale_;
      this->_retire = retire_;
      this->_timer.Start(this->_interval * 1000);
      stat = true;
    } // end if
    this->_lock.Unlock();
  } // end if
  return (stat);
}

bool
NodeTable::AddNode(const Node &node_)
{
  if (this->_lock.Lock())
  {
    this->_nodeTable[node_.GetId()] = node_;
    this->_notifyObservers(NodeTableObserver::NEW, &this->_nodeTable[node_.GetId()]);
    this->_lock.Unlock();
  } // end if
  return (true);
}

bool
NodeTable::UpdateNode(const std::string &id_)
{
  bool stat = false;
  std::map<std::string, Node>::iterator itr;

  if (this->_lock.Lock())
  {
    itr = this->_nodeTable.find(id_);
    if (itr != this->_nodeTable.end())
    {
      itr->second._tardyCnt = 0;
      stat = true;
    } // end if
    this->_lock.Unlock();
  } // end if
  return (stat);
}

bool
NodeTable::RemoveNode(const std::string &id_)
{
  bool stat = false;

  if (this->_lock.Lock())
  {
    if (this->_nodeTable.count(id_))
    {
      stat = true;
      this->_notifyObservers(NodeTableObserver::REMOVED, &this->_nodeTable[id_]);
      this->_nodeTable.erase(id_);
    } // end if
    this->_lock.Unlock();
  } // end if
  return (stat);
}

Node *
NodeTable::FindNode(const std::string &id_)
{
  Node *node = 0;
  std::map<std::string, Node>::iterator itr;

  if (this->_lock.Lock())
  {
    itr = this->_nodeTable.find(id_);
    if (itr != this->_nodeTable.end())
    {
      node = &itr->second;
    } // end if
    this->_lock.Unlock();
  } // end if
  return (node);
}

void
NodeTable::GetNodeList(std::list<Node> &nodes_)
{
  if (this->_lock.Lock())
  {
    std::map<std::string, Node>::iterator itr = this->_nodeTable.begin();
    std::map<std::string, Node>::iterator end = this->_nodeTable.end();
    for (; itr != end; ++itr)
    {
      nodes_.push_front(itr->second);
    } // end for
    this->_lock.Unlock();
  } // end if
}

void
NodeTable::Register(NodeTableObserver *obs_)
{
  if (this->_lock.Lock())
  {
    this->_observers.push_front(obs_);
    this->_lock.Unlock();
  } // end if
}

void
NodeTable::Unregister(NodeTableObserver *obs_)
{
  if (this->_lock.Lock())
  {
    this->_observers.remove(obs_);
    this->_lock.Unlock();
  } // end if
}

void
NodeTable::TimerTick()
{
  Node *node = 0;
  if (this->_lock.Lock())
  {
    std::map<std::string, Node>::iterator itr = this->_nodeTable.begin();
    std::map<std::string, Node>::iterator end = this->_nodeTable.end();
    while (itr != end)
    {
      node = &itr->second;
      // Test for expired node condition
      if (node->_tardyCnt >= (this->_retire / this->_interval))
      {
        ZLOG_WARN("Retiring Node: " + node->GetId());
        this->_notifyObservers(NodeTableObserver::RETIRED, node);
        this->_nodeTable.erase(itr++);
      } // end if
      else
      {
        // Test for tardy node condition
        if (node->_tardyCnt == (this->_tardy / this->_interval))
        {
          ZLOG_WARN("Tardy Node: " + node->GetId());
          this->_notifyObservers(NodeTableObserver::TARDY, node);
        } // end if
        else
        {
          // Test for stale node condition
          if (node->_tardyCnt == (this->_stale / this->_interval))
          {
            ZLOG_WARN("Stale Node: " + node->GetId());
            this->_notifyObservers(NodeTableObserver::STALE, node);
          } // end if
        } // end else
        node->_tardyCnt++;
        ++itr;
      } // end else
    } // end while
    this->_lock.Unlock();
  } // end if
}

void
NodeTable::_notifyObservers(NodeTableObserver::Event event_, const Node *node_)
{
  // Critical section; needs to be protected by caller
  std::list<NodeTableObserver *>::iterator itr = this->_observers.begin();
  std::list<NodeTableObserver *>::iterator end = this->_observers.end();
  for (; itr != end; itr++)
  {
    (*itr)->EventHandler(event_, node_);
  } // end for
}

}
}

