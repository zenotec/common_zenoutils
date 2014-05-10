/*
 * zNode.cpp
 *
 *  Created on: Jan 13, 2014
 *      Author: kmahoney
 */

#include <errno.h>
#include <string.h>

#include "zutils/zSelect.h"
#include "zutils/zTimer.h"
#include "zutils/zNode.h"

namespace zUtils
{

//**********************************************************************
// zNode
//**********************************************************************

const std::string zNode::ROOT = "zNode";
const std::string zNode::ID = "Id";

zNode::zNode(const zData &node_) :
    zData(node_), _tardyCnt(0)
{
}

zNode::zNode(const std::string &id_) :
    zData(zNode::ROOT)
{
//    std::cout << std::endl << "Node::Node(): " << std::endl << this->GetJson() << std::endl;
  this->SetId(id_);
  this->SetTardyCnt(0);
//    std::cout << std::endl << "Node::Node(): " << std::endl << this->GetJson() << std::endl;
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
zNodeTable::zNodeTable() :
    managerThreadId(0), exit(false)
{
  pthread_attr_t attr;

  if (pthread_attr_init(&attr))
  {
    std::string errmsg("Error: Cannot initialize thread attributes: ");
    errmsg += std::string(strerror(errno));
    throw errmsg;
  } /* end if */

  if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE))
  {
    std::string errmsg("Error: Cannot set detached state: ");
    errmsg += std::string(strerror(errno));
    throw errmsg;
  } /* end if */

  if (pthread_create(&this->managerThreadId, NULL, zNodeTable::ManagerThread, this))
  {
    std::string errmsg("Error: Cannot create manager thread: ");
    errmsg += std::string(strerror(errno));
    throw errmsg;
  } // end if

  if (pthread_attr_destroy(&attr))
  {
    std::string errmsg("Error: Cannot destroy thread attributes: ");
    errmsg += std::string(strerror(errno));
    throw errmsg;
  } /* end if */

}

zNodeTable::~zNodeTable()
{
  if (this->managerThreadId)
  {
    this->exit = true;
    pthread_join(this->managerThreadId, 0);
  } // end if
}

void
zNodeTable::UpdateNode(zNode &node_)
{
  this->updateQueue.Push(node_);
}

void
zNodeTable::RemoveNode(zNode &node_)
{
  this->removeQueue.Push(node_);
}

bool
zNodeTable::FindNode(const std::string &id_)
{
  return (this->nodeMap.count(id_) != 0);
}

std::list<zNode>
zNodeTable::GetNodeList()
{
  std::list<zNode> nodes;
  std::map<std::string, zNode>::iterator iter = this->nodeMap.begin();
  std::map<std::string, zNode>::iterator end = this->nodeMap.end();

  for (; iter != end; ++iter)
  {
    nodes.push_front(iter->second);
  } // end for

  return (nodes);
}

void *
zNodeTable::ManagerThread(void *arg_)
{
  zNodeTable *self = (zNodeTable *) arg_;

  // Create timer for timed event
  zTimer timer;
  zSelect select;

  // Register events with select loop
  select.RegisterEvent(&timer);
  select.RegisterEvent(&self->updateQueue);
  select.RegisterEvent(&self->removeQueue);

  // Start timer
  timer.Start(1000000);

  while (self->exit == false)
  {

    // Wait for queue event
    select.Wait(100000);

    // Process update queue
    while (self->updateQueue.GetPending())
    {
//      std::cout << "zNodeTable::ManagerThread: Updating" << std::endl;
      zNode node = self->updateQueue.Front();
      self->nodeMap[node.GetId()] = node;
      self->updateQueue.Pop();
    } // end while

    // Process remove queue
    while (self->removeQueue.GetPending())
    {
//      std::cout << "zNodeTable::ManagerThread: Removing" << std::endl;
      zNode node = self->removeQueue.Front();
      self->nodeMap.erase(node.GetId());
      self->removeQueue.Pop();
    } // end while

    // Retire old nodes
    if (timer.GetPending() > 0)
    {
      std::map<std::string, zNode>::iterator iter = self->nodeMap.begin();
      std::map<std::string, zNode>::iterator end = self->nodeMap.end();
      for (; iter != end; ++iter)
      {
        if (iter->second.GetTardyCnt() == 0)
        {
//          std::cout << "zNodeTable::ManagerThread: Retiring" << std::endl;
          self->nodeMap.erase(iter);
        } // end if
        else
        {
          iter->second.SetTardyCnt(iter->second.GetTardyCnt() - 1);
        } // end else
      } // end for
      timer.Acknowledge(timer.GetPending());
    } // end if

  } // end while

  // Reset exit flag
  self->exit = false;

  // Cleanup thread resources
  pthread_exit(0);

  // Return
  return (0);
}

}

