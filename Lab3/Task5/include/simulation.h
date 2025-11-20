#ifndef SIMULATION_H
#define SIMULATION_H

#include "manager.h"
#include <unistd.h>

#define TICK 200000

// unused
void startSimulation(MailService *service);

void simulateTick(MailService *service);

void runTicks(MailService *service, unsigned int n);

#endif