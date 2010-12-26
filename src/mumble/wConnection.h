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
