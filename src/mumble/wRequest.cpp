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

#include <iostream>

#if defined(Q_OS_WIN)
//#include <windows.h>
#endif

#include "wGlobal.h"
#include "wConnection.h"
#include "wError.h"
#include "wXmlWriter.h"
#include "wRequest.h"
#include "wViewerHandler.h"
#include "AudioWizard.h"
#include "AudioInput.h"
#include "Global.h"
#include "ServerHandler.h"
#include "MainWindow.h"
#include "Channel.h"
#include "ClientUser.h"

using namespace std;
using namespace whisper;


Request::Request(ViewerHandler &rVh, RequestQueue& rQ, whisper::Connection& rC): rQueue(rQ), pCurActor(0), rViewer(rVh), rConn(rC) 
{}

Actor* Request::createActor(const QString rs) {

    //
    //Initialize Actor hash table with action strings
    //

    if(!rs.compare("Aux.GetRenderDevices.1"))
        return new AuxGetRenderDevices1(rViewer, rConn);

    else if(!rs.compare("Aux.SetRenderDevice.1"))
        return new AuxSetRenderDevice1(rViewer, rConn);

    else if(!rs.compare("Aux.GetCaptureDevices.1"))
        return new AuxGetCaptureDevices1(rViewer, rConn);

    else if(!rs.compare("Aux.SetCaptureDevice.1"))
        return new AuxSetCaptureDevice1(rViewer, rConn);

    else if(!rs.compare("Connector.Create.1"))
        return new ConnectorCreate1(rViewer, rConn);

    else if(!rs.compare("Connector.InitiateShutdown.1"))
        return new ConnectorInitiateShutdown1(rViewer, rConn);

    else if(!rs.compare("Connector.MuteLocalMic.1"))
        return new ConnectorMuteLocalMic1(rViewer, rConn);

    else if(!rs.compare("Connector.SetLocalMicVolume.1"))
        return new ConnectorSetLocalMicVolume1(rViewer, rConn);

    else if(!rs.compare("Connector.MuteLocalSpeaker.1"))
        return new ConnectorMuteLocalSpeaker1(rViewer, rConn);

    else if(!rs.compare("Connector.SetLocalSpeakerVolume.1"))
        return new ConnectorSetLocalSpeakerVolume1(rViewer, rConn);

    else if(!rs.compare("Account.Login.1"))
        return new AccountLogin1(rViewer, rConn);

    else if(!rs.compare("Account.Logout.1"))
        return new AccountLogout1(rViewer, rConn);

    else if(!rs.compare("Account.ListBlockRules.1"))
        return new AccountListBlockRules1(rViewer, rConn);

    else if(!rs.compare("Account.ListAutoAcceptRules.1"))
        return new AccountListAutoAcceptRules1(rViewer, rConn);

    else if(!rs.compare("Account.GetSessionFonts.1"))
        return new AccountGetSessionFonts1(rViewer, rConn);

    else if(!rs.compare("Session.Create.1"))
        return new SessionCreate1(rViewer, rConn);

    else if(!rs.compare("Session.Set3DPosition.1"))
        return new SessionSet3DPosition1(rViewer, rConn);

    else if(!rs.compare("Session.MediaDisconnect.1"))
        return new SessionMediaDisconnect1(rViewer, rConn);

    else if(!rs.compare("SessionGroup.Terminate.1"))
        return new SessionGroupTerminate1(rViewer, rConn);

    else if(!rs.compare("Aux.CaptureAudioStart.1"))
        return new AuxCaptureAudioStart1(rViewer, rConn);

    else if(!rs.compare("Aux.CaptureAudioStop.1"))
        return new AuxCaptureAudioStop1(rViewer, rConn);

    else if(!rs.compare("Aux.SetMicLevel.1"))
        return new AuxSetMicLevel1(rViewer, rConn);

    else if(!rs.compare("Aux.SetSpeakerLevel.1"))
        return new AuxSetSpeakerLevel1(rViewer, rConn);

    //If there's no match actor returns a null Actor
    return (Actor*) 0;
}

//Handles a request
int Request::handle(QString& rsRequest) {
    WDEF;

    //Lock the thread
    //ViewerHandler& rViewer(ViewerHandler::instance());
    //rViewer.vhMutex.lock();

    //Parse it
    WASSERT(!parse(&rsRequest));

    //Execute it
    WASSERT(!execute());

    //Send response
    WASSERT(!sendResponse());

    //Enqueue it for postProcess execution
    WASSERT(pCurActor);
    rQueue.enqueue(pCurActor);

    //Resets pCurActor
    pCurActor = 0;

    //Unlock the thread
    //rViewer.vhMutex.unlock();

    WEND:
    return WCODE;
}

