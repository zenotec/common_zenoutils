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

#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include <sys/eventfd.h>

#include <zutils/zSem.h>

namespace zUtils
{
namespace zSem
{

//*****************************************************************************
// Semaphore Class
//*****************************************************************************

Semaphore::Semaphore(const uint64_t value_) :
    _fd(0)
{
  this->_fd = eventfd(value_, (EFD_NONBLOCK | EFD_SEMAPHORE));
}

Semaphore::~Semaphore()
{
  if (this->_fd)
    close(this->_fd);
}

int
Semaphore::GetFd() const
{
  return (this->_fd);
}

bool
Semaphore::Post(const uint64_t value_) const
{
  bool status = false;
  if (this->_fd)
  {
    status = (write(this->_fd, &value_, sizeof(value_)) == sizeof(value_));
  }
  return (status);
}

bool
Semaphore::Wait() const
{
  return (this->TimedWait(-1));
}

bool
Semaphore::TryWait() const
{
  return (this->TimedWait(0));
}

bool
Semaphore::TimedWait(int msec_) const
{
  bool status = false;
  int fd_cnt = 1;
  struct pollfd fds[1] = { 0 };

  // Initialize
  fds[0].fd = this->_fd;
  fds[0].events = (POLLIN | POLLERR);

  int ret = poll(fds, fd_cnt, msec_);
  switch (ret)
  {
    case 1:
    {
      uint64_t cntr = 0;
      // Poll successful
      if (fds[0].revents == POLLIN)
      {
        status = ((read(this->_fd, &cntr, sizeof(cntr)) == sizeof(cntr)) && (cntr == 1));
      }
      break;
    }
    case 0:
      // Poll timed out
      break;
    case -EINTR:
      // A signal interrupted poll; exit flag should be set
      fprintf(stderr, "Poll interrupted by signal\n"); // TODO: Debug code, remove when no longer needed
      break;
    default:
    {
      if (ret < 0)
      {
        fprintf(stderr, "Semaphore encountered a poll error: %d\n", ret);
      }
      break;
    }
  }
  return (status);
}

bool
Semaphore::Reset()
{
  bool status = false;
  return (status);
}

}
}
