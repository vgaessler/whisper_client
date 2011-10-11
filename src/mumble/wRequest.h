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

	class AccountGetSessionFonts1 : public Actor {
	public:
		AccountGetSessionFonts1(ViewerHandler &rVh, Connection& rC): Actor(rVh, rC) {}
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
