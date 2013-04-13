/// @author Alexander Rykovanov 2012
/// @email rykovanov.as@gmail.com
/// @brief Test of OpcUa::Thread.
/// @license GNU LGPL
///
/// Distributed under the GNU LGPL License
/// (See accompanying file LICENSE or copy at 
/// http://www.gnu.org/licenses/lgpl.html)
///

#include <internal/thread.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace
{

  class ThreadObserver : public ::OpcUa::Internal::ThreadObserver
  {
  public:
    typedef NiceMock<ThreadObserver> Mock;
  public:
    MOCK_METHOD0(OnSuccess, void());
    MOCK_METHOD1(OnError, void (const std::exception& exc));
  };

  void DoNothing()
  {
  }

  void ThrowsException()
  {
    throw std::logic_error("oppps!");
  }
}

TEST(OpcUaThread, CallsOnSuccess)
{
  ThreadObserver::Mock observer;
  EXPECT_CALL(observer, OnSuccess()).Times(1);
  EXPECT_CALL(observer, OnError(_)).Times(0);

  OpcUa::Internal::Thread thread(std::function<void()>(DoNothing), &observer);
  thread.Join();  
}

TEST(OpcUaThread, CallsOnError)
{
  ThreadObserver::Mock observer;
  EXPECT_CALL(observer, OnSuccess()).Times(0);
  EXPECT_CALL(observer, OnError(_)).Times(1);

  OpcUa::Internal::Thread thread(std::function<void()>(ThrowsException), &observer);
  thread.Join();  
}

