/***************************************************************************
 *   Copyright (C) 2010 by Aron Galdon                                     *
 *   auryn@wanadoo.es                                                      *
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

#ifndef BLDATEDELEGATE_H
#define BLDATEDELEGATE_H

#include "bldatesearch.h"
#include <QItemDelegate>
//#include "tsystem.h"


/** Lista desplegable en campo de listado.
*/
class BlDateDelegate: public QItemDelegate
{
   public:
	BlDateDelegate ( BlMainCompany *comp, QObject *parent ) ;
	QWidget *createEditor ( QWidget *parent, const QStyleOptionViewItem &vis, const QModelIndex &index )  const;
	void setEditorData ( QWidget *editor, const QModelIndex &index )  const;
	void setModelData ( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index )  const;
	void updateEditorGeometry ( QWidget *editor, const QStyleOptionViewItem &vis, const QModelIndex &index )  const;

   private:
	BlMainCompany *m_company;
};

#endif