// Race conditions example
// Adam Sampson <a.sampson@abertay.ac.uk>

#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <chrono>	
#include <mutex>
#include "account.h"

typedef std::chrono::steady_clock the_clock;

// Import things we need from the standard library
using std::cout;
using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::this_thread::sleep_for;
using std::chrono::seconds;
using std::condition_variable;

Account bill;
std::mutex bill_mutex;
using std::condition_variable;
condition_variable cv;
bool result_ready = false;

void callAdd() {

	for (int i = 0; i < 10000000; i++) { //this number needs to be large enough for other threads to overwrite this
		sleep_for(milliseconds(500));

		std::unique_lock<std::mutex> lockName(bill_mutex);

		bill.add(17, 29);
		cv.notify_one();
	}
	
}

void PrintOut() {

	while (true) {

		std::unique_lock<std::mutex> lockName2(bill_mutex);
		cv.wait(lockName2);

		cout << "Total: " << bill.total() << "\n";
	}
}

int main(int argc, char* argv[])
{

	std::thread printThread(PrintOut);
	std::thread producer(callAdd);

	printThread.join();
	producer.join();

	return 0;
}