//Request's Parser
int Request::parse(QString* psRequest) {
    WDEF;

    QByteArray sAction;
    QByteArray sStr;

    //Clear and add the data to the XMLStreamReader
    qR.clear();
    qR.addData(*psRequest);

    //Saves the current XML into the response object
    response.reset();
    response.setInputXml(psRequest->toAscii());

    //Skips startDocument
    QXmlStreamReader::TokenType tt = qR.readNext();

    WDEBUG1("parsing new Request");

    //Skips one in case we are at startDocument or endElement
    if(tt != QXmlStreamReader::StartElement)
        tt = qR.readNext();

    //If it's a request
    WASSERT(tt == QXmlStreamReader::StartElement);
    WASSERT(qR.qualifiedName() == "Request");

    //RequestId attribute
    sStr = qR.attributes().value("requestId").toString().toAscii();
    WDEBUG2("requestId = %s", sStr.data());
    response.setRequestId(sStr);

    //Action attribute
    sAction = qR.attributes().value("action").toString().toAscii();
    WDEBUG2("action = %s", sAction.data());
    response.setAction(sAction);

    //Creates the actor
    pCurActor = createActor(QString(sAction));
    if(!pCurActor)
        WTHROW2("Actor for %s not found", sAction.data());

    WDEBUG2("Actor %d found", pCurActor->getStatusCode().toInt());

    //Read next entry
    qR.readNext();
    //Ask the actor to parse the contents
    WASSERT(!pCurActor->parse(qR));

    //Go to endElement of request
    while ((!qR.isEndElement()) || (qR.qualifiedName() != "Request"))
        qR.readNext();

    WDEBUG1("End of request found - ok");

    WEND:

    return WCODE;
}

//Asks the actor to execute the Request
int Request::execute() {
    WDEF;

    WASSERT(!pCurActor->executeRequest());

    WEND:
    return WCODE;
}

//Creates the response from the actor, takes an instance of viewerHandler and send the message
int Request::sendResponse() {
    WDEF;

	// some actor don't need to send a response
	if (pCurActor->mustSendResponse()) {
		//WASSERT(pCurActor);
		response.compose(*pCurActor);
		WDEBUG1("Finished composing data. Sending it to viewerHandler to be dispatched.");

		// sending response
		//ViewerHandler& rViewer(ViewerHandler::instance());
		WASSERT(!rViewer.sendMessage(response));
	}

    WEND:
    return WCODE;
}


// ---------------------------------
// Actors' methods definition below
// ---------------------------------


// ------------------
// Connector.Create.1
// ------------------

int ConnectorCreate1::parse(QXmlStreamReader& rR) {
    WDEF;

    //Loop on XML till ClientName and AccountManagementServer are found
    while(!rR.atEnd() && (rConn.getClient().isEmpty() || rConn.getHost().isEmpty()))
    {
        if(rR.name() == "ClientName")
            rConn.setClient(rR.readElementText());
        else if(rR.name() == "AccountManagementServer")
        {
            //I expect something like tcp://79.125.26.171:64739
            //I split it at // and take the right-side
            QString accountMS = rR.readElementText().split("//").at(1);
            //I split it again at :
            QStringList hostPort = accountMS.split(":");
            //Left side for the host
            rConn.setHost(hostPort.at(0));
            //Right side for the port
            rConn.setPort(hostPort.at(1));
        }
        rR.readNext();
    }

    return WCODE;
}

int ConnectorCreate1::execute() {
    qsConnectorHandle.sprintf(CONNECTOR_HANDLE);
    handle_count++;

    return 0;
}

int ConnectorCreate1::response(XmlWriter* pWriter) {
    WDEBUG1("writing ConnectorCreate1 response");

    pWriter->writeStart("VersionID");
    pWriter->writeText("2.1.3010.6151");
    pWriter->writeEnd("VersionID");

    pWriter->writeStart("ConnectorHandle");
    //pWriter->writeText("c1_m1000");
    pWriter->writeText(qsConnectorHandle);
    pWriter->writeEnd("ConnectorHandle");

    return 0;
}

// -------------------------------------------------------------------
// Connector.InitiateShutdown.1
// -------------------------------------------------------------------
int ConnectorInitiateShutdown1::execute() {
    //qApp->quit();
    return 0;
}


// ------------------------
// Connector.MuteLocalMic.1
// ------------------------

