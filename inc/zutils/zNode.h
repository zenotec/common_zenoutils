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
#include <pthread.h>

#include "zutils/zData.h"
#include "zutils/zQueue.h"

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

class zNodeTable
{
public:
  zNodeTable();
  ~zNodeTable();
  void
  UpdateNode(zNode &node_);
  void
  RemoveNode(zNode &node_);
  bool
  FindNode(const std::string &id_);
  std::list<zNode>
  GetNodeList();

protected:

private:
  static void *
  ManagerThread(void *arg_);
  pthread_t managerThreadId;
  bool exit;
  zQueue<zNode> updateQueue;
  zQueue<zNode> removeQueue;
  std::map<std::string, zNode> nodeMap;
};

}

#endif /* _ZNODE_H_ */
