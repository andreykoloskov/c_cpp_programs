/*
Задача на безымянные каналы.

Пусть у нас есть длинная команда вида:

who | sort | uniq -c | sort -nk1
Надо ее прочитать из STDIN,
выполнить и STDOUT записать в файл /home/box/result.out

Вы должны создать Makefile в домашнем каталоге.

Тестовая система выполнит make. Она ожидает, что появится файл исполняемый
файл - /home/box/shell. После чего она исполнит его несколько раз,
подавая на STDIN разные строчки и проверяя result.out.

Пожалуйста, ВЕЗДЕ используйте абсолютные пути!

P.S. Существует читерский вариант:

#!/bin/bash
bash > /home/box/result.out
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <boost/algorithm/string.hpp>

int main(int argc, char **argv) {
    std::string str;
    std::getline(std::cin, str);
    std::vector<std::string> commands;
    boost::split(commands, str, boost::is_any_of("|"));
    std::for_each(commands.begin(), commands.end(),
            [](std::string &st) { boost::algorithm::trim(st); });

    int fd0[2], fd1[2];
    pipe(fd0);
    for (int i = 0; i < commands.size(); i++) {
        std::cout << "step: " << i << " : " << commands[i] << std::endl;
        if (i % 2) {
            pipe(fd0);
        } else {
            pipe(fd1);
        }

        if (!fork()) {
            if (i > 0) {
                close(STDIN_FILENO);
                dup2((i % 2) ? fd1[0] : fd0[0], STDIN_FILENO);
            }
            if (i < commands.size() - 1) {
                close(STDOUT_FILENO);
                dup2((i % 2) ? fd0[1] : fd1[1], STDOUT_FILENO);
            }
            close(fd0[0]);
            close(fd0[1]);
            close(fd1[0]);
            close(fd1[1]);
            execlp(commands[i].c_str(), commands[i].c_str(), NULL);
        } else {
            if (i % 2) {
                close(fd1[0]);
                close(fd1[1]);
                close(fd0[1]);
            } else {
                close(fd0[0]);
                close(fd0[1]);
                close(fd1[1]);
            }
            wait(NULL);
        }
    }

    return 0;
}