int ConnectorMuteLocalMic1::parse(QXmlStreamReader& rR) {
    WDEF;
	bMute = false;

    while(!rR.atEnd()) {
        if(rR.name() == "Value") {
            QString value = rR.readElementText();
            bMute = (value == "true");

			break;
        }
        rR.readNext();
	}

    return WCODE;
}


int ConnectorMuteLocalMic1::execute() {
    if(bMute) {
        rConn.setLocalMicMuted(true);
        g.mw->qaAudioMute->setChecked(false);
		g.mw->qaAudioMute->trigger();

/*
        ClientUser *p = ClientUser::get(g.uiSession);
        if(p->isValid(g.uiSession))
            p->setMute(true);
*/
    } else {
        rConn.setLocalMicMuted(false);
        g.mw->qaAudioMute->setChecked(true);
		g.mw->qaAudioMute->trigger();

        ClientUser *p = ClientUser::get(g.uiSession);
		if(p->isValid(g.uiSession)) {
            p->setMute(false);
		}
    }

    return 0;
}


// ----------------------
// Aux.GetCaptureDevices.1
// ----------------------
int AuxGetCaptureDevices1::execute() {
    return 0;
}

int AuxGetCaptureDevices1::response(XmlWriter* pWriter) {
    WDEBUG1("writing AuxGetCaptureDevices1 response...");
    //Lista de dispositivos de captura de audio
    QString devicesList = "<CaptureDevices>";
    if (AudioInputRegistrar::qmNew) {
        WDEBUG1("Entering...");
        foreach(AudioInputRegistrar *air, *AudioInputRegistrar::qmNew) {
            AudioInputRegistrar *air2 = AudioInputRegistrar::qmNew->value(air->name);
            QList<audioDevice> ql = air2->getDeviceChoices();
            foreach(audioDevice d, ql) {
                devicesList += QString("<CaptureDevice><Device>%1</Device></CaptureDevice>").arg(d.first);
            }
        }
    }
    devicesList += QString("<CaptureDevice><Device>%1</Device></CaptureDevice>").arg(AudioInputRegistrar::current);
    devicesList += QString("</CaptureDevices><CurrentCaptureDevice><Device>%1</Device></CurrentCaptureDevice>").arg(AudioInputRegistrar::current);
    //QString devicesHardCoded = "<CaptureDevices><CaptureDevice><Device>Realtek HD Audio Input</Device></CaptureDevice></CaptureDevices><CurrentCaptureDevice><Device>Realtek HD Audio Input</Device></CurrentCaptureDevice>";
    pWriter->writeText(devicesList);
    return 0;
}


// ----------------------
// Aux.GetRenderDevices.1
// ----------------------
int AuxGetRenderDevices1::execute() {
    return 0;
}

int AuxGetRenderDevices1::response(XmlWriter* pWriter) {
    WDEBUG1("writing AuxGetRenderDevices1 response");
    //Lista de dispositivos de salida de audio
    QString devicesList = "<RenderDevices>";
    if (AudioOutputRegistrar::qmNew) {
        WDEBUG1("Entering...");
        foreach(AudioOutputRegistrar *aor, *AudioOutputRegistrar::qmNew) {
            AudioOutputRegistrar *aor2 = AudioOutputRegistrar::qmNew->value(aor->name);
            QList<audioDevice> ql = aor2->getDeviceChoices();
            foreach(audioDevice d, ql) {
                devicesList += QString("<RenderDevice><Device>%1</Device></RenderDevice>").arg(d.first);
            }
        }
    }
    devicesList += QString("<RenderDevice><Device>%1</Device></RenderDevice>").arg(AudioOutputRegistrar::current);
    devicesList += QString("</RenderDevices><CurrentRenderDevice><Device>%1</Device></CurrentRenderDevice>").arg(AudioOutputRegistrar::current);
    //QString devicesHardCoded = "<RenderDevices><RenderDevice><Device>Generic Hardware on Realtek HD Audio output</Device></RenderDevice><RenderDevice><Device>Realtek HD Audio output</Device></RenderDevice></RenderDevices><CurrentRenderDevice><Device>Realtek HD Audio output</Device></CurrentRenderDevice>";
    pWriter->writeText(devicesList);
    return 0;
}

// ----------------------
// Aux.SetCaptureDevice.1
// ----------------------

int AuxSetCaptureDevice1::parse(QXmlStreamReader& rR) {
    WDEF;

    WASSERT(rR.isStartElement() && rR.qualifiedName() == "CaptureDeviceSpecifier");
    rR.readNext();
    //WASSERT(rR.isCharacters());
    this->deviceName = rR.text().toString();
    WDEBUG2("CaptureDevice found: %s", this->deviceName.toAscii().data());

    WEND:
    return WCODE;
}

