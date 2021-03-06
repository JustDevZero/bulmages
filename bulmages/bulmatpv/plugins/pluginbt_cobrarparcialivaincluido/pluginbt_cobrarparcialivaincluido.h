/***************************************************************************
 *   Copyright (C) 2011 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef PLUGINBT_COBRARPARCIALIVAINCLUIDO_H
#define PLUGINBT_COBRARPARCIALIVAINCLUIDO_H

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtCore/QObject>

#include "btbulmatpv.h"
#include "blmaincompanypointer.h"
#include "cambio.h"
#include "blapplication.h"
#include "blpostgresqlclient.h"
#include "pdefs_pluginbt_cobrarparcialivaincluido.h"


extern "C" PLUGINBT_COBRARPARCIALIVAINCLUIDO_EXPORT int entryPoint ( BtBulmaTPV * );
extern "C" PLUGINBT_COBRARPARCIALIVAINCLUIDO_EXPORT int Cambio_ivainc_pre ( Cambio * );


class PluginBt_CobrarParcialIVAIncluido : public QObject, public BlMainCompanyPointer
{
    Q_OBJECT

public:
    BlPostgreSqlClient *dbConnection;

public:
    PluginBt_CobrarParcialIVAIncluido();
    ~PluginBt_CobrarParcialIVAIncluido();
    void inicializa( BtBulmaTPV *);

public slots:
    void cobrarParcial();
};


#endif

