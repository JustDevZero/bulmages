/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#ifndef BLTABLEWIDGET_H
#define BLTABLEWIDGET_H

#include <QEvent>
#include <QTableWidget>
#include <QItemDelegate>

#include "blfunctions.h"


/// Reimplementa los items de BlTableWidget para que podamos programar cosas en ellos.
class BlTableWidgetItem : public QTableWidgetItem
{
public:
    int modo;
    BlTableWidgetItem ( int type = Type, int mode = 0 );
    BlTableWidgetItem ( const QString &text, int type = Type, int mode = 0 );
    ~BlTableWidgetItem();
    virtual bool operator< ( const QTableWidgetItem &other ) const;
};


/// Clase que deriva de QTableWidget para poder reprogramar ciertos aspectos y
/// funcionalidades.
/** Se usa principalmente como tabla para mostrar subformularios.*/
class BlTableWidget : public QTableWidget
{
    Q_OBJECT

private:
    int modo; /// Indica el modo de dibujo.
    int m_tipoorden;
    int m_colorden;

public:
    int m_teclasalida; /// Indica con que tecla se ha salido de la edicion.

public:
    int tipoorden();
    int colorden();
    void settipoorden ( int t );
    void setcolorden ( int t );
    void columnMoved ( int column, int oldIndex, int newIndex );
    BlTableWidget ( QWidget *parent = 0 );
    ~BlTableWidget();
    void editItem ( QTableWidgetItem *it );
    virtual bool eventFilter ( QObject *obj, QEvent *event );
    void setText ( int x, int y, const QString &val );
    virtual void ordenar();
    void sortByColumn ( int column, Qt::SortOrder order = Qt::AscendingOrder );
    void sortItems ( int column, Qt::SortOrder order = Qt::AscendingOrder );
    void setSortingEnabled ( bool );
    void showColumn(int);
    void hideColumn(int);

public slots:
    virtual void sitemChanged ( QTableWidgetItem *it );

signals:
    void cellRePosition ( int, int );
    void pulsadomas ( int, int, int );
    void pressedAsterisk ( int, int );
    void pressedPlus ( int, int );
    void pressedMinus ( int, int );
    void pressedSlash ( int, int );
    void ctrlSubir ( int, int );
    void ctrlBajar ( int, int );
    void ctrlDerecha ( int, int );
    void ctrlIzquierda ( int, int );
};

#endif
