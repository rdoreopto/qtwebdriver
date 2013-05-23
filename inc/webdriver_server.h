#ifndef WEBDRIVER_SERVER_H_
#define WEBDRIVER_SERVER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/command_line.h"

struct mg_context;
struct mg_connection;
struct mg_request_info;

namespace base {
class DictionaryValue;
}

namespace webdriver {

class RouteTable;
class Command;
class HttpResponse;
class Response;

class Server {
public:

    /// Creates a new Server with DefaultRouteTable.
    Server();
    ~Server();

    /// Init server, parse arguments as options. 
    /// @param argc count of arguments
    /// @param argv array of arguments
    /// @return 0 - if init was success, error code otherwise.
    int Init(int argc, char *argv[]);

    /// Set route table for this server. Server should be stopped.
    /// @param routeTable routeTable to set
    void SetRouteTable(RouteTable* routeTable);

    /// Start server 
    /// @return 0 - if success, error code otherwise.
    int Start();

private:
    CommandLine options_;
    RouteTable* routeTable_;
    std::vector<std::string> mg_options_;
    std::string url_base_;
    struct mg_context* mg_ctx_;

    static void* ProcessHttpRequestCb(int event_raised,
                              struct mg_connection* connection,
                              const struct mg_request_info* request_info);

    // Converts a |Response| into a |HttpResponse| to be returned to the client.
    // This function is exposed for testing.
    void PrepareHttpResponse(const Response& command_response,
                             HttpResponse* const http_response);

    void DispatchCommand(Command* const command,
                         const std::string& method,
                         Response* response);

    // Allows the bulk of the implementation of |Dispatch| to be moved out of this
    // header file. Takes ownership of |command|.
    void DispatchHelper(Command* command_ptr,
                        const std::string& method,
                        Response* response);

    bool ProcessHttpRequest(struct mg_connection* connection,
                        const struct mg_request_info* request_info);

    bool ParseRequestInfo(const struct mg_request_info* const request_info,
                      struct mg_connection* const connection,
                      std::string* method,
                      std::vector<std::string>* path_segments,
                      base::DictionaryValue** parameters,
                      Response* const response);

    void ReadRequestBody(const struct mg_request_info* const request_info,
                     struct mg_connection* const connection,
                     std::string* request_body);

    void SendNoContentResponse(struct mg_connection* connection,
                           const struct mg_request_info* request_info);
 
    void SendResponse(struct mg_connection* const connection,
                  const std::string& request_method,
                  const Response& response);

    void WriteHttpResponse(struct mg_connection* connection,
                       const HttpResponse& response);

    int InitMongooseOptions();

    int InitLogging();

#if !defined(OS_WIN)
    int ParseConfigToOptions();
#endif


    DISALLOW_COPY_AND_ASSIGN(Server);
};


}  // namespace webdriver

#endif  // WEBDRIVER_ROUTE_TABLE_H_