/*
 * Node.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: kmahoney
 */

#include <errno.h>
#include <string.h>

#include <zutils/zLog.h>
#include <zutils/zTimer.h>
#include <zutils/zNode.h>

namespace zUtils
{
namespace zNode
{

//**********************************************************************
// NodeTable Class
//**********************************************************************
Table::Table() :
    _interval(500), _tardy(3000), _stale(5000), _retire(10000)
{
  // Start timer
  this->_timer.Register(this);
  this->_timer.Start(this->_interval * 1000);
  this->_lock.Unlock();
}

Table::~Table()
{
  // Signal the Management thread to stop by stopping the timer thread
  if (this->_lock.Lock())
  {
    this->_timer.Start(0);
  } // end if
}

void
Table::GetConf(uint32_t &int_, uint32_t &tardy_, uint32_t &stale_, uint32_t &retire_)
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
Table::SetConf(uint32_t int_, uint32_t tardy_, uint32_t stale_, uint32_t retire_)
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
Table::AddNode(const Node &node_)
{
  if (this->_lock.Lock())
  {
    this->_nodeTable[node_.GetId()] = node_;
    this->_notifyObservers(zNode::Observer::EVENT_NEW, this->_nodeTable[node_.GetId()]);
    this->_lock.Unlock();
  } // end if
  return (true);
}

bool
Table::UpdateNode(const std::string &id_)
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
Table::RemoveNode(const std::string &id_)
{
  bool stat = false;

  if (this->_lock.Lock())
  {
    if (this->_nodeTable.count(id_))
    {
      stat = true;
      this->_notifyObservers(zNode::Observer::EVENT_REMOVED, this->_nodeTable[id_]);
      this->_nodeTable.erase(id_);
    } // end if
    this->_lock.Unlock();
  } // end if
  return (stat);
}

Node *
Table::FindNode(const std::string &id_)
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
Table::GetNodeList(std::list<zNode::Node> &nodes_)
{
  if (this->_lock.Lock())
  {
    std::map<std::string, zNode::Node>::iterator itr = this->_nodeTable.begin();
    std::map<std::string, zNode::Node>::iterator end = this->_nodeTable.end();
    for (; itr != end; ++itr)
    {
      nodes_.push_front(itr->second);
    } // end for
    this->_lock.Unlock();
  } // end if
}

void
Table::Register(zNode::Observer *obs_)
{
  if (this->_lock.Lock())
  {
    this->_observers.push_front(obs_);
    this->_lock.Unlock();
  } // end if
}

void
Table::Unregister(zNode::Observer *obs_)
{
  if (this->_lock.Lock())
  {
    this->_observers.remove(obs_);
    this->_lock.Unlock();
  } // end if
}

void
Table::TimerTick()
{
  Node *node = 0;
  if (this->_lock.Lock())
  {
    std::map<std::string, zNode::Node>::iterator itr = this->_nodeTable.begin();
    std::map<std::string, zNode::Node>::iterator end = this->_nodeTable.end();
    while (itr != end)
    {
      node = &itr->second;
      // Test for expired node condition
      if (node->_tardyCnt >= (this->_retire / this->_interval))
      {
        ZLOG_WARN("Retiring Node: " + node->GetId());
        this->_notifyObservers(zNode::Observer::EVENT_RETIRED, *node);
        this->_nodeTable.erase(itr++);
      } // end if
      else
      {
        // Test for tardy node condition
        if (node->_tardyCnt == (this->_tardy / this->_interval))
        {
          ZLOG_WARN("Tardy Node: " + node->GetId());
          this->_notifyObservers(zNode::Observer::EVENT_TARDY, *node);
        } // end if
        else
        {
          // Test for stale node condition
          if (node->_tardyCnt == (this->_stale / this->_interval))
          {
            ZLOG_WARN("Stale Node: " + node->GetId());
            this->_notifyObservers(zNode::Observer::EVENT_STALE, *node);
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
Table::_notifyObservers(zNode::Observer::EVENT event_, const zNode::Node &node_)
{
  // Critical section; needs to be protected by caller
  std::list<zNode::Observer *>::iterator itr = this->_observers.begin();
  std::list<zNode::Observer *>::iterator end = this->_observers.end();
  for (; itr != end; itr++)
  {
    (*itr)->EventHandler(event_, node_);
  } // end for
}

}
}

