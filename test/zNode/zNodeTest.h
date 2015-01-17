/*
 * zNodeTest.h
 *
 *  Created on: Jan 10, 2015
 *      Author: kmahoney
 */

#ifndef _ZNODETEST_H_
#define _ZNODETEST_H_

#include "UnitTest.h"

#include <zutils/zNode.h>

int
zNodeTest_NodeDefaults(void* arg_);
int
zNodeTest_NodeTableDefaults(void* arg_);
int
zNodeTest_NodeGetSet(void* arg_);
int
zNodeTest_NodeTableGetSet(void* arg_);

using namespace Test;
using namespace zUtils;

class TestObserver : public zNode::Observer
{
public:
  TestObserver() :
      _count(0)
  {
  }
  virtual
  ~TestObserver()
  {
  }

  int
  GetCount()
  {
    return (this->_count);
  }

protected:
  virtual void
  EventHandler(zNode::Observer::EVENT event_, const zNode::Node &node_)
  {
    this->_count++;
  }

private:
  int _count;
};

#endif /* _ZNODETEST_H_ */
