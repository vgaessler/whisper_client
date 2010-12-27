/*
   Whisper is a Mumble based voice client for OpenSim
   Copyright (C) 2009-2010  Volker Gaessler <volker.gaessler@vcomm.ch>

   This file is part of Whisper.

   Whisper is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>
*/

#include <QtCore/QMutex>
#include "wError.h"
#include "wRequestQueue.h"

using namespace std;
using namespace whisper;

//Adds an actor to the queue
void RequestQueue::enqueue(Actor* pA) {
    WDEF;

    //Locks the processes
    QMutexLocker locker(&mutex);

    WASSERT(pA);

    //Adds the actor
    queue.enqueue(pA);

    //Wakes up the processes
    dataAvailable.wakeAll();

    WDEBUG2("RequestQueue: request enqueued, %d in queue now", queue.count());
    WEND:;
    }

//Take an actor out from the queue and executes its postProcess().
Actor* RequestQueue::dequeue() {

    //Initialize the actor
    Actor* pA = 0;

    //Locks the processes
    QMutexLocker locker(&mutex);

    //Loops till an actor is found. If none, release the processes and waits one second
    while(1) {

        if(!queue.isEmpty()) {
            pA = queue.dequeue();
            if(!pA) {
                WDEBUG1("Null pointer dequeued from request queue! -> Waiting for new data.");
            } else {
                WDEBUG2("RequestQueue: request dequeued, %d remaining", queue.count());
                return pA;
            }
        }
		// return in case of timeout
		if (!dataAvailable.wait(&mutex, 1000))
			return 0;
    }
}
