#include "net/server.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <string>

#include "workflow/HttpMessage.h"
#include "workflow/HttpUtil.h"
#include "workflow/WFFacilities.h"
#include "workflow/WFHttpServer.h"
#include "workflow/WFServer.h"
namespace run_record {

// process func

void ProcessEcho(WFHttpTask* serverTask) {
  constexpr int kBufferSize = 8192;
  constexpr int kAddSize = 128;
  protocol::HttpRequest* req = serverTask->get_req();
  protocol::HttpResponse* resp = serverTask->get_resp();
  long long seq = serverTask->get_task_seq();
  protocol::HttpHeaderCursor cursor(req);
  std::string name;
  std::string value;
  char buf[kBufferSize];
  int len;

  resp->append_output_body_nocopy("<html>", 6);
  len = snprintf(buf, kBufferSize, "<p>%s %s %s</p>", req->get_method(),
                 req->get_request_uri(), req->get_http_version());
  resp->append_output_body(buf, len);
  // 获取头部
  while (cursor.next(name, value)) {
    len = snprintf(buf, kBufferSize, "<p>%s: %s</p>", name.c_str(),
                   value.c_str());
    resp->append_output_body(buf, len);
  }
  resp->append_output_body_nocopy("</html>", 7);

  resp->set_http_version("HTTP/1.1");
  resp->set_status_code("200");
  resp->set_reason_phrase("OK");

  resp->add_header_pair("Content-Type", "text/html");
  resp->add_header_pair("Server", "Sogou WFHttpServer");

  // 一个链接限制最多10访问
  if (seq == 9) {
    resp->add_header_pair("Connection", "close");
  }

  // log
  char addrstr[kAddSize];
  sockaddr_storage addr;
  socklen_t l = sizeof(addr);
  unsigned short port = 0;
  serverTask->get_peer_addr((sockaddr*)&addr, &l);
  if (addr.ss_family == AF_INET) {
    sockaddr_in* sin = (sockaddr_in*)&addr;
    inet_ntop(AF_INET, &sin->sin_addr, addrstr, kAddSize);
    port = ntohs(sin->sin_port);
  } else if (addr.ss_family == AF_INET6) {
    struct sockaddr_in6* sin6 = (struct sockaddr_in6*)&addr;
    inet_ntop(AF_INET6, &sin6->sin6_addr, addrstr, kAddSize);
    port = ntohs(sin6->sin6_port);
  } else {
    strcpy(addrstr, "Unknown");
  }

  fprintf(stderr, "Peer address: %s:%d, seq: %lld.\n", addrstr, port, seq);
}

}  // namespace run_record