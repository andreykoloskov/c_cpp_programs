#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <fstream>

int main(int argc, char **argv) {
    std::ofstream f("pid");
    f << getpid();
    f.close();

    signal(SIGTERM, SIG_IGN);
    signal(SIGINT, SIG_IGN);

    while(true) {
        sleep(1);
    }

    return 0;
}