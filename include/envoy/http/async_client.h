#pragma once

#include "envoy/common/optional.h"
#include "envoy/http/message.h"

namespace Http {

/**
 * Supports sending an HTTP request message and receiving a response asynchronously.
 */
class AsyncClient {
public:
  /**
   * Async Client failure reasons.
   */
  enum class FailureReason {
    // The stream has been reset.
    Reset
  };

  /**
   * Notifies caller of async HTTP request status.
   */
  class Callbacks {
  public:
    virtual ~Callbacks() {}

    /**
     * Called when the async HTTP request succeeds.
     * @param response the HTTP response
     */
    virtual void onSuccess(MessagePtr&& response) PURE;

    /**
     * Called when the async HTTP request fails.
     */
    virtual void onFailure(FailureReason reason) PURE;
  };

  class StreamingCallbacks {
   public:
    virtual ~StreamingCallbacks() {}

    virtual void onHeaders(HeaderMapPtr&& headers, bool end_stream) PURE;
    virtual void onData(Buffer::Instance& data, bool end_stream) PURE;
    virtual void onTrailers(HeaderMapPtr&& trailers) PURE;
    virtual void onResetStream() PURE;
  };

  /**
   * An in-flight HTTP request
   */
  class Request {
   public:
    virtual ~Request() {}

    /**
     * Signals that the request should be cancelled.
     */
    virtual void cancel() PURE;
  };

  class StreamingRequest {
   public:
    virtual ~StreamingRequest() {}
    virtual void sendHeaders(HeaderMap& headers, bool end_stream) PURE;
    virtual void sendData(Buffer::Instance& data, bool end_stream) PURE;
    virtual void sendTrailers(HeaderMap& trailers) PURE;
    virtual void sendResetStream() PURE;
  };

  virtual ~AsyncClient() {}

  /**
   * Send an HTTP request asynchronously
   * @param request the request to send.
   * @param callbacks the callbacks to be notified of request status.
   * @return a request handle or nullptr if no request could be created. NOTE: In this case
   *         onFailure() has already been called inline. The client owns the request and the
   *         handle should just be used to cancel.
   */
  virtual Request* send(MessagePtr&& request, Callbacks& callbacks,
                        const Optional<std::chrono::milliseconds>& timeout) PURE;
};

typedef std::unique_ptr<AsyncClient> AsyncClientPtr;

} // Http
