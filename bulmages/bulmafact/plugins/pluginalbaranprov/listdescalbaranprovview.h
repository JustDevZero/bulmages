/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef LISTDESCALBARANPROVVIEW_H
#define LISTDESCALBARANPROVVIEW_H

#include "subform2bf.h"
#include "company.h"


/// Muestra y administra la ventana de lista de descuentos por albar&aacute;n de proveedor.
/** */
class ListDescuentoAlbaranProvView : public SubForm2Bf
{
    Q_OBJECT

public:
    QString mdb_idalbaranp;
    ListDescuentoAlbaranProvView ( QWidget *parent = 0 );
    ~ListDescuentoAlbaranProvView() {}
    ;

public slots:
    virtual void cargar ( QString idalbaranp ) {
        _depura ( "ListDescuentoAlbaranProvView::cargar\n", 0 );
        mdb_idalbaranp = idalbaranp;
        SubForm3::cargar ( "SELECT * FROM dalbaranp WHERE idalbaranp = " + mdb_idalbaranp );
    };
};

#endif
