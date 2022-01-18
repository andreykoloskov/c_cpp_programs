/*
Реализуйте класс TeamTasks, позволяющий хранить статистику по статусам задач команды разработчиков

Метод PerformPersonTasks должен реализовывать следующий алгоритм:

Рассмотрим все невыполненные задачи разработчика person.

Упорядочим их по статусам: сначала все задачи в статусе NEW, затем все задачи в статусе IN_PROGRESS и, наконец, задачи в статусе TESTING.

Рассмотрим первые task_count задач и переведём каждую из них в следующий статус в соответствии с естественным порядком: NEW → IN_PROGRESS → TESTING → DONE.

Вернём кортеж из двух элементов: информацию о статусах обновившихся задач (включая те, которые оказались в статусе DONE) и информацию о статусах остальных не выполненных задач.

В случае отсутствия разработчика с именем person метод PerformPersonTasks должен вернуть кортеж из двух пустых TasksInfo

Гарантируется, что task_count является положительным числом. Если task_count превышает текущее количество невыполненных задач разработчика, достаточно считать, что task_count равен количеству невыполненных задач: дважды обновлять статус какой-либо задачи в этом случае не нужно.

Кроме того, гарантируется, что метод GetPersonTasksInfo не будет вызван для разработчика, не имеющего задач.

Пример работы метода PerformPersonTasks
Предположим, что у конкретного разработчика имеется 10 задач со следующими статусами:

NEW — 3

IN_PROGRESS — 2

TESTING — 4

DONE — 1

Поступает команда PerformPersonTasks с параметром task_count = 4, что означает обновление статуса для 3 задач c NEW до IN_PROGRESS и для 1 задачи с IN_PROGRESS до TESTING.
Если выписать статусы всех задач по порядку, а обновляемые заключить в квадратные скобки, то изменение можно изобразить следующим образом:

  [NEW, NEW, NEW, IN_PROGRESS,] IN_PROGRESS, TESTING, TESTING, TESTING, TESTING, DONE

  ==>

  [IN_PROGRESS, IN_PROGRESS, IN_PROGRESS, TESTING,] IN_PROGRESS, TESTING, TESTING, TESTING, TESTING, DONE

Таким образом, новые статусы задач будут следующими:

IN_PROGRESS — 3 обновлённых, 1 старая

TESTING — 1 обновлённая, 4 старых

DONE — 1 старая

В этом случае необходимо вернуть кортеж из 2 словарей:

Обновлённые задачи: IN_PROGRESS — 3, TESTING — 1.

Необновлённые задачи, исключая выполненные: IN_PROGRESS — 1, TESTING — 4.

Ни один из словарей не должен содержать лишних элементов, то есть статусов, которым соответствует ноль задач.

По условию, DONE задачи не нужно возвращать в необновлённых задачах (untouched_tasks).

Примечание
Обновление словаря одновременно с итерированием по нему может привести к непредсказуемым последствиям. При возникновении такой необходимости рекомендуется сначала в отдельном временном словаре собрать информацию об обновлениях, а затем применить их к основному словарю.
*/

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return team_statistic.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person) {
        team_tasks[person].push_back(TaskStatus::NEW);
        team_statistic[person][TaskStatus::NEW]++;
    }

    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(
            const string& person, int task_count) {
        auto& person_tasks = team_tasks[person];
        auto& person_statistic = team_statistic[person];
        vector<TaskStatus> person_tasks_new;
        int new_progress = 0, new_testing = 0, new_done = 0;
        int old_new = 0, old_progress = 0, old_testing = 0;

        sort(person_tasks.begin(), person_tasks.end());
        for (int i = 0; i < person_tasks.size(); i++) {
            if (i < task_count) {
                if (person_tasks[i] == TaskStatus::NEW) {
                    person_tasks_new.push_back(TaskStatus::IN_PROGRESS);
                    person_statistic[TaskStatus::NEW]--;
                    person_statistic[TaskStatus::IN_PROGRESS]++;
                    new_progress++;
                } else if (person_tasks[i] == TaskStatus::IN_PROGRESS) {
                    person_tasks_new.push_back(TaskStatus::TESTING);
                    person_statistic[TaskStatus::IN_PROGRESS]--;
                    person_statistic[TaskStatus::TESTING]++;
                    new_testing++;
                } else if (person_tasks[i] == TaskStatus::TESTING) {
                    person_tasks_new.push_back(TaskStatus::DONE);
                    person_statistic[TaskStatus::TESTING]--;
                    person_statistic[TaskStatus::DONE]++;
                    new_done++;
                }
            } else {
                person_tasks_new.push_back(person_tasks[i]);
                if (person_tasks[i] == TaskStatus::NEW) {
                    old_new++;
                } else if (person_tasks[i] == TaskStatus::IN_PROGRESS) {
                    old_progress++;
                } else if (person_tasks[i] == TaskStatus::TESTING) {
                    old_testing++;
                }
            }
        }

        for (auto it = person_statistic.begin(); it != person_statistic.end(); ) {
          if (it->second == 0) {
            it = person_statistic.erase(it);
          }
          else {
            ++it;
          }
        }

        team_tasks[person] = person_tasks_new;

        TasksInfo t_new;
        if (new_progress > 0)
            t_new[TaskStatus::IN_PROGRESS] = new_progress;
        if (new_testing > 0)
            t_new[TaskStatus::TESTING] = new_testing;
        if (new_done > 0)
            t_new[TaskStatus::DONE] = new_done;

        TasksInfo t_old;
        if (old_new > 0)
            t_old[TaskStatus::NEW] = old_new;
        if (old_progress > 0)
            t_old[TaskStatus::IN_PROGRESS] = old_progress;
        if (old_testing > 0)
            t_old[TaskStatus::TESTING] = old_testing;

        return make_tuple(t_new, t_old);
    };

private:
    map<string, vector<TaskStatus>> team_tasks;
    map<string, TasksInfo> team_statistic;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
         ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
         ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
         ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

int main() {
    TeamTasks tasks;
    tasks.AddNewTask("Ilia");
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan");
    }
    cout << "Ilia's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
    cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    return 0;
}
