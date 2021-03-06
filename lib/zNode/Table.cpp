/*
 * Copyright (c) 2014-2016 ZenoTec LLC (http://www.zenotec.net)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
Table::Table(const uint32_t period_)
{
  // Start timer
  this->_timer.Register(this);
  this->_timer.Start(period_ * 1000);
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

bool
Table::Add(zNode::Node &node_)
{
  bool status = false;
  std::list<std::pair<zNode::Observer::EVENT, zNode::Node> > notifyList;
  std::pair<zNode::Observer::EVENT, zNode::Node> notification;

  // Obtain lock
  if (!this->_lock.Lock())
  {
    return (false);
  } // end if

  // Verify node identifier is unique
  if (!node_.GetId().empty() && !node_.GetAddress().empty())
  {
    if (!this->_idMap.count(node_.GetId()))
    {
      ZLOG_INFO("Table::Add(): Adding new node: " + node_.GetId());
      notification = std::make_pair(zNode::Observer::EVENT_NEW, node_);
      notifyList.push_back(notification);
      this->_nodeList.push_front(node_);
      this->_idMap[node_.GetId()] = &this->_nodeList.front();
      status = true;
    }
  }

  // Release lock
  this->_lock.Unlock();

  // Conditionally notify observers
  if (status)
  {
    this->_notify(notifyList);
  }
  else
  {
    ZLOG_ERR("zNode::Table::Add(): Node already exists: " +
        node_.GetId() + " : " + node_.GetAddress());
  }

  // Return status
  return (status);
}

bool
Table::Remove(zNode::Node &node_)
{
  bool status = false;
  std::list<std::pair<zNode::Observer::EVENT, zNode::Node> > notifyList;
  std::pair<zNode::Observer::EVENT, zNode::Node> notification;

  // Obtain lock
  if (!this->_lock.Lock())
  {
    return (false);
  } // end if

  // Remove node from identifier map
  if (!node_.GetId().empty())
  {
    // Find identifier in map
    std::map<std::string, zNode::Node *>::iterator it = this->_idMap.find(node_.GetId());
    if (it != this->_idMap.end())
    {
      ZLOG_INFO("Table::Remove(): Removing node: " + node_.GetId());
      notification = std::make_pair(zNode::Observer::EVENT_REMOVED, node_);
      notifyList.push_back(notification);
      this->_idMap.erase(it);
      this->_nodeList.remove(node_);
      status = true;
    }
  }

  // Release lock
  this->_lock.Unlock();

  // Conditionally notify observers
  if (status)
  {
    this->_notify(notifyList);
  }
  else
  {
    ZLOG_ERR("zNode::Table::Remove(): Node does not exist: " +
        node_.GetId() + " : " + node_.GetAddress());
  }

  // Return status
  return (status);
}

zNode::Node *
Table::Find(const std::string &id_)
{
  zNode::Node *node = NULL;
  if (!id_.empty())
  {
    std::map<std::string, zNode::Node *>::iterator it = this->_idMap.find(id_);
    if (it != this->_idMap.end())
    {
      node = it->second;
    }
  }
  return (node);
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

  std::list<std::pair<zNode::Observer::EVENT, zNode::Node> > notifyList;
  std::pair<zNode::Observer::EVENT, zNode::Node> notification;

  // Obtain lock
  if (!this->_lock.Lock())
  {
    return;
  } // end if

  std::list<zNode::Node>::iterator it = this->_nodeList.begin();
  std::list<zNode::Node>::iterator end = this->_nodeList.end();
  for (; it != end; ++it)
  {
    switch (it->GetState())
    {
    case zNode::Node::STATE_ONLINE:
      it->SetState(zNode::Node::STATE_PENDING_TARDY);
      break;
    case zNode::Node::STATE_PENDING_TARDY:
      it->SetState(zNode::Node::STATE_TARDY);
      notification = std::make_pair(zNode::Observer::EVENT_TARDY, *it);
      notifyList.push_back(notification);
      break;
    case zNode::Node::STATE_TARDY:
      it->SetState(zNode::Node::STATE_PENDING_STALE);
      break;
    case zNode::Node::STATE_PENDING_STALE:
      it->SetState(zNode::Node::STATE_STALE);
      notification = std::make_pair(zNode::Observer::EVENT_STALE, *it);
      notifyList.push_back(notification);
      break;
    case zNode::Node::STATE_STALE:
      it->SetState(zNode::Node::STATE_PENDING_OFFLINE);
      break;
    case zNode::Node::STATE_PENDING_OFFLINE:
      it->SetState(zNode::Node::STATE_OFFLINE);
      notification = std::make_pair(zNode::Observer::EVENT_OFFLINE, *it);
      notifyList.push_back(notification);
      break;
    case zNode::Node::STATE_OFFLINE:
      it->SetState(zNode::Node::STATE_PENDING_RETIRE);
      break;
    case zNode::Node::STATE_PENDING_RETIRE:
      notification = std::make_pair(zNode::Observer::EVENT_RETIRED, *it);
      notifyList.push_back(notification);
      break;
    default:
      break;
    }
  } // end for

  // Release lock
  this->_lock.Unlock();

  // Notify observers
  this->_notify(notifyList);

  // Return
  return;
}

void
Table::_notify(std::list<std::pair<zNode::Observer::EVENT, zNode::Node> > notifications_)
{

  std::list<std::pair<zNode::Observer::EVENT, zNode::Node> >::iterator not_it;
  std::list<zNode::Observer *>::iterator obs_it;

  for (not_it = notifications_.begin(); not_it != notifications_.end(); ++not_it)
  {

    // Notify observers of event
    for (obs_it = this->_observers.begin(); obs_it != this->_observers.end(); ++obs_it)
    {
      (*obs_it)->EventHandler(not_it->first, not_it->second);
    } // end for

    // Conditionally remove node from table
    if (not_it->first == zNode::Observer::EVENT_RETIRED)
    {
      this->Remove(not_it->second);
    }
  }

  return;
}

}
}

