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

#include <stdio.h>

#include "pluginpedidoprov.h"
#include "company.h"
#include "funcaux.h"
#include "pedidoproveedorview.h"
#include "pedidosproveedorlist.h"



PedidosProveedorList *g_pedidosProveedorList=NULL;

///
/**
**/
mypluginpedp::mypluginpedp()
{
    _depura ( "mypluginpedp::mypluginpedp", 0 );
    _depura ( "END mypluginpedp::mypluginpedp", 0 );
}


///
/**
**/
mypluginpedp::~mypluginpedp()
{
    _depura ( "mypluginpedp::~mypluginpedp", 0 );
    _depura ( "END mypluginpedp::~mypluginpedp", 0 );
}


///
/**
**/
void mypluginpedp::elslot()
{
    _depura ( "mypluginpedp::elslot", 0 );
    if (g_pedidosProveedorList) {
	g_pedidosProveedorList->hide();
	g_pedidosProveedorList->show();
    }// end if
    _depura ( "END mypluginpedp::elslot", 0 );
}

///
/**
**/
void mypluginpedp::elslot1()
{
    _depura ( "mypluginpedp::elslot1", 0 );
        PedidoProveedorView * bud = new PedidoProveedorView((Company *)empresaBase(), NULL);
        empresaBase() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    _depura ( "END mypluginpedp::elslot1", 0 );
}



///
/**
\param bges
**/
void mypluginpedp::inicializa ( Bulmafact *bges )
{
    _depura ( "mypluginpedp::inicializa", 0 );

    if ( bges->getcompany()->has_table_privilege ( "pedidoproveedor", "SELECT" ) ) {

    /// Miramos si existe un menu Compras
    QMenu *pPluginMenu = NULL;
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( "menuCompras" );
    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
    	QMenu *pPluginMaestro = NULL;
	    pPluginMaestro = bges->menuBar() ->findChild<QMenu *> ( "menuMaestro" );
        pPluginMenu = new QMenu ( "&Compras", bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuCompras" ));
		bges->menuBar()->insertMenu(pPluginMaestro->menuAction(), pPluginMenu);
    } // end if

	/// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
	m_bges = bges;
	setEmpresaBase ( bges->getcompany() );
	QAction *planCuentas = new QAction ( tr ( "&Pedidos a Proveedores" ), 0 );
	planCuentas->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/supplier-purchase-order-list.svg" ) ));
	planCuentas->setStatusTip ( tr ( "Pedidos a Proveedores" ) );
	planCuentas->setWhatsThis ( tr ( "Pedidos a Proveedores" ) );
	pPluginMenu->addAction ( planCuentas );
	bges->Listados->addAction (planCuentas);
	connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

	QAction *npago = new QAction ( tr ( "&Nuevo Pedido a Proveedor" ), 0 );
	npago->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/supplier-purchase-order.svg" ) ));
	npago->setStatusTip ( tr ( "Nuevo Pedido a Proveedor" ) );
	npago->setWhatsThis ( tr ( "Nuevo Pedido a Proveedor" ) );
	pPluginMenu->addAction ( npago );
	bges->Fichas->addAction (npago);
	connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );


    }// end if
    _depura ( "END mypluginpedp::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de Entrada del plugin de Pedidos a Proveedor\n", 0 );
    mypluginpedp *plug = new mypluginpedp();
    plug->inicializa ( bges );
    return 0;
}


int Company_createMainWindows_Post(Company *comp) {
    if ( comp->has_table_privilege ( "pedidoproveedor", "SELECT" ) ) {
	g_pedidosProveedorList = new PedidosProveedorList( comp, NULL );	
	comp->m_pWorkspace->addWindow ( g_pedidosProveedorList );
	g_pedidosProveedorList->hide();
    }// end if
    return 0;
}


int ProveedorView_ProveedorView_Post (ProveedorView *prov) {
    if ( prov->empresaBase()->has_table_privilege ( "pedidoproveedor", "SELECT" ) ) {
	PedidosProveedorList *pedidosProveedorList = new PedidosProveedorList( (Company *)prov->empresaBase(), NULL );
	pedidosProveedorList->setObjectName("listPedidosProveedorList");
	pedidosProveedorList->hideBusqueda();
        prov->mui_tab->addTab ( pedidosProveedorList, "Pedidos" );
    }// end if
    return 0;
}

int ProveedorView_cargarPost_Post (ProveedorView *prov) {
    if ( prov->empresaBase()->has_table_privilege ( "pedidoproveedor", "SELECT" ) ) {
	PedidosProveedorList *pedidosProveedorList = prov->findChild<PedidosProveedorList *> ( "listPedidosProveedorList" );
        pedidosProveedorList->setidproveedor ( prov->DBvalue ( "idproveedor" ) );
        pedidosProveedorList->presentar();
    }// end if
    return 0;
}// end if


int BusquedaReferencia_on_mui_abrirtodo_clicked_Post (BusquedaReferencia *ref) {
    QString SQLQuery = "SELECT * FROM pedidoproveedor WHERE refpedidoproveedor = '" + ref->mui_referencia->text() + "'";
    cursor2 *cur = ref->empresaBase() ->cargacursor ( SQLQuery );
    while ( !cur->eof() ) {
        PedidoProveedorView * bud = new PedidoProveedorView((Company *)ref->empresaBase(), NULL);
        ref->empresaBase() ->m_pWorkspace->addWindow ( bud );
        bud->cargar ( cur->valor ( "idpedidoproveedor" ) );
        bud->show();
        cur->siguienteregistro();
    } // end while
    delete cur;
}// end if
