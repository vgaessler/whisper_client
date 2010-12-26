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

#include <QtCore>

#include "ClientUser.h"

#include "wConnection.h"
#include "wError.h"
#include "wEvent.h"

using namespace whisper;

void Connection::talkingChanged(bool isTalking, const QString userName) {
    //Sends message except when the local mic is muted and the user is local user.
    if(!(getLocalMicMuted() && (userName == getName()))) {
        QString userParticipantURI = "sip:" + userName + "@" + getHost();

        ParticipantUpdatedEvent oEvent;
        oEvent.reset(QByteArray(getAccountHandle().toAscii()), 
				     QByteArray(getSessionGroupHandle().toAscii()), 
					 QByteArray(getSessionHandle().toAscii()));
        oEvent.setParticipantUri(userParticipantURI.toAscii());
        oEvent.setIsSpeaking(isTalking ? QByteArray("true") : QByteArray("false"));
        oEvent.setVolume(QByteArray("30"));
        oEvent.setEnergy(QByteArray("0.5"));
        oEvent.compose();
        rViewer.sendMessage(oEvent);
    }
}


void Connection::addParticipant(unsigned int id) {
    ClientUser *p = ClientUser::get(id);
    QString userName = p->qsName;
    QString userParticipantURI = "sip:" + userName + "@" + getHost();
    //Sends ParticipantAddedEvent
    {
        ParticipantAddedEvent oEvent;
        oEvent.reset(QByteArray(getAccountHandle().toAscii()), QByteArray(getSessionGroupHandle().toAscii()), QByteArray(getSessionHandle().toAscii()));
        oEvent.setParticipantUri(QByteArray(userParticipantURI.toAscii()));
        oEvent.setAccountName(QByteArray(userName.toAscii()));
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }

    //Sends ParticipantUpdatedEvent
    {
        ParticipantUpdatedEvent oEvent;
        oEvent.reset(QByteArray(getAccountHandle().toAscii()), QByteArray(getSessionGroupHandle().toAscii()), QByteArray(getSessionHandle().toAscii()));
        oEvent.setParticipantUri(QByteArray(userParticipantURI.toAscii()));
        oEvent.setIsSpeaking(QByteArray("true"));
        oEvent.setVolume(QByteArray("0"));
        oEvent.setEnergy(QByteArray("0"));
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }
}


void Connection::removeParticipant(unsigned int id) {
    ClientUser *p = ClientUser::get(id);
    QString userName = p->qsName;
    QString userParticipantURI = "sip:" + userName + "@" + getHost();
    //Sends ParticipantRemovedEvent
    {
        ParticipantRemovedEvent oEvent;
        oEvent.reset(QByteArray(getAccountHandle().toAscii()), QByteArray(getSessionGroupHandle().toAscii()), QByteArray(getSessionHandle().toAscii()));
        oEvent.setParticipantUri(QByteArray(userParticipantURI.toAscii()));
        oEvent.setAccountName(QByteArray(userName.toAscii()));
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }
}

void Connection::setTalking(unsigned int id) {
	WDEF;

	WDEBUG1("ViewerHandler: setTalking called");
	ClientUser *p=ClientUser::get(id);
	WASSERT(p);

	switch (p->tsState) {
		case ClientUser::Talking:
		case ClientUser::TalkingWhisper:
		case ClientUser::TalkingWhisperChannel:
			rTalk.setUser(id, p->qsName, true);
			WDEBUG3("ViewerHandler: %d %s is talking", id, p->qsName.toAscii().data());
			break;
		case ClientUser::TalkingOff:
		default:
			rTalk.setUser(id, p->qsName, false);
			WDEBUG3("ViewerHandler: %d %s is not talking", id, p->qsName.toAscii().data());
			break;
	}

	WEND:;
}
