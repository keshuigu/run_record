#ifndef NET_SERVER_H
#define NET_SERVER_H

#include "workflow/WFHttpServer.h"
namespace run_record {
// process func

void ProcessEcho(WFHttpTask* serverTask);

}  // namespace run_record

#endif  // NET_SERVER_H