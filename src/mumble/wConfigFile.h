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


#ifndef WCONFIGFILE_H
#define WCONFIGFILE_H

#pragma warning(disable : 4996)

#include <QtCore>

namespace whisper {

    class Parameter;

    class ConfigFile {
    public:
        const QString getValue(const QString& family, const QString& key);
        bool isValid() {return this->valid;}
        static ConfigFile& getInstance();
		static void init();

    protected:
        ConfigFile();

    private:
        static ConfigFile* cfInstance;
        QFile* cf;
        QList<Parameter> lcf;
        bool valid;
    };

    class Parameter {
    public:
        //Friendship
        friend class ConfigFile;

        //Constructors
        Parameter(const QString& family, const QString& key, const QString& value) {
            this->family = family;
            this->key = key;
            this->value = value;
        };
        Parameter() {};

        //Getters
        const QString getFamily() {return family;}
        const QString getKey() {return key;}
        const QString getValue() {return value;}

        //Methods
        bool isValid() { return !(family.isEmpty() || key.isEmpty() || value.isEmpty()); }

    private:
        QString family;
        QString key;
        QString value;
    };
}

#endif // WCONFIGFILE_H
