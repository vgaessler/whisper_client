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


#ifndef WGAMEHANDLER_H
#define WGAMEHANDLER_H

#include <QtNetwork>

#include "wMessage.h"

namespace whisper {

	// Abstract parent class for implementation of a game handler
	class GameHandler : public QThread {
	public:
		GameHandler(QObject *parent = 0) : QThread(parent) {bEnd = false; bInitialized = false;}
		virtual ~GameHandler() {}
		virtual void run() = 0;
		void end() {bEnd = true;}
		virtual void addParticipant(const unsigned int) = 0;
		virtual void removeParticipant(const unsigned int) = 0;
		virtual void setTalking(const unsigned int) = 0;
		virtual void talkingChanged(bool bTalking, QString& rUserName) = 0;
		virtual int  sendMessage(Message& rMsg) = 0;


	protected:
		void initialized() {bInitialized = true;}
		bool is_shutdown() {return bEnd;}
		bool is_active() {return !bEnd && bInitialized;}

	private:
		bool bEnd;
		bool bInitialized;
	};


	// Default game handler: does nothing
	class NullGameHandler : public GameHandler {
	public:
		NullGameHandler(QObject *parent = 0) : GameHandler(parent) {}
		virtual ~NullGameHandler() {}
		void run() {return;}
		void addParticipant(const unsigned int) {return;}
		void removeParticipant(const unsigned int) {return;}
		void setTalking(const unsigned int) {return;}

#pragma warning(disable : 4100)
		void talkingChanged(bool bTalking, QString& rUserName) {};
		int  sendMessage(Message& rMsg) {return 0;}
#pragma warning(default : 4100)
	};
}

#endif // WGAMEHANDLER_H
