/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef PRESUPUESTOCONTABLE_H
#define PRESUPUESTOCONTABLE_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>

#include "ui_presupuestocontablebase.h"
#include "blpostgresqlclient.h"
#include "bldatesearch.h"
#include "blfixed.h"
#include "bfform.h"
#include "pdefs_pluginbf_minicontabilidad.h"


class BfCompany;


/// Muestra y administra la ventana con la informaci&oacute;n de un presupuesto.
/** */
class PLUGINBF_MINICONTABILIDAD_EXPORT PresupuestoContableView : public BfForm, public Ui_PresupuestoContableBase
{
    Q_OBJECT

private:
    BlDbRecordSet *m_cursorcombo;

public:
    PresupuestoContableView ( BfCompany *, QWidget * );
    ~PresupuestoContableView();
    virtual int cargarPost ( QString );
    virtual int afterSave();
    virtual int beforeDelete();
    void inicializar();

};

#endif

