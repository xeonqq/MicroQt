﻿#pragma once

#include "LoadMonitor.h"
#include "Signal.h"
#ifdef ARDUINO
#include <SimplyAtomic.h>
#else
#define ATOMIC(x)
#warning "Not thread safe in host env!"
#endif

namespace MicroQt {
class EventLoop {
  friend class Synchronizer;

public:
  int exec();
  void exit(int a_exitCode);
  bool isRunning() { return !m_exit; }

  void enqueueEvent(function<void()> a_event) {
    ATOMIC() { m_events.push_back(a_event); }
  }

  uint32_t registerTask(function<void()> a_function) {
    return m_taskUpdate.connect(a_function);
  }
  void unregisterTask(uint32_t a_connection) {
    return m_taskUpdate.disconnect(a_connection);
  }

  // Setting this to 0 will deactivate the cpu and ram load logging
  void setLogIntervalMs(uint32_t a_intervalMs = 0) {
    m_loadMonitor.setIntervalMs(a_intervalMs);
  }

  void update();

private:
  void processEvents();

private:
  LoadMonitor m_loadMonitor;
  Signal<> m_taskUpdate;
  Vector<function<void()>> m_events;
  bool m_exit = true;
  int m_exitCode = 0;
};
extern EventLoop eventLoop;
} // namespace MicroQt
