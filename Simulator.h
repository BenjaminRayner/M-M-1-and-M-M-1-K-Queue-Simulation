#pragma once

#include <iostream>
#include <cmath>
#include <ctime>
#include <list>
#include "Event.h"

#define ARRIVAL 1
#define DEPARTURE 2
#define OBSERVER 3

const int sim_time = 5000;							          // seconds
const int avg_size = 2000;							          // bits/packet
const int net_speed = 1000000;						        // bits/second
const double intensity = 0.15;						        // arrival/service
const double avg_sevice = net_speed / avg_size;		// packets/second
const double avg_arrive = intensity * avg_sevice;	// packets/second

const int buf_size = 10;							            // packets

const bool test = false;							            // Test exponential()
const bool inf = true;								            // Infinite buffer
const bool fin = false;								            // Finite buffer

double exponential(double rate);
void test_exp();
void infinite();
void finite();
