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

#include <QtNetwork>
#include <QtCore>
#include "wConnection.h"
#include "wRequest.h"
#include "wError.h"
#include "wViewerHandler.h"
#include "wVoiceClient.h"
#include "Global.h"
#include "MainWindow.h"
#include "wTalkingThread.h"
#include "wConfigFile.h"

using namespace whisper;

//Thread run
void ViewerHandler::run() {
    WDEF;
    pSocket = 0;
	int i;

    //Request and voiceClient creation and initialization (using ViewerHandler::requestQueue)

    //TalkingThread tt(this->request);
    this->tt = new TalkingThread(*this);
	this->connection = new Connection(*this, *(this->tt));
    this->request = new Request(*this, requestQueue, *connection);

    //VoiceClient deals with messages originated in the client
    VoiceClient voiceClient(requestQueue);
    voiceClient.start();

    WDEBUG1("ViewerHandler: whisper started");

    //Server listening to localhost:44125
    QTcpServer server;
    server.listen(QHostAddress::LocalHost, (quint16)44125);

    WDEBUG3("ViewerHandler: waiting for connection to %s:%d", "127.0.0.1", 44125);

    //Timeout
    if(!server.waitForNewConnection(10000))
        WTHROW1("no connection found");

    // Getting ViewerHandler::pSocket
    pSocket = server.nextPendingConnection();

    if (!pSocket)
        WTHROW1("ViewerHandler: invalid socket - terminating");

    WDEBUG1("ViewerHandler: entering main loop");
	initialized();
    // Main request processing loop
    for(;;) {

        if (pSocket->bytesAvailable() > 0 || pSocket->waitForReadyRead(10000)) {

            WDEBUG1("ViewerHandler: reading data from buffer");

            //Reads the whole buffer
            QString qsData = pSocket->readAll();

			// TODO: it might be that this is not the only request in the string
			//       -> the request should be handled properly
			if(qsData.contains("Connector.InitiateShutdown.1")) {
				WDEBUG1("ViewerHandler: Connector.InitiateShutdown.1 received - doing nothing");
                //break;
			}

            //Splits the buffer in several requests
            QStringList& rlRequests = qsData.split("\n\n\n", QString::SkipEmptyParts);

            //Counts number of requests
            int count = rlRequests.size();

			WDEBUG2("ViewerHandler: %d request(s) received", count);

            //For each request
            for (i=0; i<count; i++) {
				WDEBUG3("ViewerHandler: Sending request %d to be handled: \n%s\n", i, rlRequests[i].toAscii().data());

                //handles it
                if(this->request->handle(rlRequests[i])) {
					WDEBUG1("ViewerHandler: error while handling request - next request");
                    continue;
                }
            }
        //If there're no requests received
        } else {
            //Checks if socket is still valid and disconnect if not
            WDEBUG2("Socket state: %d", (int) pSocket->state());
            if(pSocket->state() != QAbstractSocket::ConnectedState) {
                WDEBUG1("Socket is no more connected - closing program");
				WWRITE1 ("Socket no more connected - shutting down");

                break;
            }
        }
    }

    WEND:
				
	WDEBUG1("ViewerHandler: sending quit to the main application");
	qApp->quit();

	while (!is_shutdown()) {
		//wait to receive shutdown from main thread
		WDEBUG1("ViewerHandler: waiting to receive shutdown");
		QThread::msleep(1000);
		//vhSleep(1000);
	}
	WDEBUG1("ViewerHandler: shutdown received");

    WDEBUG1("ViewerHandler: stopping VoiceClient...");
    voiceClient.end();

    voiceClient.wait();
	WDEBUG1("Viewer Handler: VoiceClient has terminated");

    //WDEBUG1("Stopping talkingThread...");
    //if(this->tt->isRunning())this->tt->quit();
    WDEBUG1("ViewerHandler: stopping TalkingThread...");
    //voiceClient.end();

	if(tt != 0) {
		tt->end();
		tt->wait();
		delete tt;
		tt=0;
		WDEBUG1("Viewer Handler: TalkingThread has terminated");
	}

    WDEBUG1("ViewerHandler: finishing...");

	if (pSocket){
		pSocket->close();

		WDEBUG2("ViewerHandler: pSocket status: %s", pSocket->state() == QAbstractSocket::UnconnectedState ? "Unconnected" : "Still Up");
		pSocket = 0;
	}
	server.close();

	WDEBUG2("ViewerHandler: server status: %s", server.isListening() ? "Listening" : "Shutdown");

    return;
}


