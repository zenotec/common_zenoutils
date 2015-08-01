#ifndef _ZGPIO_H_
#define _ZGPIO_H_

#include <stdint.h>

#include <string>
#include <list>

#include <zutils/zSwitch.h>

namespace zUtils
{
namespace zGpio
{

class Handler;

class Port
{
  friend class Handler;

  static const std::string DEFAULT_ROOT;

public:
  enum DIR
  {
    DIR_ERR = -1,
    DIR_NONE = 0,
    DIR_IN = 1,
    DIR_DEF = 1,
    DIR_OUT = 2,
    DIR_INOUT = 3,
    DIR_LAST
  };

  enum STATE
  {
    STATE_ERR = -1,
    STATE_NONE = 0,
    STATE_DEF = 0,
    STATE_ACTIVE = 1,
    STATE_INACTIVE = 2,
    STATE_LAST
  };

  Port(uint32_t id_, Port::DIR dir_ = Port::DIR_DEF,
      Port::STATE initialState_ = Port::STATE_DEF,
      const std::string& root_ = Port::DEFAULT_ROOT);

  virtual
  ~Port();

  uint32_t
  GetId();

protected:

  static const std::string EXPORT_FILENAME;
  static const std::string UNEXPORT_FILENAME;

  static const std::string DIR_FILENAME;
  static const std::string DIR_IN_STR;
  static const std::string DIR_OUT_STR;

  static const std::string STATE_FILENAME;
  static const std::string STATE_ACTIVE_STR;
  static const std::string STATE_INACTIVE_STR;

  bool
  _open();
  bool
  _close();

  Port::DIR
  _getDirection();

  bool
  _setDirection(Port::DIR dir_);

  Port::STATE
  _getState();

  bool
  _setState(Port::STATE state_);

  std::string
  _getClassDir();

  std::string
  _getRootDir();

  bool
  _setRootDir(const std::string& root_);

  Port::DIR _dir;
  Port::STATE _state;

private:

  std::string _root;

  bool _opened;
  uint32_t _id;
};

class Handler
{

public:

  Handler();

  virtual
  ~Handler();

  bool
  AddPort(Port* port_);

  bool
  RemovePort(Port* port_);

  Port*
  GetPort(uint32_t id_);

  Port::STATE
  GetState();

  bool
  SetState(Port::STATE state_);

protected:

private:

  std::list<Port*> _portList;

};

class Switch : public zSwitch::Switch, public Handler
{

public:

  Switch(zSwitch::Switch::STATE state_);

  virtual
  ~Switch();

protected:

private:

    virtual bool
    _turnOn();

    virtual bool
    _turnOff();

};

}
}

#endif /* _ZGPIO_H_ */
