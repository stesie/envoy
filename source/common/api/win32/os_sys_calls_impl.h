#pragma once

#include "envoy/api/os_sys_calls.h"

#include "common/singleton/threadsafe_singleton.h"

namespace Envoy {
namespace Api {

class OsSysCallsImpl : public OsSysCalls {
public:
  // Api::OsSysCalls
  SysCallIntResult bind(SOCKET_FD sockfd, const sockaddr* addr, socklen_t addrlen) override;
  SysCallIntResult ioctl(SOCKET_FD sockfd, unsigned long int request, void* argp) override;
  SysCallSizeResult writev(SOCKET_FD fd, IOVEC* iovec, int num_iovec) override;
  SysCallSizeResult readv(SOCKET_FD fd, IOVEC* iovec, int num_iovec) override;
  SysCallSizeResult recv(SOCKET_FD socket, void* buffer, size_t length, int flags) override;
  SysCallSizeResult recvfrom(SOCKET_FD sockfd, void* buffer, size_t length, int flags,
                             struct sockaddr* addr, socklen_t* addrlen) override;
  SysCallSizeResult recvmsg(SOCKET_FD sockfd, LPWSAMSG msg, int flags) override;
  SysCallIntResult close(SOCKET_FD fd) override;
  SysCallIntResult ftruncate(int fd, off_t length) override;
  SysCallPtrResult mmap(void* addr, size_t length, int prot, int flags, int fd,
                        off_t offset) override;
  SysCallIntResult stat(const char* pathname, struct stat* buf) override;
  SysCallIntResult setsockopt(SOCKET_FD sockfd, int level, int optname, const void* optval,
                              socklen_t optlen) override;
  SysCallIntResult getsockopt(SOCKET_FD sockfd, int level, int optname, void* optval,
                              socklen_t* optlen) override;
  SysCallSocketResult socket(int domain, int type, int protocol) override;
  SysCallSizeResult sendto(SOCKET_FD fd, const void* buffer, size_t size, int flags, const sockaddr* addr,
                           socklen_t addrlen) override;
  SysCallSizeResult sendmsg(SOCKET_FD fd, const LPWSAMSG message, int flags) override;
  SysCallIntResult getsockname(SOCKET_FD sockfd, sockaddr* name, socklen_t* namelen) override;

  // TODO: Pivotal review- the following functions don't exist in master
  SysCallIntResult getpeername(SOCKET_FD sockfd, sockaddr* name, socklen_t* namelen) override;
  SysCallIntResult setSocketNonBlocking(SOCKET_FD sockfd) override;
  SysCallIntResult setSocketBlocking(SOCKET_FD sockfd) override;
  SysCallIntResult shutdown(SOCKET_FD sockfd, int how) override;
  SysCallIntResult listen(SOCKET_FD sockfd, int backlog) override;
  SysCallIntResult socketpair(int domain, int type, int protocol, SOCKET_FD sv[2]) override;
  SysCallSocketResult accept(SOCKET_FD sockfd, sockaddr* addr, socklen_t* addr_len) override;
  SysCallIntResult connect(SOCKET_FD sockfd, const sockaddr* addr, socklen_t addrlen) override;
  SysCallSizeResult writeSocket(SOCKET_FD fd, const void* buffer, size_t num_bytes) override;
};

typedef ThreadSafeSingleton<OsSysCallsImpl> OsSysCallsSingleton;

} // namespace Api
} // namespace Envoy