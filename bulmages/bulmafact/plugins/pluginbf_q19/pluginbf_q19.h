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

#ifndef PLUGINBF_Q19_H
#define PLUGINBF_Q19_H

#include "cobroslist.h"
#include "facturaslist.h"
#include "reciboslist.h"
#include "bfbulmafact.h"
#include "pdefs_pluginbf_q19.h"


extern "C" PLUGINBF_Q19_EXPORT int CobrosList_CobrosList ( CobrosList * );
extern "C" PLUGINBF_Q19_EXPORT int RecibosList_RecibosList ( RecibosList * );
extern "C" PLUGINBF_Q19_EXPORT int FacturasList_FacturasList ( FacturasList * );
extern "C" PLUGINBF_Q19_EXPORT int entryPoint ( BfBulmaFact * );


#endif

