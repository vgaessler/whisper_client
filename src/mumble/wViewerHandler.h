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

#ifndef WVIEWERHANDLER_H
#define WVIEWERHANDLER_H

#include <QtCore>
#include <QtNetwork>

#include "ClientUser.h"
#include "Global.h"

#include "wConnection.h"
#include "wRequest.h"
#include "wGameHandler.h"
#include "wRequestQueue.h"
#include "wMessage.h"
#include "wTalkingThread.h"

namespace whisper {
	class Connection;

	class ViewerHandler : public GameHandler {
//        Q_OBJECT
    public:
        ViewerHandler(QObject *parent = 0) : GameHandler(parent) {tt = 0;}
		virtual ~ViewerHandler() {}
        void run();

		void vhSleep(unsigned long msec);
        int sendMessage(Message& rMsg);
        QMutex mutex;

		// Delegeation methods forwarding to connection
        void addParticipant(const unsigned int);
        void removeParticipant(const unsigned int);
        void setTalking(const unsigned int);
		void talkingChanged(bool bTalking, QString& rUserName);

    private:
        static ViewerHandler* pInstance;
        QTcpSocket* pSocket;
        RequestQueue requestQueue;
        Request* request;
		QMutex mSenderMutex;
		Connection* connection;
        TalkingThread* tt;
   };
}

#endif // WVIEWERHANDLER_H
