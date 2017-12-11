#pragma once

#include "envoy/server/transport_socket_config.h"

#include "common/config/well_known_names.h"

namespace Envoy {
namespace Server {
namespace Configuration {

/**
 * Config registration for the tcp proxy filter. @see NamedNetworkFilterConfigFactory.
 */
class SslSocketFactory : public virtual TransportSocketConfigFactory {
 public:
  virtual ~SslSocketFactory() {}
  std::string name() const override { return Config::TransportSocketNames::get().BORINGSSL; }
};

class DownstreamSslSocketFactory : public DownstreamTransportSocketConfigFactory,
                                   public SslSocketFactory {
 public:
  Network::TransportSocketFactory createTransportSocketFactory(const Protobuf::Message &config) override;
  ProtobufTypes::MessagePtr createEmptyConfigProto() override;
};

class UpstreamSslSocketFactory : public UpstreamTransportSocketConfigFactory,
                                 public SslSocketFactory {
 public:
  Network::TransportSocketFactory createTransportSocketFactory(const Protobuf::Message &config) override;
  ProtobufTypes::MessagePtr createEmptyConfigProto() override;
};

} // namespace Configuration
} // namespace Server
} // namespace Envoy
