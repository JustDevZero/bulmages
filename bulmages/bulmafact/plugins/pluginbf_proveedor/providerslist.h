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

#ifndef PROVIDERSLIST_H
#define PROVIDERSLIST_H

#include "pgimportfiles.h"
#include "subform2bf.h"
#include "listado.h"


/// Administra el listado de proveedores.
/** */
class ProveedorListSubform : public SubForm2Bf
{
    Q_OBJECT

public:
    ProveedorListSubform ( QWidget *parent = 0 );
    ~ProveedorListSubform() {}

public slots:
    virtual void cargar();
    virtual void cargar ( QString a );
};


#include "ui_providerslistbase.h"


/// Muestra y administra la ventana con el listado de proveedores.
/** */
class ProveedorList : public Listado, public Ui_ProveedorListBase, public pgimportfiles
{
    Q_OBJECT


private:
    QString m_idprovider;
    QString m_cifprovider;
    QString m_nomprovider;

public:
    ProveedorList ( Company *, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmode = EditMode );
    virtual ~ProveedorList();
    void presentar();
    void editar ( int );
    void imprimir();
    void crear();
    void borrar();
    QString idprovider();
    QString cifprovider();
    QString nomprovider();

public slots:
    virtual void on_mui_exportar_clicked();
    virtual void on_mui_importar_clicked();

signals:
    void selected ( QString );
};

#endif
