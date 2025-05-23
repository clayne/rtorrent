#include "config.h"

#include "test_main_thread.h"

#include "globals.h"
#include "test/helpers/mock_function.h"
#include "torrent/exceptions.h"
#include "torrent/poll.h"
#include "torrent/net/resolver.h"
#include "torrent/utils/scheduler.h"

std::unique_ptr<TestMainThread>
TestMainThread::create() {
  if (torrent::Poll::slot_create_poll() == nullptr)
    set_create_poll();

  // Needs to be called before Thread is created.
  mock_redirect_defaults();

  auto thread = new TestMainThread();
  return std::unique_ptr<TestMainThread>(thread);
}

TestMainThread::TestMainThread() {}

TestMainThread::~TestMainThread() {
  m_self = nullptr;
}

void
TestMainThread::init_thread() {
  if (!torrent::Poll::slot_create_poll())
    throw torrent::internal_error("ThreadMain::init_thread(): Poll::slot_create_poll() not valid.");

  m_poll = std::unique_ptr<torrent::Poll>(torrent::Poll::slot_create_poll()());
  m_resolver = std::make_unique<torrent::net::Resolver>();
  m_state = STATE_INITIALIZED;

  //m_instrumentation_index = INSTRUMENTATION_POLLING_DO_POLL_MAIN - INSTRUMENTATION_POLLING_DO_POLL;

  init_thread_local();
}

void
TestMainThread::call_events() {
  process_callbacks();
}

std::chrono::microseconds
TestMainThread::next_timeout() {
  return std::chrono::microseconds(10min);
}
