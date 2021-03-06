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

#ifndef PLUGINBC_MAYOR_H
#define PLUGINBC_MAYOR_H

#include "bfbulmafact.h"
#include "blmaincompanypointer.h"
#include "blmaincompany.h"
#include "pdefs_pluginbf_mayor.h"
#include "blaction.h"
#include "blsubform.h"
#include "clienteview.h"
#include "bltoolbutton.h"
#include "provedit.h"


extern "C" PLUGINBC_MAYOR_EXPORT int entryPoint ( BfBulmaFact * );
extern "C" PLUGINBC_MAYOR_EXPORT int BlAction_actionTriggered(BlAction *);
extern "C" PLUGINBC_MAYOR_EXPORT int BlSubForm_preparaMenu ( BlSubForm * );
extern "C" PLUGINBC_MAYOR_EXPORT int BlSubForm_BlSubForm_Post ( BlSubForm * );
extern "C" PLUGINBC_MAYOR_EXPORT int ClienteView_ClienteView_Post ( ClienteView * );
extern "C" PLUGINBC_MAYOR_EXPORT int BlToolButton_released(BlToolButton *); 
extern "C" PLUGINBC_MAYOR_EXPORT int ProveedorView_ProveedorView_Post ( ProveedorView * );

#endif