int AuxSetCaptureDevice1::execute() {
    return 0;
}

int AuxSetCaptureDevice1::response(XmlWriter* pWriter) {
    WDEBUG1("Writing AuxSetCaptureDevice1 response");
    if (AudioInputRegistrar::qmNew) {
        bool flag;
        WDEBUG1("Entering...");
        foreach(AudioInputRegistrar *air, *AudioInputRegistrar::qmNew) {
            AudioInputRegistrar *air2 = AudioInputRegistrar::qmNew->value(air->name);
            QList<audioDevice> ql = air2->getDeviceChoices();
            foreach(audioDevice d, ql) {
                if(QString(d.first) == this->deviceName) {
                    air->setDeviceChoice(QVariant(d.first), g.s);
                    WDEBUG2("AuxSetCaptureDevice1 %s has been set", d.first.toAscii().data());
                    flag = true;
                }
            }
            if(!flag) {
                WDEBUG2("%s not found.", this->deviceName.toAscii().data());
            }
        }
    }
    setStatus(QByteArray("0"), QByteArray(""), true);
    return 0;
}

// ----------------------
// Aux.SetRenderDevice.1
// ----------------------

int AuxSetRenderDevice1::parse(QXmlStreamReader& rR) {
    WDEF;

    WASSERT(rR.isStartElement() && rR.qualifiedName() == "RenderDeviceSpecifier");
    rR.readNext();
    //WASSERT(rR.isCharacters());
    this->deviceName = rR.text().toString();
    WDEBUG2("RenderDevice found: %s", this->deviceName.toAscii().data());

    WEND:
    return WCODE;
}

int AuxSetRenderDevice1::execute() {
    return 0;
}

int AuxSetRenderDevice1::response(XmlWriter* pWriter) {
    WDEBUG1("Writing AuxSetRenderDevice1 response");
    if (AudioOutputRegistrar::qmNew) {
        bool flag;
        WDEBUG1("Entering...");
        foreach(AudioOutputRegistrar *aor, *AudioOutputRegistrar::qmNew) {
            AudioOutputRegistrar *aor2 = AudioOutputRegistrar::qmNew->value(aor->name);
            QList<audioDevice> ql = aor2->getDeviceChoices();
            foreach(audioDevice d, ql) {
                if(QString(d.first) == this->deviceName) {
                    aor->setDeviceChoice(QVariant(d.first), g.s);
                    WDEBUG1("AuxSetRenderDevice1 has been set");
                    flag = true;
                }
            }
            if(!flag) {
                WDEBUG2("Realtek not found.", this->deviceName.toAscii().data());
            }
        }
    }
    setStatus(QByteArray("0"), QByteArray(""), true);
    return 0;
}

// ----------------------
// Connector.MuteLocalSpeaker.1
// ----------------------

int ConnectorMuteLocalSpeaker1::parse(QXmlStreamReader& rR) {
    WDEF;
    rR.readNext();
    rR.readNext();
    rR.readNext();
    WASSERT(rR.isStartElement() && rR.qualifiedName() == "Value");
    rR.readNext();
    WASSERT(rR.isCharacters());
    this->newValue = rR.text().toString();
    WDEBUG2("New Value found: %s", this->newValue.toAscii().data());

    WEND:
    return WCODE;
}

int ConnectorMuteLocalSpeaker1::execute() {
    return 0;
}

int ConnectorMuteLocalSpeaker1::response(XmlWriter* pWriter) {
    WDEBUG1("Writing ConnectorMuteLocalSpeaker1 response");
    
    if(this->newValue == "true") {
		WDEBUG1("ConnectorMuteLocalSpeaker1: setting deaf to false");
        g.mw->qaAudioDeaf->setChecked(false);
        g.mw->qaAudioDeaf->trigger();
    } else {
		WDEBUG1("ConnectorMuteLocalSpeaker1: setting deaf to true");
        g.mw->qaAudioDeaf->setChecked(true);
        g.mw->qaAudioDeaf->trigger();

		// if mic was off: switch it off again
		if (rConn.getLocalMicMuted()) {
			WDEBUG1("ConnectorMuteLocalSpeaker1: switching mic off  to true");
			g.mw->qaAudioMute->setChecked(false);
			g.mw->qaAudioMute->trigger();
		}
    }

	setStatus(QByteArray("0"), QByteArray(""), true);
    return 0;
}


