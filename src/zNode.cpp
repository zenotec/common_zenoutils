/*
 * zNode.cpp
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

//**********************************************************************
// zNode
//**********************************************************************

const std::string zNode::ROOT = "zNode";
const std::string zNode::TYPE = "Type";
const std::string zNode::ID = "Id";

zNode::zNode(const zData &node_) :
    zData(node_), _tardyCnt(0)
{
}

zNode::zNode(const std::string &type_, const std::string &id_) :
    zData(zNode::ROOT), _tardyCnt(0)
{
  this->SetType(type_);
  this->SetId(id_);
  ZLOG_DEBUG("Creating new node: " + type_ + "[" + id_ + "]");
}

zNode::~zNode()
{
}

bool
zNode::operator ==(const zNode &other_) const
{
  return (this->GetId() == other_.GetId());
}

bool
zNode::operator !=(const zNode &other_) const
{
  return (this->GetId() != other_.GetId());
}

std::string
zNode::GetType() const
{
  return (this->GetVal(zNode::TYPE));
}

void
zNode::SetType(const std::string &type_)
{
  this->SetVal(zNode::TYPE, type_);
}

std::string
zNode::GetId() const
{
  return (this->GetVal(zNode::ID));
}

void
zNode::SetId(const std::string &id_)
{
  this->SetVal(zNode::ID, id_);
}

//**********************************************************************
// zNodeTable Class
//**********************************************************************
zNodeTable::zNodeTable() :
    _interval(500), _tardy(3000), _stale(5000), _retire(10000)
{
  // Start timer
  this->_timer.Register(this);
  this->_timer.Start(this->_interval * 1000);
  this->_lock.Unlock();
}

zNodeTable::~zNodeTable()
{
  // Signal the Management thread to stop by stopping the timer thread
  if (this->_lock.Lock())
  {
    this->_timer.Start(0);
  } // end if
}

void
zNodeTable::GetConf(uint32_t &int_, uint32_t &tardy_, uint32_t &stale_, uint32_t &retire_)
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
zNodeTable::SetConf(uint32_t int_, uint32_t tardy_, uint32_t stale_, uint32_t retire_)
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
zNodeTable::AddNode(const zNode &node_)
{
  if (this->_lock.Lock())
  {
    this->_nodeTable[node_.GetId()] = node_;
    this->_notifyObservers(zNodeTableObserver::NEW, &this->_nodeTable[node_.GetId()]);
    this->_lock.Unlock();
  } // end if
  return (true);
}

bool
zNodeTable::UpdateNode(const std::string &id_)
{
  bool stat = false;
  std::map<std::string, zNode>::iterator itr;

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
zNodeTable::RemoveNode(const std::string &id_)
{
  bool stat = false;

  if (this->_lock.Lock())
  {
    if (this->_nodeTable.count(id_))
    {
      stat = true;
      this->_notifyObservers(zNodeTableObserver::REMOVED, &this->_nodeTable[id_]);
      this->_nodeTable.erase(id_);
    } // end if
    this->_lock.Unlock();
  } // end if
  return (stat);
}

zNode *
zNodeTable::FindNode(const std::string &id_)
{
  zNode *node = 0;
  std::map<std::string, zNode>::iterator itr;

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
zNodeTable::GetNodeList(std::list<zNode> &nodes_)
{
  if (this->_lock.Lock())
  {
    std::map<std::string, zNode>::iterator itr = this->_nodeTable.begin();
    std::map<std::string, zNode>::iterator end = this->_nodeTable.end();
    for (; itr != end; ++itr)
    {
      nodes_.push_front(itr->second);
    } // end for
    this->_lock.Unlock();
  } // end if
}

void
zNodeTable::Register(zNodeTableObserver *obs_)
{
  if (this->_lock.Lock())
  {
    this->_observers.push_front(obs_);
    this->_lock.Unlock();
  } // end if
}

void
zNodeTable::Unregister(zNodeTableObserver *obs_)
{
  if (this->_lock.Lock())
  {
    this->_observers.remove(obs_);
    this->_lock.Unlock();
  } // end if
}

void
zNodeTable::TimerTick()
{
  zNode *node = 0;
  if (this->_lock.Lock())
  {
    std::map<std::string, zNode>::iterator itr = this->_nodeTable.begin();
    std::map<std::string, zNode>::iterator end = this->_nodeTable.end();
    while (itr != end)
    {
      node = &itr->second;
      // Test for expired node condition
      if (node->_tardyCnt >= (this->_retire / this->_interval))
      {
        ZLOG_WARN("Retiring zNode: " + node->GetId());
        this->_notifyObservers(zNodeTableObserver::RETIRED, node);
        this->_nodeTable.erase(itr++);
      } // end if
      else
      {
        // Test for tardy node condition
        if (node->_tardyCnt == (this->_tardy / this->_interval))
        {
          ZLOG_WARN("Tardy zNode: " + node->GetId());
          this->_notifyObservers(zNodeTableObserver::TARDY, node);
        } // end if
        else
        {
          // Test for stale node condition
          if (node->_tardyCnt == (this->_stale / this->_interval))
          {
            ZLOG_WARN("Stale zNode: " + node->GetId());
            this->_notifyObservers(zNodeTableObserver::STALE, node);
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
zNodeTable::_notifyObservers(zNodeTableObserver::Event event_, const zNode *node_)
{
  // Critical section; needs to be protected by caller
  std::list<zNodeTableObserver *>::iterator itr = this->_observers.begin();
  std::list<zNodeTableObserver *>::iterator end = this->_observers.end();
  for (; itr != end; itr++)
  {
    (*itr)->EventHandler(event_, node_);
  } // end for
}

}

