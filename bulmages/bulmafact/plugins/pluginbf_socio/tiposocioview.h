/***************************************************************************
 *   Copyright (C) 2009 by Xavier Rusiñol Escriu                           *
 *   russi7@gmail.com                                                      *
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

#ifndef TIPOSOCIOVIEW_H
#define TIPOSOCIOVIEW_H

#include "blfunctions.h"
#include <ui_tiposociobase.h>

#include "bfform.h"
#include "blpostgresqlclient.h"


/// Muestra y administra la ventana con la informaci&oacute;n de un tipo de socios.
/** */
class TiposocioView : public BfForm, private Ui_TiposocioBase
{
    Q_OBJECT

private:
    BlDbRecordSet *m_cursortiposocio;
    /// Indica cual es el objeto que se esta mostrando.
    QString mdb_idtiposocio;
    /// Indica en la lista de tiposocio cual es el item seleccionado.
    QListWidgetItem *m_item;

private:
    /// Se encarga de hacer la carga del query inicial y de mostrar la lista bien
    /// y presentar el elemento que se especifique.
    void pintar();
    virtual void imprimir();

public:
    TiposocioView ( BfCompany * emp, QWidget *parent = 0 );
    ~TiposocioView();
    bool trataModificado();
    virtual int save();
    virtual void on_mui_borrar_clicked();

private slots:
    virtual void on_mui_lista_currentItemChanged ( QListWidgetItem *cur, QListWidgetItem *prev );
    virtual void on_mui_nuevo_clicked();
};

#endif
