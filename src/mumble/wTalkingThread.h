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
 #ifndef WTALKINGTHREAD_H
#define WTALKINGTHREAD_H

#include <QtCore>
#include <QThread>
#include <QList>

#include "wGameHandler.h"

namespace whisper {
	class Connection;
    class TalkingUser;
	class Request;

    class TalkingThread : public QThread
    {
    public:
		TalkingThread(GameHandler& rG) : rGame(rG) {bEnd = false;}
        void run();
		void end() {bEnd = true;}
        static void ttSleep(unsigned long msec) { QThread::msleep(msec); };
        void setUser(const unsigned int&, const QString&, const bool&);

	private:
		GameHandler& rGame;
        TalkingUser* userExists(const unsigned int&);
        QList<TalkingUser> usersArray;
        bool bEnd;
		QMutex mUserMutex;
    };

    class TalkingUser {
    public:
        TalkingUser(const unsigned int& uId, const QString& uName, const bool& uIsTalking) {id = uId; name = uName; bTalking = uIsTalking;}
        void setData(const unsigned int&, const QString&, const bool& uisTalking);
		bool isTalking() {return bTalking;}
		QString& getName() {return name;}
		unsigned int getId() {return id;}

	private:
        unsigned int id;
        QString name;
        bool bTalking;
    };

}
#endif // WTALKINGTHREAD_H
