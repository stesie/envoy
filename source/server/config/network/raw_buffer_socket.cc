#include "server/config/network/raw_buffer_socket.h"

#include "envoy/registry/registry.h"
#include "common/network/raw_buffer_socket.h"

namespace Envoy {
namespace Server {
namespace Configuration {

Network::TransportSocketFactory RawBufferSocketFactory::createTransportSocketFactory(const Protobuf::Message &) {
  return []() -> Network::TransportSocketPtr {
    return Network::TransportSocketPtr{new Network::RawBufferSocket};
  };
}

ProtobufTypes::MessagePtr RawBufferSocketFactory::createEmptyConfigProto() {
  return ProtobufTypes::MessagePtr{new ProtobufWkt::Empty};
}

static Registry::RegisterFactory<DownstreamRawBufferSocketFactory, DownstreamTransportSocketConfigFactory>
    downstream_registered_;

static Registry::RegisterFactory<UpstreamRawBufferSocketFactory, UpstreamTransportSocketConfigFactory>
    upstream_registered_;

}
}
}
