/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#include <QMessageBox>
#include <QMenu>
#include <QKeyEvent>
#include <QEvent>

#include "listdescpedidoclienteview.h"
#include "articulolist.h"
#include "comparticulolistview.h"
#include "funcaux.h"


///
/**
\param parent
**/
ListDescuentoPedidoClienteView::ListDescuentoPedidoClienteView ( QWidget *parent )
        : SubForm2Bf ( parent )
{
    _depura ( "ListDescuentoPedidoClienteView::ListDescuentoPedidoClienteView", 0 );
    setDBTableName ( "dpedidocliente" );
    setDBCampoId ( "iddpedidocliente" );
    addSHeader ( "iddpedidocliente", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr ( "Id descuento" ) );
    addSHeader ( "conceptdpedidocliente", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr ( "Concepto descuento" ) );
    addSHeader ( "proporciondpedidocliente", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr ( "% Descuento" ) );
    addSHeader ( "idpedidocliente", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView | SHeader::DBNoWrite, tr ( "Id pedido" ) );
    setinsercion ( TRUE );
    _depura ( "END ListDescuentoPedidoClienteView::ListDescuentoPedidoClienteView", 0 );
}

void ListDescuentoPedidoClienteView::cargar ( QString idpedidocliente )
{
    _depura ( "ListDescuentoPedidoClienteView::cargar", 0 );
    mdb_idpedidocliente = idpedidocliente;
    SubForm3::cargar ( "SELECT * FROM dpedidocliente WHERE idpedidocliente = " + mdb_idpedidocliente );
    _depura ( "END ListDescuentoPedidoClienteView::cargar", 0 );
}
