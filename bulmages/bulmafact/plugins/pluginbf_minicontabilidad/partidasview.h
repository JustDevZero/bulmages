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

#ifndef PARTIDASVIEW_H
#define PARTIDASVIEW_H

#include "blfunctions.h"
#include "ui_partidasbase.h"
#include "bfform.h"
#include "pdefs_pluginbf_minicontabilidad.h"


class BfCompany;


/// Muestra y administra la ventana de familias de art&iacute;culos.
/** */
class PLUGINBF_MINICONTABILIDAD_EXPORT PartidasView : public BfForm, public Ui_PartidasBase
{
    Q_OBJECT

private:
    /// Indica cu&aacute;l es el registro que se est&aacute; modificando. Sirve para saber los
    /// cambios que hay que guardar despu&eacute;s de que se haya modificado.
    QString m_idpartida;
    /// Indica si es modo consulta o modo edici&oacute;n. (modifica el comportamiento del
    /// doble click sobre la lista).
    bool m_selectMode;
    /// Este semaforo indica si se esta en el proceso de pintado.
    bool m_semaforoPintar;
    QTreeWidgetItem *m_init;

public:
    PartidasView ( BfCompany *, QWidget *parent = 0, bool selectMode = false );
    ~PartidasView();
    bool trataModificado();
    void mostrarplantilla();
    void setSelectMode();
    void setEditMode();
    QString codigoCompletoPartida();
    QString idPartida();
    QString nombrePartida();
    void pintar ( QTreeWidgetItem * );
    virtual int remove();
    virtual void on_mui_aceptar_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_imprimir_clicked();
    virtual int save();

public slots:
    virtual void pintar();
    virtual void on_m_listPartidas_itemDoubleClicked ( QTreeWidgetItem * );
    virtual void on_m_listPartidas_currentItemChanged ( QTreeWidgetItem *current, QTreeWidgetItem *previous );
    virtual void on_mui_crear_clicked();

signals:
    void selected ( QString );
};

#endif

