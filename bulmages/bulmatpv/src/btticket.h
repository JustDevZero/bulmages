/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef BTTICKET_H
#define BTTICKET_H

#include <QWidget>
#include <QCloseEvent>
#include <QLabel>

#include "blfunctions.h"
#include "blwidget.h"
#include "bldb.h"
#include "blfixed.h"


class BT_EXPORT BtTicket :  public BlWidget, public BlDbRecord
{
    Q_OBJECT

private:
    QList<BlDbRecord *> *m_listaLineas;
    BlDbRecord *m_lineaActual;

public:
    virtual void pintar();

public:
    BtTicket ( BlMainCompany *emp = NULL, QWidget *parent = 0 );
    virtual ~BtTicket();
    BlDbRecord *agregarLinea();
    QList<BlDbRecord *> *listaLineas();
    /// Inserta o agrega cantidad de articulos al ticket
    /// El parametro nuevalinea indica que se inserte en nueva linea.
    BlDbRecord * insertarArticulo ( QString idArticulo, BlFixed cantidad = BlFixed ( "1" ), bool nuevaLinea = FALSE );
    void borrarArticulo ( BlDbRecord *linea, BlFixed cantidad = BlFixed ( "1" ) );
    void vaciarBtTicket();
    void subirPosArticulo ( BlDbRecord *linea, int filas = 1 );
    void bajarPosArticulo ( BlDbRecord *linea, int filas = 1 );
    void inicioPosBtTicket ( BlDbRecord * );
    void finPosBtTicket ( BlDbRecord * );
    BlDbRecord *lineaBtTicket ( int posicion );
    BlDbRecord *lineaActBtTicket();
    void setLineaActual ( BlDbRecord * );
    void setDescuentoGlobal ( BlFixed descuento );
    virtual void imprimir();
    virtual void abrircajon();
    int guardar();
    int cargar ( QString );
    void borrarLinea ( BlDbRecord *linea );

public slots:
    virtual void subir();
    virtual void bajar();
    virtual void agregarCantidad ( QString cantidad );
    virtual void ponerCantidad ( QString cantidad );
    virtual void ponerPrecio ( QString precio );
    virtual void insertarArticuloCodigo ( QString );
    virtual void insertarArticuloCodigoNL ( QString );
};

#endif
