#include <bits/stdc++.h>

using namespace std;

int FindDuplicate(vector<int> &nums)
{
	int tortoise = nums[0];
	int hare = nums[0];

	do
	{
		tortoise = nums[(tortoise) % (nums.size())];
		hare = nums[(nums[hare]) % (nums.size())];

	} while (tortoise != hare);

	tortoise = nums[0];
	while (tortoise != hare)
	{
		tortoise = nums[(tortoise) % (nums.size())];
		hare = nums[(hare) % (nums.size())];
	}
	return hare;
}

int main()
{
	vector<int> array(4);
	for (auto &i : array)
		cin >> i;
	cout << FindDuplicate(array);
	return 0;
}