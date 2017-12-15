#include "server/config/network/ssl_socket.h"

#include "envoy/registry/registry.h"

#include "common/network/raw_buffer_socket.h"
#include "common/protobuf/utility.h"
#include "common/ssl/context_config_impl.h"
#include "common/ssl/ssl_socket.h"

#include "api/sds.pb.h"
#include "api/sds.pb.validate.h"

namespace Envoy {
namespace Server {
namespace Configuration {
/*
Network::TransportSocketFactoryPtr DownstreamSslSocketFactory::createTransportSocketFactory(const
Protobuf::Message &) { return Network::TransportSocketFactoryPtr{new RawBufferSocketFactory};
}

ProtobufTypes::MessagePtr DownstreamSslSocketFactory::createEmptyConfigProto() {
  return ProtobufTypes::MessagePtr{new envoy::api::v2::DownstreamTlsContext};
}

static Registry::RegisterFactory<DownstreamSslSocketFactory, DownstreamTransportSocketConfigFactory>
    downstream_registered_;
*/
Network::TransportSocketFactoryPtr UpstreamSslSocketFactory::createTransportSocketFactory(
    const Protobuf::Message& message, Network::TransportSocketFactoryContext& context) {
  return Network::TransportSocketFactoryPtr{new Ssl::ClientSslSocketFactory(
      Ssl::ClientContextConfigImpl(
          MessageUtil::downcastAndValidate<const envoy::api::v2::UpstreamTlsContext&>(message)),
      context)};
}

ProtobufTypes::MessagePtr UpstreamSslSocketFactory::createEmptyConfigProto() {
  return ProtobufTypes::MessagePtr{new envoy::api::v2::UpstreamTlsContext};
}

static Registry::RegisterFactory<UpstreamSslSocketFactory, UpstreamTransportSocketConfigFactory>
    upstream_registered_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy
