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

#ifndef __ZCOMPATIBILITY_H__
#define __ZCOMPATIBILITY_H__

#if __cplusplus < 201103L

#include <boost/thread/mutex.hpp>
#include <boost/interprocess/smart_ptr/unique_ptr.hpp>
#include <boost/interprocess/smart_ptr/shared_ptr.hpp>
#include <boost/move/move.hpp>
#include <boost/foreach.hpp>

#define MUTEX           boost::mutex
#define TIMED_MUTEX     boost::timed_mutex
#define UNIQUE_LOCK(t)  boost::unique_lock<t>
#define TIMED_LOCK(l,t) ((l).timed_lock(boost::get_system_time() + boost::posix_time::milliseconds (t)))
#define UNIQUE_PTR(p)   boost::interprocess::unique_ptr<p>
#define SHARED_PTR(p)   boost::shared_ptr<p>
#define STATIC_CAST(p)  boost::static_pointer_cast<p>
#define MOVE(p)         boost::move(p)
#define FOREACH(a,b)    BOOST_FOREACH(a,b)

#elif __cplusplus >= 201103L

#include <mutex>
#include <chrono>
#include <memory>
#include <utility>

#define MUTEX           std::mutex
#define TIMED_MUTEX     std::timed_mutex
#define UNIQUE_LOCK(t)  std::unique_lock<t>
#define TIMED_LOCK(l,t) ((l).try_lock_for (std::chrono::milliseconds(t)))
#define UNIQUE_PTR(p)   std::unique_ptr<p>
#define SHARED_PTR(p)   std::shared_ptr<p>
#define STATIC_CAST(p)  std::static_pointer_cast<p>
#define MOVE(p)         std::move(p)
#define FOREACH(a,b)    for (a : b)

#else

#error "Unsupported compiler version"

#endif

#endif /* __ZCOMPATIBILITY_H__ */
