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

#ifndef WREQUEST_H
#define WREQUEST_H

#pragma warning(disable : 4996)

#include <QtCore>
#include <QtXml>

//#include "Global.h"
//#include "ClientUser.h"
#include "wConnection.h"
#include "wXmlWriter.h"
#include "wEvent.h"
#include "wResponse.h"
#include "wRequestQueue.h"
#include "wError.h"

namespace whisper {
    class Actor;
	class ViewerHandler;
	class Connection;

    class Request : public QObject {
//        Q_OBJECT
    public:
        Request(ViewerHandler &rVh, RequestQueue& rQ, Connection& rC);
        int handle(QString& rsRequest);
		/*
        void talkingChanged(const bool&, const QString);
        void addParticipant(unsigned int id);
        void removeParticipant(unsigned int id);
		*/


    protected:
        Actor* createActor(const QString rs);

    private:
		ViewerHandler &rViewer;
        RequestQueue& rQueue;
        QXmlStreamReader qR;
		Response response;
		Connection& rConn;

        Actor* pCurActor;
        int execError;

        // results
        QString qsStatusCode;
        QString qsStatusString;
        QString qsResults;

        int parse(QString* psRequest);
        int execute();
        int sendResponse();
    };


    // base actor
    class Actor {
    public:
		Actor(ViewerHandler &rVh, Connection& rC) : rViewer(rVh), rConn(rC) {}
        int executeRequest() {setStatus(QByteArray("0"), QByteArray(""), true); return execute();}
#pragma warning(disable : 4100)
        virtual int parse(QXmlStreamReader& rR) {return 0;}
        virtual int response(XmlWriter* pW) {return 0;}
#pragma warning(default : 4100)
        virtual void postProcess(){}
		virtual bool mustSendResponse() {return true;}
        //virtual bool isShutdown() {return false;}
        QByteArray& getStatusCode() {return sStatusCode;}
        QByteArray& getStatusString() {return sStatusString;}
        bool isSuccess() {return bSuccess;}

    protected:
		ViewerHandler &rViewer;
		Connection &rConn;
        void setStatus(const QByteArray rsStatusCode, const QByteArray rsStatusString, const bool bSuccess_p) {
            sStatusCode = rsStatusCode; sStatusString=rsStatusString; bSuccess = bSuccess_p;
        }
        virtual int execute() {return 0;}

    private:
        QByteArray sStatusCode;
        QByteArray sStatusString;
        bool bSuccess;
    };

    // Conntector
    class ConnectorCreate1 : public Actor{
    public:
		ConnectorCreate1(ViewerHandler &rVh, Connection& rC) : Actor(rVh, rC), handle_count(0) {}
        int parse(QXmlStreamReader& rR);
        int execute();
        int response(XmlWriter* pW);

    private:
        int handle_count;
        QString qsConnectorHandle;
    };

    class ConnectorInitiateShutdown1 : public Actor{
    public:
		ConnectorInitiateShutdown1(ViewerHandler &rVh, Connection& rC) : Actor(rVh, rC) {}
        int execute();
		virtual bool mustSendResponse() {return false;}
    };

    class ConnectorMuteLocalMic1 : public Actor{
	public:
		ConnectorMuteLocalMic1(ViewerHandler &rVh, Connection& rC) : Actor(rVh, rC) {}
		int parse(QXmlStreamReader& rR);
		int execute();
	private:
		bool bMute;
    };

    class ConnectorSetLocalMicVolume1 : public Actor{
	public:
		ConnectorSetLocalMicVolume1(ViewerHandler &rVh, Connection& rC) : Actor(rVh, rC) {}
    };

    class ConnectorMuteLocalSpeaker1 : public Actor{
    public:
 		ConnectorMuteLocalSpeaker1(ViewerHandler &rVh, Connection& rC) : Actor(rVh, rC) {}
        int execute();
        int parse(QXmlStreamReader& rR);
        int response(XmlWriter* pW);
        QString newValue;
    };

    class ConnectorSetLocalSpeakerVolume1 : public Actor{
    public:
 		ConnectorSetLocalSpeakerVolume1(ViewerHandler &rVh, Connection& rC) : Actor(rVh, rC) {}
        int execute();
        int parse(QXmlStreamReader& rR);
        int response(XmlWriter* pW);
        QString newVolume;
    };

    // Aux
    class AuxGetCaptureDevices1 : public Actor {
    public:
 		AuxGetCaptureDevices1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
        int execute();
        int response(XmlWriter* pW);
    };

    class AuxSetCaptureDevice1 : public Actor {
    public:
 		AuxSetCaptureDevice1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
        int execute();
        int parse(QXmlStreamReader& rR);
        int response(XmlWriter* pW);
        QString deviceName;
    };

    class AuxGetRenderDevices1 : public Actor {
    public:
 		AuxGetRenderDevices1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
        int execute();
        int response(XmlWriter* pW);
    };

    class AuxSetRenderDevice1 : public Actor {
    public:
 		AuxSetRenderDevice1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
        int execute();
        int parse(QXmlStreamReader& rR);
        int response(XmlWriter* pW);
        QString deviceName;
    };

    // Account
    class AccountLogin1 : public Actor {
    public:
        AccountLogin1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
        int parse(QXmlStreamReader& rR);
        int execute();
        int response(XmlWriter* pW);
        void postProcess();
    };

	class AccountLogout1 : public Actor {
    public:
        AccountLogout1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
		bool mustSendResponse() {return false;}
		// does nothing
    };

    class AccountListBlockRules1 : public Actor {
    public:
 		AccountListBlockRules1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
        int response(XmlWriter* pW);
    };

    class AccountListAutoAcceptRules1 : public Actor {
    public:
 		AccountListAutoAcceptRules1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
        int response(XmlWriter* pW);
    };


    // Session
    class SessionCreate1 : public Actor {
    public:
        SessionCreate1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
        int execute();
        int parse(QXmlStreamReader& rR);
        int response(XmlWriter* pW);
        void postProcess();
    private:
        QByteArray sUri;  // Session URI
        QString aHandle;  // Account Handle
        QString sHandle;  // Session Handle
        QString sgHandle; // Session Group Handle
        QString pURI;     // Participant Uri
    };

    class SessionSet3DPosition1 : public Actor {
    public:
 		SessionSet3DPosition1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
        int execute();
    };

    class SessionMediaDisconnect1 : public Actor {
    public:
        SessionMediaDisconnect1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
        int execute();
        //int response(XmlWriter *pWriter);
        void postProcess();
    };

    class SessionGroupTerminate1 : public Actor {
    public:
        SessionGroupTerminate1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {};
        int execute();
        void postProcess();
    };

    class AuxCaptureAudioStart1 : public Actor {
    public:
 		AuxCaptureAudioStart1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
        int execute();
        void postProcess();
        int response(XmlWriter *pWriter);
    };

    class AuxCaptureAudioStop1 : public Actor {
    public:
 		AuxCaptureAudioStop1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
        int execute();
        int response(XmlWriter *pWriter);
    };

    class AuxSetMicLevel1 : public Actor {
    public:
 		AuxSetMicLevel1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
        int execute();
    };

    class AuxSetSpeakerLevel1 : public Actor {
    public:
 		AuxSetSpeakerLevel1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
        int execute();
    };

}

#endif // WREQUEST_H
