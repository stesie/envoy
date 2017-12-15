#pragma once

#include <string>

#include "envoy/network/transport_socket.h"

#include "common/protobuf/protobuf.h"

namespace Envoy {
namespace Server {
namespace Configuration {

class TransportSocketConfigFactory {
public:
  virtual ~TransportSocketConfigFactory() {}

  virtual Network::TransportSocketFactoryPtr
  createTransportSocketFactory(const Protobuf::Message& config,
                               Network::TransportSocketFactoryContext& context) PURE;

  virtual ProtobufTypes::MessagePtr createEmptyConfigProto() PURE;

  /**
   * @return std::string the identifying name for a particular TransportPtr implementation
   * produced by the factory.
   */
  virtual std::string name() const PURE;
};

class UpstreamTransportSocketConfigFactory : public virtual TransportSocketConfigFactory {};

} // namespace Configuration
} // namespace Server
} // namespace Envoy
