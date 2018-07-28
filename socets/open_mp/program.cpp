#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <omp.h>

#define LEN1 1024
#define LEN2 32

struct Sys
{
    char name[LEN2];
    int count;
};

int main(int argc, char **argv)
{
    if (argc < 2)
        return 0;

    char str[LEN1];
    int fd[2];
    pipe(fd);
    int count = 0;
    struct Sys sys[LEN1];
    char *ch;
    int i;
    int num_threads = count;
    int flag;

    for (i = 0; i < LEN1; i++) {
        sys[i].name[0] = '\0';
        sys[i].count = 0;
    }

    if (!fork()) {
        dup2(fd[1], 2);
        close(fd[0]);
        close(fd[1]);
        execlp("strace", "strace", argv[1], NULL);
        //execvp("strace", argv + 2);
    }

    close(fd[1]);
    FILE *fl= fdopen(fd[0], "r");

    while (fgets(str, LEN1, fl)) {
        if (!(ch = strchr(str, '(')))
            continue;
        *ch = '\0';
        flag = 0;

        omp_set_num_threads(8);

        #pragma omp parallel for private(i)   
        for (i = 0; i < count; i++) {
            if (!strcmp(str, sys[i].name)) {
                sys[i].count++;
                flag = 1;
            }
        }

        if (!flag) {
            strcpy(sys[count].name, str);
            sys[count].count++;
            count++;
        }
    }

    printf("\n");
    for (i = 0; i < count; i++)
        printf("%s - %d\n", sys[i].name, sys[i].count);

    fclose(fl);

    return 0;
} 
