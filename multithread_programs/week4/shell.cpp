/*
Задача на безымянные каналы.

Пусть у нас есть длинная команда вида:

who | sort | uniq -c | sort -nk1
Надо ее прочитать из STDIN, выполнить и STDOUT записать в файл /home/box/result.out

Вы должны создать Makefile в домашнем каталоге.

Тестовая система выполнит make. Она ожидает, что появится файл исполняемый файл - /home/box/shell. После чего она исполнит его несколько раз, подавая на STDIN разные строчки и проверяя result.out.

Пожалуйста, ВЕЗДЕ используйте абсолютные пути!

P.S. Существует читерский вариант:

#!/bin/bash
bash > /home/box/result.out
*/

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    std::string str;
    std::getline(std::cin, str);
    char *str_commands = (char *) str.c_str();
    std::vector<char *> commands;

    int is_new_command = 1;
    for (int i = 0, j = 0; i < str.size(); i++) {
        if (is_new_command) {
            commands.push_back(&str_commands[i]);
            is_new_command = 0;
        }

        if (str_commands[i] == '|') {
            str_commands[i] = '\0';
            is_new_command = 1;
        }
    }

    int fd0[2], fd1[2];
    pipe(fd0);
    std::cout << "create: " << fd0[0] << " : " << fd0[1] << std::endl;
    for (int i = 0; i < commands.size(); i++) {
        std::cout << "step: " << i << std::endl;
        if (i % 2) {
            pipe(fd0);
            std::cout << "create: " << fd0[0] << ":" << fd0[1] << std::endl;
        } else {
            pipe(fd1);
            std::cout << "create: " << fd1[0] << ":" << fd1[1] << std::endl;
        }

        if (!fork()) {
            if (i > 0) {
                std::cout << commands[i] << " in: " << ((i % 2) ? fd1[0] : fd0[0]) << std::endl;
                close(STDIN_FILENO);
                dup2((i % 2) ? fd1[0] : fd0[0], STDIN_FILENO);
            }
            if (i < commands.size() - 1) {
                std::cout << commands[i] << " out: " << ((i % 2) ? fd0[1] : fd1[1]) << std::endl;
                close(STDOUT_FILENO);
                dup2((i % 2) ? fd0[1] : fd1[1], STDOUT_FILENO);
            }
            close(fd0[0]);
            close(fd0[1]);
            close(fd1[0]);
            close(fd1[1]);
            execlp(commands[i], commands[i], NULL);
        } else {
            if (i % 2) {
                std::cout << "close: " << fd1[0] << ":" << fd1[1] << ":" << fd0[1] << std::endl;
                close(fd1[0]);
                close(fd1[1]);
                close(fd0[1]);
            } else {
                std::cout << "close: " << fd0[0] << ":" << fd0[1] << ":" << fd1[1] << std::endl;
                close(fd0[0]);
                close(fd0[1]);
                close(fd1[1]);
            }
            wait(NULL);
        }
    }

    return 0;
}
