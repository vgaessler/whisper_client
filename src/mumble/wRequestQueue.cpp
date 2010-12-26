/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>,
                            Volker Gaessler <volker.gaessler@vcomm.ch

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
