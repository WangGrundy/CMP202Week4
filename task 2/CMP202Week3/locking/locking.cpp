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


void callAdd(int threadNum) {
	
	sleep_for(seconds(1)); // wait for 1 second
	
	std::unique_lock<std::mutex> lockName(bill_mutex); //lock thread, so only 1 tread can add to the bill
	
	bill.add(1, 0); //add to bill

	result_ready = true; //result is ready to print
	cv.notify_one(); //updates bill total for rest of threads
}

void PrintOut() {

	while (1 != 0) {
		std::unique_lock<std::mutex> lockName(bill_mutex); //lock 

		while (!result_ready) {

			cv.wait(lockName); //blocks the current thread until the condition variable is awakened
		}


		cout << "bill total: " << bill.total() << "\n";

		result_ready = false;
	}
	

}

int main(int argc, char *argv[])
{
	std::vector<std::thread> threadVector;
	std::thread printThread(PrintOut);

	while (1 != 0) {
		for (int i = 0; i < 10; i++) {
			threadVector.push_back(std::thread(callAdd, i));
		}
	}
	

	for (std::thread& thread : threadVector) {
		thread.join();
	}
	printThread.join();

	return 0;
}