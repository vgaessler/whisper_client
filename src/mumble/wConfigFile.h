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