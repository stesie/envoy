#include "common/grpc/transcoder_input_stream_impl.h"

namespace Envoy {
namespace Grpc {

int64_t TranscoderInputStreamImpl::BytesAvailable() const { return buffer_->length() - position_; }

bool TranscoderInputStreamImpl::Finished() const { return finished_; }

} // namespace Grpc
} // namespace Envoy
