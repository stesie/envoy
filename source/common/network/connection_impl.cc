#include "connection_impl.h"

#include "envoy/event/timer.h"
#include "envoy/common/exception.h"
#include "envoy/network/filter.h"

#include "common/common/assert.h"
#include "common/common/empty_string.h"
#include "common/common/enum_to_int.h"
#include "common/event/dispatcher_impl.h"
#include "common/network/utility.h"

namespace Network {

void ConnectionImplUtility::updateBufferStats(uint64_t delta, uint64_t new_total,
                                              uint64_t& previous_total, Stats::Counter& stat_total,
                                              Stats::Gauge& stat_current) {
  if (delta) {
    stat_total.add(delta);
  }

  if (new_total != previous_total) {
    if (new_total > previous_total) {
      stat_current.add(new_total - previous_total);
    } else {
      stat_current.sub(previous_total - new_total);
    }

    previous_total = new_total;
  }
}

std::atomic<uint64_t> ConnectionImpl::next_global_id_;

ConnectionImpl::ConnectionImpl(Event::DispatcherImpl& dispatcher, int fd,
                               const std::string& remote_address)
    : filter_manager_(*this, *this), remote_address_(remote_address), dispatcher_(dispatcher),
      fd_(fd), id_(++next_global_id_) {

  // Treat the lack of a valid fd (which in practice only happens if we run out of FDs) as an OOM
  // condition and just crash.
  RELEASE_ASSERT(fd_ != -1);

  file_event_ =
      dispatcher_.createFileEvent(fd_, [this](uint32_t events) -> void { onFileEvent(events); });
}

ConnectionImpl::~ConnectionImpl() {
  ASSERT(fd_ == -1);

  // In general we assume that owning code has called close() previously to the destructor being
  // run. This generally must be done so that callbacks run in the correct context (vs. deferred
  // deletion). Hence the assert above. However, call close() here just to be completely sure that
  // the fd is closed and make it more likely that we crash from a bad close callback.
  close(ConnectionCloseType::NoFlush);
}

void ConnectionImpl::addWriteFilter(WriteFilterPtr filter) {
  filter_manager_.addWriteFilter(filter);
}

void ConnectionImpl::addFilter(FilterPtr filter) { filter_manager_.addFilter(filter); }

void ConnectionImpl::addReadFilter(ReadFilterPtr filter) { filter_manager_.addReadFilter(filter); }

bool ConnectionImpl::initializeReadFilters() { return filter_manager_.initializeReadFilters(); }

void ConnectionImpl::close(ConnectionCloseType type) {
  if (fd_ == -1) {
    return;
  }

  uint64_t data_to_write = write_buffer_.length();
  conn_log_debug("closing data_to_write={} type={}", *this, data_to_write, enumToInt(type));
  if (data_to_write == 0 || type == ConnectionCloseType::NoFlush) {
    if (data_to_write > 0) {
      // We aren't going to wait to flush, but try to write as much as we can if there is pending
      // data.
      doWriteToSocket();
    }

    closeSocket(ConnectionEvent::LocalClose);
  } else {
    ASSERT(type == ConnectionCloseType::FlushWrite);
    state_ |= InternalState::CloseWithFlush;
    state_ &= ~InternalState::ReadEnabled;
  }
}

Connection::State ConnectionImpl::state() {
  if (fd_ == -1) {
    return State::Closed;
  } else if (state_ & InternalState::CloseWithFlush) {
    return State::Closing;
  } else {
    return State::Open;
  }
}

void ConnectionImpl::closeSocket(uint32_t close_type) {
  if (fd_ == -1) {
    return;
  }

  conn_log_debug("closing socket: {}", *this, close_type);

  // Drain input and output buffers.
  updateReadBufferStats(0, 0);
  updateWriteBufferStats(0, 0);
  buffer_stats_.reset();

  file_event_.reset();
  ::close(fd_);
  fd_ = -1;

  raiseEvents(close_type);
}

Event::Dispatcher& ConnectionImpl::dispatcher() { return dispatcher_; }

void ConnectionImpl::noDelay(bool enable) {
  // There are cases where a connection to localhost can immediately fail (e.g., if the other end
  // does not have enough fds, reaches a backlog limit, etc.). Because we run with deferred error
  // events, the calling code may not yet know that the connection has failed. This is one call
  // where we go outside of libevent and hit the fd directly and this case can fail if the fd is
  // invalid. For this call instead of plumbing through logic that will immediately indicate that a
  // connect failed, we will just ignore the noDelay() call if the socket is invalid since error is
  // going to be raised shortly anyway and it makes the calling code simpler.
  if (fd_ == -1) {
    return;
  }

  // Don't set NODELAY for unix domain sockets
  sockaddr addr;
  socklen_t len = sizeof(addr);
  int rc = getsockname(fd_, &addr, &len);
  RELEASE_ASSERT(rc == 0);

  if (addr.sa_family == AF_UNIX) {
    return;
  }

  // Set NODELAY
  int new_value = enable;
  rc = setsockopt(fd_, IPPROTO_TCP, TCP_NODELAY, &new_value, sizeof(new_value));
  RELEASE_ASSERT(0 == rc);
  UNREFERENCED_PARAMETER(rc);
}

uint64_t ConnectionImpl::id() { return id_; }

void ConnectionImpl::onRead(uint64_t read_buffer_size) {
  if (!(state_ & InternalState::ReadEnabled)) {
    return;
  }

  if (read_buffer_size == 0) {
    return;
  }

  filter_manager_.onRead();
}

void ConnectionImpl::readDisable(bool disable) {
  bool read_enabled = readEnabled();
  UNREFERENCED_PARAMETER(read_enabled);
  conn_log_trace("readDisable: enabled={} disable={}", *this, read_enabled, disable);

  // We do not actually disable reading from the socket. We just stop firing read callbacks.
  // This allows us to still detect remote close in a timely manner. In practice there is a chance
  // that a bad client could send us a large amount of data on a HTTP/1.1 connection while we are
  // processing the current request.
  // TODO: Add buffered data stats and potentially fail safe processing that disconnects or
  //       applies back pressure to bad HTTP/1.1 clients.
  if (disable) {
    ASSERT(read_enabled);
    state_ &= ~InternalState::ReadEnabled;
  } else {
    ASSERT(!read_enabled);
    state_ |= InternalState::ReadEnabled;
    if (read_buffer_.length() > 0) {
      file_event_->activate(Event::FileReadyType::Read);
    }
  }
}

void ConnectionImpl::raiseEvents(uint32_t events) {
  for (ConnectionCallbacks* callback : callbacks_) {
    // TODO: If we close while raising a connected event we should not raise further connected
    //       events.
    callback->onEvent(events);
  }
}

bool ConnectionImpl::readEnabled() { return state_ & InternalState::ReadEnabled; }

void ConnectionImpl::addConnectionCallbacks(ConnectionCallbacks& cb) { callbacks_.push_back(&cb); }

void ConnectionImpl::write(Buffer::Instance& data) {
  // NOTE: This is kind of a hack, but currently we don't support restart/continue on the write
  //       path, so we just pass around the buffer passed to us in this function. If we ever support
  //       buffer/restart/continue on the write path this needs to get more complicated.
  current_write_buffer_ = &data;
  FilterStatus status = filter_manager_.onWrite();
  current_write_buffer_ = nullptr;

  if (FilterStatus::StopIteration == status) {
    return;
  }

  if (data.length() > 0) {
    conn_log_trace("writing {} bytes", *this, data.length());
    write_buffer_.move(data);
    if (!(state_ & InternalState::Connecting)) {
      file_event_->activate(Event::FileReadyType::Write);
    }
  }
}

void ConnectionImpl::onFileEvent(uint32_t events) {
  conn_log_trace("socket event: {}", *this, events);

  if (state_ & InternalState::ImmediateConnectionError) {
    conn_log_debug("raising immediate connect error", *this);
    closeSocket(ConnectionEvent::RemoteClose);
    return;
  }

  // Read may become ready if there is an error connecting. If still connecting, skip straight
  // to write ready which is where the connection logic is.
  if (!(state_ & InternalState::Connecting) && (events & Event::FileReadyType::Read)) {
    onReadReady();
  }

  // Possible for a read event close the socket.
  if (fd_ != -1 && (events & Event::FileReadyType::Write)) {
    onWriteReady();
  }
}

ConnectionImpl::IoResult ConnectionImpl::doReadFromSocket() {
  PostIoAction action;
  uint64_t bytes_read = 0;
  do {
    // 16K read is arbitrary. IIRC, libevent will currently clamp this to 4K. libevent will also
    // use an ioctl() before every read to figure out how much data there is to read.
    // TODO PERF: Tune the read size and figure out a way of getting rid of the ioctl(). The extra
    //            syscall is not worth it.
    int rc = read_buffer_.read(fd_, 16384);
    conn_log_trace("read returns: {}", *this, rc);

    // Remote close. Might need to raise data before raising close.
    if (rc == 0) {
      action = PostIoAction::Close;
      break;
    } else if (rc == -1) {
      // Remote error (might be no data).
      conn_log_trace("read error: {}", *this, errno);
      if (errno == EAGAIN) {
        action = PostIoAction::KeepOpen;
      } else {
        action = PostIoAction::Close;
      }

      break;
    } else {
      bytes_read += rc;
    }
  } while (true);

  return {action, bytes_read};
}

void ConnectionImpl::onReadReady() {
  ASSERT(!(state_ & InternalState::Connecting));

  IoResult result = doReadFromSocket();
  uint64_t new_buffer_size = read_buffer_.length();
  updateReadBufferStats(result.bytes_processed_, new_buffer_size);
  onRead(new_buffer_size);

  // The read callback may have already closed the connection.
  if (result.action_ == PostIoAction::Close) {
    conn_log_debug("remote close", *this);
    closeSocket(ConnectionEvent::RemoteClose);
  }
}

ConnectionImpl::IoResult ConnectionImpl::doWriteToSocket() {
  PostIoAction action;
  uint64_t bytes_written = 0;
  do {
    if (write_buffer_.length() == 0) {
      action = PostIoAction::KeepOpen;
      break;
    }

    int rc = write_buffer_.write(fd_);
    conn_log_trace("write returns: {}", *this, rc);
    if (rc == -1) {
      conn_log_trace("write error: {}", *this, errno);
      if (errno == EAGAIN) {
        action = PostIoAction::KeepOpen;
      } else {
        action = PostIoAction::Close;
      }

      break;
    } else {
      bytes_written += rc;
    }
  } while (true);

  return {action, bytes_written};
}

void ConnectionImpl::onConnected() { raiseEvents(ConnectionEvent::Connected); }

void ConnectionImpl::onWriteReady() {
  conn_log_trace("write ready", *this);

  if (state_ & InternalState::Connecting) {
    int error;
    socklen_t error_size = sizeof(error);
    int rc = getsockopt(fd_, SOL_SOCKET, SO_ERROR, &error, &error_size);
    ASSERT(0 == rc);
    UNREFERENCED_PARAMETER(rc);

    if (error == 0) {
      conn_log_debug("connected", *this);
      state_ &= ~InternalState::Connecting;
      onConnected();
    } else {
      conn_log_debug("delayed connection error: {}", *this, error);
      closeSocket(ConnectionEvent::RemoteClose);
      return;
    }
  }

  IoResult result = doWriteToSocket();
  uint64_t new_buffer_size = write_buffer_.length();
  updateWriteBufferStats(result.bytes_processed_, new_buffer_size);

  if (result.action_ == PostIoAction::Close) {
    // It is possible (though unlikely) for the connection to have already been closed during the
    // write callback. This can happen if we manage to complete the SSL handshake in the write
    // callback, raise a connected event, and close the connection.
    closeSocket(ConnectionEvent::RemoteClose);
  } else if ((state_ & InternalState::CloseWithFlush) && new_buffer_size == 0) {
    conn_log_debug("write flush complete", *this);
    closeSocket(ConnectionEvent::LocalClose);
  }
}

void ConnectionImpl::doConnect(const sockaddr* addr, socklen_t addrlen) {
  int rc = ::connect(fd_, addr, addrlen);
  if (rc == 0) {
    // write will become ready.
    state_ |= InternalState::Connecting;
  } else {
    ASSERT(rc == -1);
    if (errno == EINPROGRESS) {
      state_ |= InternalState::Connecting;
      conn_log_debug("connection in progress", *this);
    } else {
      // read/write will become ready.
      state_ |= InternalState::ImmediateConnectionError;
      conn_log_debug("immediate connection error: {}", *this, errno);
    }
  }
}

void ConnectionImpl::setBufferStats(const BufferStats& stats) {
  ASSERT(!buffer_stats_);
  buffer_stats_.reset(new BufferStats(stats));
}

void ConnectionImpl::updateReadBufferStats(uint64_t num_read, uint64_t new_size) {
  if (!buffer_stats_) {
    return;
  }

  ConnectionImplUtility::updateBufferStats(num_read, new_size, last_read_buffer_size_,
                                           buffer_stats_->read_total_,
                                           buffer_stats_->read_current_);
}

void ConnectionImpl::updateWriteBufferStats(uint64_t num_written, uint64_t new_size) {
  if (!buffer_stats_) {
    return;
  }

  ConnectionImplUtility::updateBufferStats(num_written, new_size, last_write_buffer_size_,
                                           buffer_stats_->write_total_,
                                           buffer_stats_->write_current_);
}

ClientConnectionImpl::ClientConnectionImpl(Event::DispatcherImpl& dispatcher, int fd,
                                           const std::string& url)
    : ConnectionImpl(dispatcher, fd, url) {}

Network::ClientConnectionPtr ClientConnectionImpl::create(Event::DispatcherImpl& dispatcher,
                                                          const std::string& url) {
  if (url.find(Network::Utility::TCP_SCHEME) == 0) {
    return Network::ClientConnectionPtr{new Network::TcpClientConnectionImpl(dispatcher, url)};
  } else if (url.find(Network::Utility::UNIX_SCHEME) == 0) {
    return Network::ClientConnectionPtr{new Network::UdsClientConnectionImpl(dispatcher, url)};
  } else {
    throw EnvoyException(fmt::format("malformed url: {}", url));
  }
}

TcpClientConnectionImpl::TcpClientConnectionImpl(Event::DispatcherImpl& dispatcher,
                                                 const std::string& url)
    : ClientConnectionImpl(dispatcher, socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0), url) {}

void TcpClientConnectionImpl::connect() {
  AddrInfoPtr addr_info = Utility::resolveTCP(Utility::hostFromUrl(remote_address_),
                                              Utility::portFromUrl(remote_address_));
  doConnect(addr_info->ai_addr, addr_info->ai_addrlen);
}

UdsClientConnectionImpl::UdsClientConnectionImpl(Event::DispatcherImpl& dispatcher,
                                                 const std::string& url)
    : ClientConnectionImpl(dispatcher, socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0), url) {}

void UdsClientConnectionImpl::connect() {
  sockaddr_un addr = Utility::resolveUnixDomainSocket(Utility::pathFromUrl(remote_address_));
  doConnect(reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_un));
}

} // Network
