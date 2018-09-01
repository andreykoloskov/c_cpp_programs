#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fstream>

int main(int argc, char **argv) {
    int pid = fork();

    if (pid) {
        std::ofstream f("pid_parent");
        f << getpid();
        f.close();
        wait(NULL);
    } else {
        std::ofstream f("pid_child");
        f << getpid();
        f.close();

        while(true) {
            sleep(1);
        }
    }

    return 0;
}