//Sends message through ViewerHandler::pSocket
int ViewerHandler::sendMessage(Message& rMsg) {
	QMutexLocker locker(&mSenderMutex);

	WDEF;

	if (is_active()) {

		//Create buffer from message data
		QByteArray& rsBuffer(rMsg.getSendData());

		//Add to message (not part of XML)
		rsBuffer.append(0x0A).append(0x0A).append(0x0A);

		//Sends the data
		if(pSocket->state() == QAbstractSocket::ConnectedState) {
			pSocket->write(rsBuffer);
			if(!pSocket->waitForBytesWritten(2000))
				WTHROW1("send operation timed out");

			WDEBUG2("ViewerHandler: message sent: \n%s", rsBuffer.data());
		}
	} else {
		WDEBUG1("Viewer Handler: Entering sendMessage after thread end!");
	}
	WEND:
    return WCODE;
}


// ---------------------------------
// callbacks for connection handling
// ---------------------------------

void ViewerHandler::addParticipant(unsigned int id) {
	if (is_active()) {
		this->connection->addParticipant(id);
	} else {
		WDEBUG1("ViewerHandler::addParticipant entered after while thread not running");
	}
}


void ViewerHandler::removeParticipant(unsigned int id) {
	// VGDEBUG
	if (is_active()) {
	    this->connection->removeParticipant(id);
	} else {
		// CHECKTHIS
		WDEBUG1("ViewerHandler::removeParticipant entered after while thread not running");
	}
}

/*
void ViewerHandler::setTalking(unsigned int id) {
	WDEF;

	if (is_active()) {

		WDEBUG1("ViewerHandler: setTalking called");
		ClientUser *p=ClientUser::get(id);

		WASSERT(this->tt != 0);
		if(!this->tt->isRunning()) {
			WDEBUG1("ViewerHandler: starting TalkingThread");
			this->tt->start();
		}

		switch (p->tsState) {
			case ClientUser::Talking:
			case ClientUser::TalkingWhisper:
			case ClientUser::TalkingWhisperChannel:
				this->tt->setUser(id, p->qsName, true);
				WDEBUG3("ViewerHandler: %d %s is talking", id, p->qsName.toAscii().data());
				break;
			case ClientUser::TalkingOff:
			default:
				this->tt->setUser(id, p->qsName, false);
				WDEBUG3("ViewerHandler: %d %s is not talking", id, p->qsName.toAscii().data());
				break;
		}
	} else {
		// CHECKTHIS
		WDEBUG1("ViewerHandler::setTalking entered after while thread not running");
	}
	WEND:;
}
*/


void ViewerHandler::setTalking(unsigned int id) {
	if (is_active()) {
		// start thread only when first requests arrive
		if(!this->tt->isRunning()) {
			WDEBUG1("ViewerHandler: starting TalkingThread");
			this->tt->start();
		}

		WDEBUG1("ViewerHandler: setTalking called");
		connection->setTalking(id);
	} else {
		WDEBUG1("ViewerHandler::setTalking entered after while thread not running");
	}
}


void ViewerHandler::talkingChanged(bool bTalking, QString& rUserName) {
	if (is_active()) {
		WDEBUG1("ViewerHandler: talkingChanged called");
		connection->talkingChanged(bTalking, rUserName);
	} else {
		WDEBUG1("ViewerHandler::talkingChanged entered after while thread not running");
	}
}
