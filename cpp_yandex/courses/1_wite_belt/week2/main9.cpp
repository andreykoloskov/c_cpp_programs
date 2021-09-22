/*
Ћюди сто€т в очереди, но никогда не уход€т из еЄ начала, зато могут приходить в конец и уходить оттуда. Ѕолее того, иногда некоторые люди могут прекращать и начинать беспокоитьс€ из-за того, что очередь не продвигаетс€.

Ѕудем считать, что люди в очереди нумеруютс€ целыми числами, начина€ с 0.

–еализуйте обработку следующих операций над очередью:

WORRY i: пометить i-го человека с начала очереди как беспоко€щегос€;

QUIET i: пометить i-го человека как успокоившегос€;

COME k: добавить k спокойных человек в конец очереди;

COME -k: убрать k человек из конца очереди;

WORRY_COUNT: узнать количество беспоко€щихс€ людей в очереди.

»значально очередь пуста.

‘ормат ввода
 оличество операций Q, затем описани€ операций.

ƒл€ каждой операции WORRY i и QUIET i гарантируетс€, что человек с номером i существует в очереди на момент операции.

ƒл€ каждой операции COME -k гарантируетс€, что k не больше текущего размера очереди.

‘ормат вывода
ƒл€ каждой операции WORRY_COUNT выведите одно целое число Ч количество беспоко€щихс€ людей в очереди.

ѕример

¬вод
8
COME 5
WORRY 1
WORRY 4
COME -2
WORRY_COUNT
COME 3
WORRY 3
WORRY_COUNT

¬ывод
1
2
*/

#include <iostream>
#include <vector>
#include <string>

using namespace std;

void
come(vector<int>& que, int k)
{
	if (k > 0) {
		for (int i = 0; i < k; i++)
			que.push_back(0);
	} else if (k < 0) {
		for (int i = k; i < 0; i++)
			que.pop_back();
	}
}

void
worry(vector<int>& que, int k)
{
	que[k] = 1;
}

void
quiet(vector<int>& que, int k)
{
	que[k] = 0;
}

void worry_count(vector<int>& que)
{
	int cnt = 0;
	for (auto w : que)
		if (w == 1)
			cnt++;
	cout << cnt << endl;
}

int
main()
{
	int q;
	cin >> q;
	vector<int> que;
	for (int i = 0; i < q; i++) {
		string op;
		int k;
		cin >> op;
		if (op == "COME") {
			cin >> k;
			come(que, k);
		} else if (op == "WORRY") {
			cin >> k;
			worry(que, k);
		} else if (op == "QUIET") {
			cin >> k;
			quiet(que, k);
		} else if (op == "WORRY_COUNT") {
			worry_count(que);
		}
	}

	return 0;
}
