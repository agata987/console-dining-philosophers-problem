#include <thread>
#include <string>
#include <ncurses.h>
#include <mutex>

using Fork=std::mutex;

class Philosopher
{
	public:
		Philosopher(std::string name, Fork & left_fork, Fork & right_fork, bool * stop, int*hunger, int number, int * progress, bool * started);
		virtual ~Philosopher();


	const std::string name;
	std::string * states;
	int currentState=0;

	protected:

	private:

	Fork & left_fork;
	Fork & right_fork;
	std::thread thread_;

	void think();
	void eat();
	void dine();
	void sleep(bool eating);

	int randomTime_microseconds();

	bool * stop;
	int * hunger;
	int * progress;
	bool * started;
	// number of philosopher
	int number;
	int left_neighbor;
	int right_neighbor;
};