// ----------------------
// Connector.SetLocalSpeakerVolume.1
// ----------------------

int ConnectorSetLocalSpeakerVolume1::parse(QXmlStreamReader& rR) {
    WDEF;
    rR.readNext();
    rR.readNext();
    rR.readNext();
    WASSERT(rR.isStartElement() && rR.qualifiedName() == "Value");
    rR.readNext();
    WASSERT(rR.isCharacters());
	WDEBUG2("ConnectorSetLocalSpeakerVolume1: mute before setting volume is %s", g.mw->qaAudioMute->isChecked() ? "true" : "false");
    this->newVolume = rR.text().toString();
    WDEBUG2("New Volume found: %s", this->newVolume.toAscii().data());

	// if mic was off: switch it off again
	if (rConn.getLocalMicMuted()) {
		WDEBUG1("ConnectorMuteLocalSpeaker1: switching mic off  to true");
		g.mw->qaAudioMute->setChecked(false);
		g.mw->qaAudioMute->trigger();
	}
	WDEBUG2("ConnectorSetLocalSpeakerVolume1: mute after setting volume is %s", g.mw->qaAudioMute->isChecked() ? "true" : "false");

    WEND:
    return WCODE;
}

int ConnectorSetLocalSpeakerVolume1::execute() {
    return 0;
}

int ConnectorSetLocalSpeakerVolume1::response(XmlWriter* pWriter) {
    WDEBUG1("Writing ConnectorSetLocalSpeakerVolume1 response");
    //Hack to set new Volume.
    float v = (this->newVolume.toFloat() * 10.0f) / 1000.0f;
    g.mw->setVolume(v);
    /*if(g.s.fVolume) {
        float v = (this->newVolume.toFloat() * 20.0f) / 100.0f;
        if(v < 20.0f)
            g.s.fVolume = v / 10.0f;
    }*/
    setStatus(QByteArray("0"), QByteArray(""), true);

	WDEBUG2("Local speaker volume set to %f", v); 
    return 0;
}


// ----------------------
// Account.Login.1
// ----------------------
int AccountLogin1::parse(QXmlStreamReader& rR) {
	WDEF;

    //Loop on XML till AccountName and AccountPassword are found
    while(!rR.atEnd() && (rConn.getConnectorHandle().isEmpty() || rConn.getName().isEmpty() || rConn.getPwd().isEmpty()))
    {
        if(rR.name() == "ConnectorHandle")
            rConn.setConnectorHandle(rR.readElementText());
        else if(rR.name() == "AccountName")
            rConn.setName(rR.readElementText());
        else if(rR.name() == "AccountPassword")
            rConn.setPwd(rR.readElementText());
        rR.readNext();
    }

	return WCODE;
}


int AccountLogin1::execute() {

    //Creation of QUrl object
    QUrl url;
    url.setScheme(QLatin1String("mumble"));
    url.setHost(rConn.getHost());
    url.setPort((unsigned short) rConn.getPort().toUShort());
    url.setUserName(rConn.getName());
    url.setPassword(rConn.getPwd());
    url.addQueryItem(QLatin1String("version"), QLatin1String("1.2.0"));
    //Sends the signal for MainWindow (mw) to connect to the url
    if (url.isValid()) {
            OpenURLEvent *oue = new OpenURLEvent(url);
            qApp->postEvent(g.mw, oue);
        }
    //Debugging information - Prints the login info on whisper.log (sh = ServerHandler)
    if (g.sh && g.sh->isRunning()) {
        WDEBUG1("Login - Looking for connection settings...");
        QString host, uName, pass;
        unsigned short port;
        g.sh->getConnectionInfo(host, port, uName, pass);
        QByteArray hostBA = host.toLatin1();
        const char *c_str1 = hostBA.data();
        QByteArray uNameBA = uName.toLatin1();
        const char *c_str2 = uNameBA.data();
        QByteArray passBA = pass.toLatin1();
        const char *c_str3 = passBA.data();
        QString portQS = QString::number(port, 10);
        QByteArray portBA = portQS.toLatin1();
        const char *c_str4 = portBA.data();
        WDEBUG1(c_str1);
        WDEBUG1(c_str2);
        WDEBUG1(c_str3);
        WDEBUG1(c_str4);
    }
    setStatus(QByteArray("200"), QByteArray("OK"), true);
    return 0;
}

