#include <iostream>
#include <thread>

#include "lock.h"

template<Common::Lock::LN NAME>
void ThreadFunc(int id, int& counter)
{
	for (int i = 0; i < 5; ++i)
	{
		Common::CSLock<NAME> lock(true);
		std::wcout << L"id:" << id << L", counter:" << counter << L", lock:" << lock.Name() << std::endl;
		++counter;
	}
}

int main()
{
	int counter1 = 0;
	int counter2 = 0;

	// ALLOCATOR_LOCK test
	std::thread th1(ThreadFunc<Common::Lock::ALLOCATOR_LOCK>, 0, std::ref(counter1));
	std::thread th2(ThreadFunc<Common::Lock::ALLOCATOR_LOCK>, 1, std::ref(counter1));

	th1.join();
	th2.join();

	// USER_LOCK test
	std::thread th3(ThreadFunc<Common::Lock::USER_LOCK>, 0, std::ref(counter2));
	std::thread th4(ThreadFunc<Common::Lock::USER_LOCK>, 1, std::ref(counter2));

	th3.join();
	th4.join();

    return 0;
}