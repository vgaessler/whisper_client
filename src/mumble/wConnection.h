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

#ifndef WCONNECTION_H
#define WCONNECTION_H

#pragma warning(disable : 4996)

#include <QtCore>

#include "wGameHandler.h"
#include "wMessage.h"
#include "wTalkingThread.h"

namespace whisper {
	class GameHandler;
	class TalkingThread;

	// Abstract parent class for implementation
	class Connection {
	public:
		Connection(GameHandler& rG, TalkingThread& rT):rTalk(rT), rViewer(rG), localMicMuted(false) {}
		virtual ~Connection() {}
		virtual void init() {}
		virtual void end() {}

		// ---------------------
		// Callbacks form mumble
		// ---------------------
        void setLocalMicMuted(bool bMute) {localMicMuted = bMute;}
		void talkingChanged(bool isTalking, const QString userName);
		void setTalking(unsigned int id);
        void addParticipant(unsigned int id);
        void removeParticipant(unsigned int id);

		// ---------------------
		//Setters
		// ---------------------
        void setName(const QString& userName) {
            uName = userName;
            //Sets AccountHandle provided ConnectorHandle exists
            if(!cHandle.isEmpty()) this->setAccountHandle(cHandle + uName);
            //Sets ParticipantURI provided Host exists
            if(!host.isEmpty()) this->setParticipantURI(QString("sip:"+uName+"@"+host));
        };

        void setPwd(const QString& userPwd) {uPwd = userPwd;}

        void setHost(const QString& newHost) {
            host = newHost;
            //Sets ParticipantURI provided UserName exists
            //Format: sip:[AccountName]@[Host] -- Without Port!!
            if(!uName.isEmpty()) this->setParticipantURI(QString("sip:"+uName+"@"+host));
        };

        void setPort(const QString& newPort) {port = newPort;}
        void setClient(const QString& clientName) {cName = clientName;}

		void setConnectorHandle(const QString& connectorHandle) {
            cHandle = connectorHandle;
            //Sets AccountHandle provided UserName exists
            if(!uName.isEmpty()) this->setAccountHandle(cHandle + uName);
        };

        void setAccountHandle(const QString& accountHandle) {
            aHandle = accountHandle;
            //Sets SessionHandle and SessionGroupHandle
            this->setSessionHandle(aHandle + QString("0"));
            this->setSessionGroupHandle(aHandle + QString("_sg0"));
        };

        void setSessionHandle(const QString& sessionHandle) {sHandle = sessionHandle;}
        void setSessionGroupHandle(const QString& sessionGroupHandle) {sgHandle = sessionGroupHandle;}
        void setURI(const QString& uri) {URI = uri;}
        void setParticipantURI(const QString& participantURI) {pURI = participantURI;}

		// ---------------------
        //Getters
		// ---------------------
        const QString getName() {return uName;}
        const QString getPwd() {return uPwd;}
        const QString getHost() {return host;}
        const QString getPort() {return port;}
        const QString getClient() {return cName;}
        const QString getConnectorHandle() {return cHandle;}
        const QString getAccountHandle() {return aHandle;}
        const QString getSessionHandle() {return sHandle;}
        const QString getSessionGroupHandle() {return sgHandle;}
        const QString getURI() {return URI;}
        const QString getParticipantURI() {return pURI;}
        const bool getLocalMicMuted() {return localMicMuted;}
	
	private:
		GameHandler& rViewer;
		TalkingThread& rTalk;

        //Connection data
        QString uName;      //AccountName
        QString uPwd;       //AccountPassword
        QString cName;      //ClientName
        QString host;       //HostNumber
        QString port;       //PortNumber
        QString cHandle;    //ConnectorHandle
        QString aHandle;    //AccountHandle
        QString sHandle;    //SessionHandle
        QString sgHandle;   //SessionGroupHandle
        QString URI;        //URI
        QString pURI;       //ParticipantURI
        bool localMicMuted; //Mute Status of Local Mic
	};
}

#endif // WCONNECTION_H
