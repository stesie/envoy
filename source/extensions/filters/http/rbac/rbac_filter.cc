#include "extensions/filters/http/rbac/rbac_filter.h"

#include "common/http/utility.h"

#include "extensions/filters/http/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace RBACFilter {

RoleBasedAccessControlFilterConfig::RoleBasedAccessControlFilterConfig(
    const envoy::config::filter::http::rbac::v2::RBAC& proto_config,
    const std::string& stats_prefix, Stats::Scope& scope)
    : stats_(RoleBasedAccessControlFilter::generateStats(stats_prefix, scope)),
      engine_(proto_config.rules()), shadow_engine_(proto_config.shadow_rules()),
      config_(proto_config) {}

RoleBasedAccessControlFilterStats
RoleBasedAccessControlFilter::generateStats(const std::string& prefix, Stats::Scope& scope) {
  const std::string final_prefix = prefix + "rbac.";
  return {ALL_RBAC_FILTER_STATS(POOL_COUNTER_PREFIX(scope, final_prefix))};
}

const Filters::Common::RBAC::RoleBasedAccessControlEngine&
RoleBasedAccessControlFilterConfig::engine(const Router::RouteConstSharedPtr route,
                                           EnforcementMode mode) const {
  if (!route || !route->routeEntry()) {
    return engine(mode);
  }

  const std::string& name = HttpFilterNames::get().RBAC;
  const auto* entry = route->routeEntry();

  const auto* route_local =
      entry->perFilterConfigTyped<RoleBasedAccessControlRouteSpecificFilterConfig>(name)
          ?: entry->virtualHost()
                 .perFilterConfigTyped<RoleBasedAccessControlRouteSpecificFilterConfig>(name);

  if (route_local) {
    return route_local->engine(mode);
  }

  return engine(mode);
}

bool RoleBasedAccessControlFilterConfig::isEnabled(const Router::RouteConstSharedPtr route,
                                                   EnforcementMode mode) const {
  if (!route || !route->routeEntry()) {
    return isEnabled(mode);
  }

  const std::string& name = HttpFilterNames::get().RBAC;
  const auto* entry = route->routeEntry();

  const auto* route_local =
      entry->perFilterConfigTyped<RoleBasedAccessControlRouteSpecificFilterConfig>(name)
          ?: entry->virtualHost()
                 .perFilterConfigTyped<RoleBasedAccessControlRouteSpecificFilterConfig>(name);

  if (route_local) {
    return route_local->isEnabled(mode);
  }

  return isEnabled(mode);
}

RoleBasedAccessControlRouteSpecificFilterConfig::RoleBasedAccessControlRouteSpecificFilterConfig(
    const envoy::config::filter::http::rbac::v2::RBACPerRoute& per_route_config)
    : engine_(per_route_config.rbac().rules()),
      shadow_engine_(per_route_config.rbac().shadow_rules()), config_(per_route_config.rbac()) {}

Http::FilterHeadersStatus RoleBasedAccessControlFilter::decodeHeaders(Http::HeaderMap& headers,
                                                                      bool) {
  if (config_->isEnabled(callbacks_->route(), EnforcementMode::SHADOW)) {
    if (config_->engine(callbacks_->route(), EnforcementMode::SHADOW)
            .allowed(*callbacks_->connection(), headers)) {
      config_->stats().shadow_allowed_.inc();
    } else {
      config_->stats().shadow_denied_.inc();
    }
  }

  if (!config_->isEnabled(callbacks_->route(), EnforcementMode::ENFORCED) ||
      config_->engine(callbacks_->route(), EnforcementMode::ENFORCED)
          .allowed(*callbacks_->connection(), headers)) {
    config_->stats().allowed_.inc();
    return Http::FilterHeadersStatus::Continue;
  }

  callbacks_->sendLocalReply(Http::Code::Forbidden, "RBAC: access denied", nullptr);
  config_->stats().denied_.inc();
  return Http::FilterHeadersStatus::StopIteration;
}

} // namespace RBACFilter
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
