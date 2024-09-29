#include "Simulator.h"
using namespace std;

// Returns a number according to exponential distribution
double exponential(double rate)
{
	double U = (double) rand() / RAND_MAX;
	return -(1 / rate) * log(1 - U);
}

// Test mean and variance of exponential
void test_exp()
{
	int runs = 5000;
	double samples[runs];
	double cumulative = 0;
	for (int i = 0; i < runs; ++i)
	{
		samples[i] = exponential(75);
		cumulative += samples[i];
	}
	double mean = cumulative / runs;

	cumulative = 0;
	for (int i = 0; i < runs; ++i)
	{
		cumulative += pow(samples[i] - mean, 2);
	}
	double variance = cumulative / (runs - 1);

	cout << "Mean: " << mean << endl;
	cout << "Variance: " << variance << endl;
}

void infinite()
{
	// Generate Events --------------------------------------------------------------------------------

	list<Event*> event_queue;

	// Generate arrival events
	double time = 0;
	while (time < sim_time)
	{
		time += exponential(avg_arrive);
		event_queue.push_back(new Event(time, ARRIVAL));
	}

	// Generate departure events
	time = 0;
	for (Event* arrival : event_queue)
	{
		double service_time = exponential(avg_sevice);

		if (time > arrival->time_stamp) { time += service_time; }	// Buffer not empty (no wait) 
		else { time = arrival->time_stamp + service_time; }				// Buffer empty (wait)

		event_queue.push_front(new Event(time, DEPARTURE));
	}

	// Generate observer events
	time = 0;
	while (time < sim_time)
	{
		time += exponential(5 * avg_arrive);
		event_queue.push_back(new Event(time, OBSERVER));
	}

	// Sort events in ascending time
	event_queue.sort([](Event* lhs, Event* rhs)
	{
		return lhs->time_stamp < rhs->time_stamp;
	});

	// Process Events ---------------------------------------------------------------------------------

	int num_arrive = 0;
	int num_depart = 0;
	int num_observe = 0;
	int avg_queue = 0;
	int avg_idle = 0;

	for (Event* event : event_queue)
	{
		if (event->event_type == ARRIVAL) { ++num_arrive; }
		else if (event->event_type == DEPARTURE) { ++num_depart; }
		else
		{
			++num_observe;
			if (num_arrive == num_depart) { ++avg_idle; }
			else { avg_queue += num_arrive - num_depart; }
		}
		free(event);
	}
	cout << "Arrivals: " << num_arrive << endl;
	cout << "Departures: " << num_depart << endl;
	cout << "Observers: " << num_observe << endl;
	cout << "E[N]: " << ((double) avg_queue / num_observe) << endl;
	cout << "P_IDLE: " << ((double) avg_idle / num_observe) << endl;

	return;
}

void finite()
{
	list<Event*> event_queue;

	// Generate arrival events
	double time = 0;
	while (time < sim_time)
	{
		time += exponential(avg_arrive);
		event_queue.push_back(new Event(time, ARRIVAL));
	}

	// Generate observer events
	time = 0;
	while (time < sim_time)
	{
		time += exponential(5 * avg_arrive);
		event_queue.push_back(new Event(time, OBSERVER));
	}

	// Sort events in ascending time
	event_queue.sort([](Event* lhs, Event* rhs)
	{
		return lhs->time_stamp < rhs->time_stamp;
	});

	// Process Events ---------------------------------------------------------------------------------

	list<Event*> departure_queue;
	int num_arrive = 0;
	int num_depart = 0;
	int num_observe = 0;
	int num_dropped = 0;
	int avg_queue = 0;
	int avg_idle = 0;
	time = 0;

	for (Event* event : event_queue)
	{
		//Departures happened if current event is newer
		while (!departure_queue.empty() && (event->time_stamp > departure_queue.front()->time_stamp))
		{
			++num_depart;
			free(departure_queue.front());
			departure_queue.pop_front();
		}

		if (event->event_type == ARRIVAL) 
		{
			//If queue is full, cannot take more arrivals.
			//Else, generate departure in real-time.
			if (num_arrive - num_depart == buf_size) { ++num_dropped; }
			else 
			{
				++num_arrive;
				double service_time = exponential(avg_sevice);

				if (time > event->time_stamp) { time += service_time; }	// Buffer not empty (no wait) 
				else { time = event->time_stamp + service_time; }				// Buffer empty (wait)

				departure_queue.push_back(new Event(time, DEPARTURE));
			}
		}
		else
		{
			++num_observe;
			if (num_arrive == num_depart) { ++avg_idle; }
			else { avg_queue += num_arrive - num_depart; }
		}
		free(event);
	}
	cout << "Arrivals: " << num_arrive << endl;
	cout << "Departures: " << num_depart << endl;
	cout << "Observers: " << num_observe << endl;
	cout << "E[N]: " << ((double) avg_queue / num_observe) << endl;
	cout << "P_IDLE: " << ((double) avg_idle / num_observe) << endl;
	cout << "P_LOSS: " << ((double) num_dropped / num_arrive) << endl;

	return;
}

int main()
{
	// Seed rand() with time of day
	srand(time(NULL));
	if (test) { test_exp(); }
	if (inf) { infinite(); }
	if (fin) { finite(); }

	return 0;
}
