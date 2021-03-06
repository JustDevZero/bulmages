/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#ifndef EFACTURAENVIO_H
#define EFACTURAENVIO_H

#include <QtWidgets/QWidget>

#include <ui_efacturaenviobase.h>
#include "bfcompany.h"


class EFacturaEnvio : public QWidget, public Ui_EFacturaEnvioBase
{
    Q_OBJECT

private:
    BfCompany *mainCompany();

public:
    EFacturaEnvio ( BfCompany *emp, QWidget *parent = 0 );
    ~EFacturaEnvio();

// public slots:
//  virtual void on_mui_examinaECertificado_clicked();
//  virtual void on_mui_cancelar_clicked();
//  virtual void on_mui_guardar_clicked();
};

#endif
