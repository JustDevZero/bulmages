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

#ifndef PLUGINBC_CANALYCCOSTE_H
#define PLUGINBC_CANALYCCOSTE_H

#include "bfbulmafact.h"
#include "blpostgresqlclient.h"
#include "bfcompany.h"
#include "blmaincompanypointer.h"
#include "pdefs_pluginbf_canalyccoste.h"
#include "blaction.h"

extern "C" PLUGINBC_CANALYCCOSTE_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBC_CANALYCCOSTE_EXPORT int BlAction_actionTriggered(BlAction *);
extern "C" PLUGINBC_CANALYCCOSTE_EXPORT int PgetSelCostes(QString &);
extern "C" PLUGINBC_CANALYCCOSTE_EXPORT int PgetSelCanales(QString &);
extern "C" PLUGINBC_CANALYCCOSTE_EXPORT int PgetSinCanal(bool &);
extern "C" PLUGINBC_CANALYCCOSTE_EXPORT int BlSubFormDelegate_createEditor(BlSubFormDelegate *);
extern "C" PLUGINBC_CANALYCCOSTE_EXPORT int BlSubFormDelegate_setModelData(BlSubFormDelegate *);
extern "C" PLUGINBC_CANALYCCOSTE_EXPORT int BlSubFormDelegate_setEditorData(BlSubFormDelegate *);
#endif