int AccountLogin1::response(XmlWriter* pWriter) {
    WDEBUG1("writing AccountLogin1 response");

    // for now give a hardcoded handle
    pWriter->writeStart("AccountHandle");
    pWriter->writeText(rConn.getAccountHandle().toAscii());
    pWriter->writeEnd("AccountHandle");

    pWriter->writeStart("NumberOfAliases");
    pWriter->writeText("0");
    pWriter->writeEnd("NumberOfAliases");

    return 0;
}

void AccountLogin1::postProcess() {
    {
        AccountLoginStateChangeEvent oEvent;
        oEvent.reset(QByteArray(rConn.getAccountHandle().toAscii()), QByteArray(""), QByteArray(""));
        oEvent.setState(QByteArray("1"));
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }
    {
        BuddyAndGroupListChangeEvent oEvent;
        oEvent.reset(QByteArray(rConn.getAccountHandle().toAscii()), QByteArray(""), QByteArray(""));
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }
}


// ------------------------
// Account.ListBlockRules.1
// ------------------------

int AccountListBlockRules1::response(XmlWriter* pWriter) {

    // for now give a hardcoded handle
    pWriter->writeEmpty("BlockRules");

    return 0;
}


// -----------------------------
// Account.ListAutoAcceptRules.1
// -----------------------------

int AccountListAutoAcceptRules1::response(XmlWriter* pW) {

    // for now give a hardcoded handle
    pW->writeEmpty("AutoAcceptRules");
    return 0;
}


// -----------------------------
// Account.GetSessionFonts.1
// -----------------------------

int AccountGetSessionFonts1::response(XmlWriter* pW) {

    // for now give a hardcoded handle
    pW->writeEmpty("SessionFonts");
    return 0;
}


// -----------------------------
// Session.Create.1
// -----------------------------

int SessionCreate1::parse(QXmlStreamReader& rR) {
    //QStringList sgHandleSL;
    WDEF;

    WASSERT(rR.isStartElement() && rR.qualifiedName() == "AccountHandle");
    rR.readNext();
    rR.readNext();
    rR.readNext();

    WASSERT(rR.isStartElement() && rR.qualifiedName() == "URI");
    rR.readNext();
    WASSERT(rR.isCharacters());
    rConn.setURI(rR.text().toString());
    WDEBUG2("URI found: %s", rConn.getURI().toAscii().data());

    WEND:
    return WCODE;
}

int SessionCreate1::execute() {

	// unmute server side in any case
    ClientUser *p = ClientUser::get(g.uiSession);
	if(p->isValid(g.uiSession)) {
        p->setMute(false);
		WDEBUG1("SessionCreate1::execute: server mute set to false");
	}

    if(rConn.getLocalMicMuted() == false) {
        WDEBUG1("LocalMicMuted equals false.");

		g.mw->qaAudioMute->setChecked(true);
		g.mw->qaAudioMute->trigger();
    }
    return 0;
}

int SessionCreate1::response(XmlWriter* pWriter) {

    pWriter->writeStart("SessionGroupHandle");
    pWriter->writeText(rConn.getSessionGroupHandle().toAscii());
    pWriter->writeEnd("SessionGroupHandle");

    pWriter->writeStart("SessionHandle");
    pWriter->writeText(rConn.getSessionHandle().toAscii());
    pWriter->writeEnd("SessionHandle");

    return 0;
}

