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

#include "wTalkingThread.h"
#include "wRequest.h"
#include "wError.h"

using namespace whisper;

void TalkingThread::run() {
    for(;;) {
        for(int i=0; i<this->usersArray.size(); i++) {
            if(usersArray[i].isTalking()) {
                WDEBUG2("Sending true at %s", usersArray[i].getName().toAscii().data());
				rGame.talkingChanged(true, usersArray[i].getName());
            }
            else {
                WDEBUG2("Sending false at %s", usersArray[i].getName().toAscii().data());
				rGame.talkingChanged(false, usersArray[i].getName());
                usersArray.removeAt(i);
            }
        }
        ttSleep(400);

		if(bEnd) {
			WDEBUG1("Thread TalkingThread returning");
			return;
		}
    }
}

void TalkingThread::setUser(const unsigned int& id, const QString& name, const bool& isTalking) {
	QMutexLocker locker(&mUserMutex);

    TalkingUser* actual = this->userExists(id);
    if(actual->getName() != "") {
        actual->setData(id, name, isTalking);
        WDEBUG2("Changing already created %s", actual->getName().toAscii().data());
    }
    else {
        TalkingUser tt(id, name, isTalking);
        usersArray.append(tt);
        WDEBUG2("Changing new %s", name.toAscii().data());
    }
}

TalkingUser* TalkingThread::userExists(const unsigned int& id) {
	QList<TalkingUser>::iterator i;
    for(i=usersArray.begin(); i!=this->usersArray.end(); ++i) {
        TalkingUser temp = *i;
        if((*i).getId() == id)
            return &(*i);
    }
    return &TalkingUser(0, "", false);
}

void TalkingUser::setData(const unsigned int& uid, const QString& uname, const bool& uisTalking) {
    id = uid;
    name = uname;
    bTalking = uisTalking;
}

