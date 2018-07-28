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