void SessionCreate1::postProcess() {
    // send SessionGroupAddedEvent
    {
        SessionGroupAddedEvent oEvent;
        oEvent.reset(QByteArray(rConn.getAccountHandle().toAscii()), QByteArray(rConn.getSessionGroupHandle().toAscii()), QByteArray(rConn.getSessionHandle().toAscii()));
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
		rViewer.sendMessage(oEvent);
    }
    // send SessionAddedEvent
    {
        SessionAddedEvent oEvent;
        oEvent.reset(QByteArray(rConn.getAccountHandle().toAscii()), QByteArray(rConn.getSessionGroupHandle().toAscii()), QByteArray(rConn.getSessionHandle().toAscii()));
        oEvent.setUri(QByteArray(rConn.getURI().toAscii()));
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }
    // send SessionUpdatedEvent
    {
        SessionUpdatedEvent oEvent;
        oEvent.reset(QByteArray(rConn.getAccountHandle().toAscii()), QByteArray(rConn.getSessionGroupHandle().toAscii()), QByteArray(rConn.getSessionHandle().toAscii()));
        oEvent.setUri(QByteArray(rConn.getURI().toAscii()));
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }
    // send MediaStreamUpdatedEvent
    {
        MediaStreamUpdatedEvent oEvent;
        oEvent.reset(QByteArray(rConn.getAccountHandle().toAscii()), QByteArray(rConn.getSessionGroupHandle().toAscii()), QByteArray(rConn.getSessionHandle().toAscii()));
		oEvent.setState("2");
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }

    // Wait after creating avatars' sessions
    //ViewerHandler::vhSleep(300);  unclear what that was for

    //Initialize users list
    ClientUser *p = ClientUser::get(g.uiSession);
    if(p->isValid(g.uiSession)) {
        Channel *c = p->cChannel;

        //For each user into my channel
        foreach(User* user, c->qlUsers) {

            //Gets Users Information
            QString userName = user->qsName;
            QString userParticipantURI = "sip:" + userName + "@" + rConn.getHost();

            //Sends ParticipantAddedEvent
            {
                ParticipantAddedEvent oEvent;
                oEvent.reset(QByteArray(rConn.getAccountHandle().toAscii()), QByteArray(rConn.getSessionGroupHandle().toAscii()), QByteArray(rConn.getSessionHandle().toAscii()));
                oEvent.setParticipantUri(QByteArray(userParticipantURI.toAscii()));
                oEvent.setAccountName(QByteArray(userName.toAscii()));
                oEvent.compose();
                //ViewerHandler::instance().sendMessage(oEvent);
                rViewer.sendMessage(oEvent);
            }

            //Sends ParticipantUpdatedEvent
            {
                ParticipantUpdatedEvent oEvent;
                oEvent.reset(QByteArray(rConn.getAccountHandle().toAscii()), QByteArray(rConn.getSessionGroupHandle().toAscii()), QByteArray(rConn.getSessionHandle().toAscii()));
                oEvent.setParticipantUri(QByteArray(userParticipantURI.toAscii()));
                oEvent.setIsSpeaking(QByteArray("true"));
                oEvent.setVolume(QByteArray("0"));
                oEvent.setEnergy(QByteArray("0"));
                oEvent.compose();
                //ViewerHandler::instance().sendMessage(oEvent);
                rViewer.sendMessage(oEvent);
            }
        }
    }

}

int SessionSet3DPosition1::execute() {
    setStatus(QByteArray("200"), QByteArray("OK"), true);
    return 0;
}

// -----------------------------
// Session.MediaDisconnect.1
// -----------------------------

int SessionMediaDisconnect1::execute() {
    setStatus(QByteArray("0"), QByteArray(""), true);
    WDEBUG1("Forcing mute...");

	g.mw->qaAudioMute->setChecked(false);
	g.mw->qaAudioMute->trigger();

/*
    ClientUser *p = ClientUser::get(g.uiSession);
    if(p->isValid(g.uiSession))
        p->setMute(true);
*/
    return 0;
}

/*
int SessionMediaDisconnect1::response(XmlWriter *pWriter) {
    WDEBUG1("writing SessionMediaDisconnect1 response");

    return 0;
}
*/

void SessionMediaDisconnect1::postProcess() {
    // send MediaStreamUpdatedEvent
    {
        MediaStreamUpdatedEvent oEvent;
        oEvent.reset(QByteArray(rConn.getAccountHandle().toAscii()), QByteArray(rConn.getSessionGroupHandle().toAscii()), QByteArray(rConn.getSessionHandle().toAscii()));
		oEvent.setState("1");
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }
    // send SessionRemovedEvent
    {
        SessionRemovedEvent oEvent;
        oEvent.reset(QByteArray(rConn.getAccountHandle().toAscii()), QByteArray(rConn.getSessionGroupHandle().toAscii()), QByteArray(rConn.getSessionHandle().toAscii()));
        oEvent.setUri(QByteArray(rConn.getURI().toAscii()));
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }
}

// -----------------------------
// SessionGroup.Terminate.1
// -----------------------------

int SessionGroupTerminate1::execute() {
    setStatus(QByteArray("0"), QByteArray(""), true);
    return 0;
}

void SessionGroupTerminate1::postProcess() {
    // send SessionGroupRemovedEvent
    {
        SessionGroupRemovedEvent oEvent;
        oEvent.reset(QByteArray(rConn.getAccountHandle().toAscii()), QByteArray(rConn.getSessionGroupHandle().toAscii()), QByteArray(rConn.getSessionHandle().toAscii()));
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }
}

// -----------------------------
// Aux.CaptureAudioStart.1
// -----------------------------

int AuxCaptureAudioStart1::execute() {
    setStatus(QByteArray("0"), QByteArray(""), true);
    return 0;
}

