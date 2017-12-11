#include "server/config/network/ssl_socket.h"

#include "api/sds.pb.h"
#include "envoy/registry/registry.h"
#include "common/ssl/connection_impl.h"
#include "common/network/raw_buffer_socket.h"

namespace Envoy {
namespace Server {
namespace Configuration {

Network::TransportSocketFactory DownstreamSslSocketFactory::createTransportSocketFactory(const Protobuf::Message &) {
  return []() -> Network::TransportSocketPtr {
    return Network::TransportSocketPtr{new Network::RawBufferSocket};
  };
}

ProtobufTypes::MessagePtr DownstreamSslSocketFactory::createEmptyConfigProto() {
  return ProtobufTypes::MessagePtr{new envoy::api::v2::DownstreamTlsContext};
}

Network::TransportSocketFactory UpstreamSslSocketFactory::createTransportSocketFactory(const Protobuf::Message &) {
  return []() -> Network::TransportSocketPtr {
    return Network::TransportSocketPtr{new Network::RawBufferSocket};
  };
}

ProtobufTypes::MessagePtr UpstreamSslSocketFactory::createEmptyConfigProto() {
  return ProtobufTypes::MessagePtr{new envoy::api::v2::UpstreamTlsContext};
}

static Registry::RegisterFactory<DownstreamSslSocketFactory, DownstreamTransportSocketConfigFactory>
    downstream_registered_;
static Registry::RegisterFactory<UpstreamSslSocketFactory, UpstreamTransportSocketConfigFactory>
    upstream_registered_;

}
}
}