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
