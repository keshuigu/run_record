#include <iostream>
#include <signal.h>
#include "net/server.h"
#include "workflow/WFFacilities.h"
#include "workflow/WFHttpServer.h"

static WFFacilities::WaitGroup wait_group(1);

void sig_handler(int signo) { wait_group.done(); }

int main(int argc, char *argv[]) {
  unsigned short port;

  if (argc != 2) {
    fprintf(stderr, "USAGE: %s <port>\n", argv[0]);
    exit(1);
  }
  signal(SIGINT, sig_handler);
  WFHttpServer server(run_record::ProcessEcho);
	port = atoi(argv[1]);
	if (server.start(port) == 0)
	{
		wait_group.wait();
		server.stop();
	}
	else
	{
		perror("Cannot start server");
		exit(1);
	}

	return 0;
}