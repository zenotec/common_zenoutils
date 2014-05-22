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
    zData(zNode::ROOT)
{
  this->SetType(type_);
  this->SetId(id_);
  this->SetTardyCnt(0);
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

uint32_t
zNode::GetTardyCnt() const
{
  return (this->_tardyCnt);
}

void
zNode::SetTardyCnt(const uint32_t &cnt_)
{
  this->_tardyCnt = cnt_;
}

//**********************************************************************
// NodeTable
//**********************************************************************
zNodeTable::zNodeTable()
{
  // Start timer
  this->_timer.Register(this);
  this->_timer.Start(100000);
  // Unlock
  this->_lock.Unlock();
}

zNodeTable::~zNodeTable()
{
  this->_timer.Stop();
}

bool
zNodeTable::AddNode(const zNode &node_)
{
  this->_lock.Lock();
  this->_nodeTable[node_.GetId()] = node_;
  this->_notifyObservers(zNodeTableObserver::NEW, this->_nodeTable[node_.GetId()]);
  this->_lock.Unlock();
  return (true);
}

bool
zNodeTable::RemoveNode(const std::string &id_)
{
  bool stat = false;

  this->_lock.Lock();
  if (this->_nodeTable.count(id_))
  {
    stat = true;
    this->_notifyObservers(zNodeTableObserver::REMOVED, this->_nodeTable[id_]);
    this->_nodeTable.erase(id_);
  } // end if
  this->_lock.Unlock();
  return (stat);
}

zNode *
zNodeTable::FindNode(const std::string &id_)
{
  zNode *node = 0;
  std::map<std::string, zNode>::iterator itr;

  this->_lock.Lock();
  itr = this->_nodeTable.find(id_);
  if (itr != this->_nodeTable.end())
  {
    node = &itr->second;
  } // end if
  this->_lock.Unlock();
  return (node);
}

void
zNodeTable::GetNodeList(std::list<zNode> &nodes_)
{
  this->_lock.Lock();
  std::map<std::string, zNode>::iterator itr = this->_nodeTable.begin();
  std::map<std::string, zNode>::iterator end = this->_nodeTable.end();
  for (; itr != end; ++itr)
  {
    nodes_.push_front(itr->second);
  } // end for
  this->_lock.Unlock();
}

void
zNodeTable::Register(zNodeTableObserver *obs_)
{
  this->_lock.Lock();
  this->_observers.push_front(obs_);
  this->_lock.Unlock();
}

void
zNodeTable::Unregister(zNodeTableObserver *obs_)
{
  this->_lock.Lock();
  this->_observers.remove(obs_);
  this->_lock.Unlock();
}

void
zNodeTable::TimerTick()
{
  this->_lock.Lock();
  std::map<std::string, zNode>::iterator itr = this->_nodeTable.begin();
  std::map<std::string, zNode>::iterator end = this->_nodeTable.end();
  while (itr != end)
  {
    if (itr->second.GetTardyCnt() > 1)
    {
      itr->second.SetTardyCnt(itr->second.GetTardyCnt() - 1);
      ++itr;
    }
    else if (itr->second.GetTardyCnt() == 1)
    {
      ZLOG_WARN("Tardy zNode: " + itr->second.GetId());
      this->_notifyObservers(zNodeTableObserver::TARDY, itr->second);
      itr->second.SetTardyCnt(itr->second.GetTardyCnt() - 1);
      ++itr;
    } // end if
    else
    {
      ZLOG_WARN("Retiring zNode: " + itr->second.GetId());
      this->_notifyObservers(zNodeTableObserver::RETIRED, itr->second);
      this->_nodeTable.erase(itr++);
    } // end else
  } // end for
  this->_lock.Unlock();
}

void
zNodeTable::_notifyObservers(zNodeTableObserver::Event event_, zNode &node_)
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