int AuxCaptureAudioStart1::response(XmlWriter *pWriter) {
    WDEBUG1("writing AuxCaptureAudioStart1 response");
    pWriter->writeStart("ReturnCode");
    pWriter->writeText("0");
    pWriter->writeEnd("ReturnCode");
    return 0;
}

void AuxCaptureAudioStart1::postProcess() {
    // send AuxAudioPropertiesEvent
    {
        AuxAudioPropertiesEvent oEvent;
        oEvent.setMicIsActive(true);
        oEvent.setMicEnergy("0.04");
        oEvent.setMicVolume("44");
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }
}

// -----------------------------
// Aux.CaptureAudioStop.1
// -----------------------------

int AuxCaptureAudioStop1::execute() {
    setStatus(QByteArray("0"), QByteArray(""), true);
    return 0;
}

int AuxCaptureAudioStop1::response(XmlWriter *pWriter) {
    WDEBUG1("writing AuxCaptureAudioStop1 response");
    pWriter->writeStart("ReturnCode");
    pWriter->writeText("0");
    pWriter->writeEnd("ReturnCode");
    return 0;
}

// -----------------------------
// Aux.SetMicLevel.1
// -----------------------------

int AuxSetMicLevel1::execute() {
    setStatus(QByteArray("0"), QByteArray(""), true);
    return 0;
}

// -----------------------------
// Aux.SetSpeakerLevel.1
// -----------------------------

int AuxSetSpeakerLevel1::execute() {
    setStatus(QByteArray("0"), QByteArray(""), true);
    return 0;
}

// Callbacks from Mumble routed via ViewerHandler  CAREFUL: DIFFERENT THREAD!

/*
void Request::talkingChanged(const bool& isTalking, const QString userName) {
    //Sends message except when the local mic is muted and the user is local user.
    if(!(rConn.getLocalMicMuted() && (userName == rConn.getName()))) {
        QString userParticipantURI = "sip:" + userName + "@" + rConn.getHost();
        ParticipantUpdatedEvent oEvent;
        oEvent.reset(QByteArray(rConn.getAccountHandle().toAscii()), QByteArray(rConn.getSessionGroupHandle().toAscii()), QByteArray(rConn.getSessionHandle().toAscii()));
        oEvent.setParticipantUri(userParticipantURI.toAscii());
        oEvent.setIsSpeaking(isTalking ? QByteArray("true") : QByteArray("false"));
        oEvent.setVolume(QByteArray("30"));
        oEvent.setEnergy(QByteArray("0.5"));
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }
}

void Request::addParticipant(unsigned int id) {
    ClientUser *p = ClientUser::get(id);
    QString userName = p->qsName;
    QString userParticipantURI = "sip:" + userName + "@" + rConn.getHost();
    //Sends ParticipantAddedEvent
    {
        ParticipantAddedEvent oEvent;
        oEvent.reset(QByteArray(rConn.getAccountHandle().toAscii()), QByteArray(rConn.getSessionGroupHandle().toAscii()), QByteArray(rConn.getSessionHandle().toAscii()));
        oEvent.setParticipantUri(QByteArray(userParticipantURI.toAscii()));
        oEvent.setAccountName(QByteArray(userName.toAscii()));
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }

    //Sends ParticipantUpdatedEvent
    {
        ParticipantUpdatedEvent oEvent;
        oEvent.reset(QByteArray(rConn.getAccountHandle().toAscii()), QByteArray(rConn.getSessionGroupHandle().toAscii()), QByteArray(rConn.getSessionHandle().toAscii()));
        oEvent.setParticipantUri(QByteArray(userParticipantURI.toAscii()));
        oEvent.setIsSpeaking(QByteArray("true"));
        oEvent.setVolume(QByteArray("0"));
        oEvent.setEnergy(QByteArray("0"));
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }
}

void Request::removeParticipant(unsigned int id) {
    ClientUser *p = ClientUser::get(id);
    QString userName = p->qsName;
    QString userParticipantURI = "sip:" + userName + "@" + rConn.getHost();
    //Sends ParticipantRemovedEvent
    {
        ParticipantRemovedEvent oEvent;
        oEvent.reset(QByteArray(rConn.getAccountHandle().toAscii()), QByteArray(rConn.getSessionGroupHandle().toAscii()), QByteArray(rConn.getSessionHandle().toAscii()));
        oEvent.setParticipantUri(QByteArray(userParticipantURI.toAscii()));
        oEvent.setAccountName(QByteArray(userName.toAscii()));
        oEvent.compose();
        //ViewerHandler::instance().sendMessage(oEvent);
        rViewer.sendMessage(oEvent);
    }
}
*/
