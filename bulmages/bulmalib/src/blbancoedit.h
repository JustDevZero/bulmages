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

#ifndef BLBANCOEDIT_H
#define BLBANCOEDIT_H

#include <QLineEdit>
#include "blwidget.h"

#include "ui_blbancoeditbase.h"


class BLBancoEdit : public BLWidget, public Ui_BLBancoEditBase
{
    Q_OBJECT

public:
    BLBancoEdit ( QWidget *parent = 0 );
    ~BLBancoEdit();
    virtual void setText ( QString val );
    virtual void setValorCampo ( QString val );
    virtual QString text();
    virtual QString valorCampo();

public slots:
    virtual void s_cuentatextChanged ( const QString & );
    virtual void s_cuentalostFocus();
    virtual void s_returnPressed();
    virtual void selectAll();
    virtual void setFocus();

signals:
    void valueChanged ( QString );
    void returnPressed();
    void editingFinished();
};

#endif
