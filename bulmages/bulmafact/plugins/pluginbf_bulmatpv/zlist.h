/***************************************************************************
 *   Copyright (C) 2007 by Asociación Iglues                               *
 *   info@iglues.org                                                       *
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

#ifndef ZLIST_H
#define ZLIST_H

#include "bfcompany.h"
#include "bfsubform.h"
#include "blformlist.h"
#include "bulmafact.h"
#include "ui_zlistbase.h"


class myplugincont : public QObject
{
    Q_OBJECT

public:
    BlPostgreSqlClient *m_dbConnection;
    Bulmafact *m_bulmafact;

public:
    myplugincont();
    ~myplugincont();
    void inicializa ( Bulmafact * );

public slots:
    void elslot();
};



/** Esta clase implementa la pantalla de listado de Clientes.
    Deriva de Ficha para la estandarizacion de Pantallas.
    Tiene dos modos de funcionamiento (Edicion y Seleccion)
*/
class ZList : public BlFormList, public Ui_ZListBase
{
    Q_OBJECT

private:
    QString m_idz;

public:
    ZList ( BfCompany *comp = NULL, QWidget *parent = 0, Qt::WFlags flag = 0 );
    ~ZList();
    void presentar();
    void editar ( int );
    void borrar();
    void crear();
    QString idalmacen();
    void setMainCompany ( BfCompany *comp );
    void imprimir();
    QString generaFiltro();
    void iniciaForm();

signals:
    void selected ( QString );
};

#endif