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

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#include "bfcompany.h"
#include "blconfiguration.h"
#include "blfunctions.h"
#include "plugins.h"
#include "pedidoclienteview.h"
#include "pedidosclientelist.h"


///
/**
\param parent
\param flag
\return
**/
PedidosClienteList::PedidosClienteList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    _depura ( "PedidosClienteList::PedidosClienteList", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "PedidosClienteList_PedidosClienteList", this );
    if ( res != 0 )
        return;
    m_idpedidocliente = "";
    setSubForm ( mui_list );
    hideBusqueda();
		/// Establecemos los parametros de busqueda del Cliente
    m_cliente->setLabel ( _( "Cliente:" ) );
	m_cliente->setTableName( "cliente" );
	m_cliente->m_valores["cifcliente"] = "";
	m_cliente->m_valores["nomcliente"] = "";
    iniciaForm();
    _depura ( "END PedidosClienteList::PedidosClienteList", 0 );
}


///
/**
\param comp
\param parent
\param flags
\return
**/
PedidosClienteList::PedidosClienteList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo ) : BlFormList ( comp, parent, flag, editmodo)
{
    _depura ( "PedidosClienteList::PedidosClienteList", 0 );
    setupUi ( this );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "PedidosClienteList_PedidosClienteList", this );
    if ( res != 0 )
        return;
    m_cliente->setMainCompany ( comp );
    m_articulo->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    setSubForm ( mui_list );
    iniciaForm();
    presentar();
    m_idpedidocliente = "";
		/// Establecemos los parametros de busqueda del Cliente
    m_cliente->setLabel ( _( "Cliente:" ) );
	m_cliente->setTableName( "cliente" );
	m_cliente->m_valores["cifcliente"] = "";
	m_cliente->m_valores["nomcliente"] = "";

    if(modoEdicion()) {
    	mainCompany() ->meteWindow ( windowTitle(), this );
    } // end if
    hideBusqueda();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "pedidocliente" );
    _depura ( "END PedidosClienteList::PedidosClienteList", 0 );
}


///
/**
\return
**/
void PedidosClienteList::iniciaForm()
{
    _depura ( "PedidosClienteList::iniciaForm" );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "PedidosClienteList_iniciaForm", this );
    if ( res != 0 )
        return;
    mui_procesada->insertItem ( 0, _( "Todos los pedidos" ) );
    mui_procesada->insertItem ( 1, _( "Pedidos procesados" ) );
    mui_procesada->insertItem ( 2, _( "Pedidos no procesados" ) );
	mui_procesada->setCurrentIndex(2);
    _depura ( "END PedidosClienteList::iniciaForm" );
}


///
/**
**/
PedidosClienteList::~PedidosClienteList()
{
    _depura ( "PedidosClienteList::~PedidosClienteList", 0 );
    _depura ( "END PedidosClienteList::~PedidosClienteList", 0 );
}


///
/**
**/
void PedidosClienteList::presentar()
{
    _depura ( "PedidosClienteList::presenta", 0 );
    /// Hacemos el listado y lo presentamos.
    mui_list->cargar ( "SELECT *, totalpedidocliente AS total, bimppedidocliente AS base, imppedidocliente AS impuestos FROM pedidocliente LEFT JOIN  cliente ON pedidocliente.idcliente = cliente.idcliente LEFT JOIN almacen ON pedidocliente.idalmacen=almacen.idalmacen WHERE 1 = 1 " + generarFiltro() );

    /// Hacemos el calculo del total.
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT SUM(totalpedidocliente) AS total FROM pedidocliente LEFT JOIN cliente ON pedidocliente.idcliente=cliente.idcliente LEFT JOIN almacen ON pedidocliente.idalmacen = almacen.idalmacen WHERE 1 = 1 " + generarFiltro() );
    /// Esta consulta podria resultar NULL por problemas de permisos y debe tratarse el caso.
    if ( cur ) {
        m_total->setText ( cur->valor ( "total" ) );
        delete cur;
    } // end if
    _depura ( "END PedidosClienteList::presenta", 0 );
}


///
/**
\return
**/
QString PedidosClienteList::generarFiltro()
{
    /// Tratamiento de los filtros.
    _depura ( "PedidosClienteList::generarFiltro", 0 );
    QString filtro = "";
    if ( m_filtro->text() != "" ) {
        filtro = " AND ( lower(descpedidocliente) LIKE lower('%" + m_filtro->text() + "%') ";
        filtro += " OR refpedidocliente LIKE '" + m_filtro->text() + "%' ";
        filtro += " OR lower(nomcliente) LIKE lower('%" + m_filtro->text() + "%')) ";
    } else {
        filtro = "";
    } // end if
    if ( m_cliente->id() != "" ) {
        filtro += " AND pedidocliente.idcliente = " + m_cliente->id();
    } // end if

    /// Tratamos los elementos procesados y no procesados.
    if ( mui_procesada->currentIndex() == 1 ) {
        /// Muestra solo las procesadas.
        filtro += " AND procesadopedidocliente";
    } else if ( mui_procesada->currentIndex() == 2 ) {
        /// Muestra solo las NO procesadas.
        filtro += " AND NOT procesadopedidocliente ";
    } // end if



    if ( m_articulo->idarticulo() != "" ) {
        filtro += " AND idpedidocliente IN (SELECT DISTINCT idpedidocliente FROM lpedidocliente WHERE idarticulo = '" + m_articulo->idarticulo() + "')";
    } // end if
    if ( m_fechain->text() != "" ) {
        filtro += " AND fechapedidocliente >= '" + m_fechain->text() + "' ";
    } // end if
    if ( m_fechafin->text() != "" ) {
        filtro += " AND fechapedidocliente <= '" + m_fechafin->text() + "' ";
    } // end if
    _depura ( "END PedidosClienteList::generarFiltro", 0 );
    return ( filtro );
}


///
/**
**/
void PedidosClienteList::crear()
{
    _depura ( "PedidosClienteList:crear", 0 );
    PedidoClienteView *prov = new PedidoClienteView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->m_pWorkspace->addWindow ( prov );
    prov->inicializar();
    prov->show();
    _depura ( "END PedidosClienteList:crear", 0 );
}


/// \TODO: BfCompany debe instanciar la clase y no hacerse asi como esta ahora.
/**
\param row
\return
**/
void PedidosClienteList::editar ( int row )
{
    _depura ( "ProveedorList::editar", 0 );
    try {
        m_idpedidocliente = mui_list->dbValue ( QString ( "idpedidocliente" ), row );
        if ( modoEdicion() ) {
            PedidoClienteView * prov = new PedidoClienteView ( ( BfCompany * ) mainCompany(), 0 );
            if ( prov->cargar ( m_idpedidocliente ) ) {
                delete prov;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addWindow ( prov );
            prov->show();
        } else {
            emit ( selected ( m_idpedidocliente ) );
        } // end if
    } catch ( ... ) {
        mensajeInfo ( _( "Error al cargar el pedido cliente" ), this );
    } // end try
    _depura ( "END ProveedorList::editar", 0 );
}


///
/**
**/
void PedidosClienteList::imprimir()
{
    _depura ( "PedidosClienteList::imprimir", 0 );
    mui_list->imprimirPDF ( _( "Pedidos de clientes" ) );
    _depura ( "END PedidosClienteList::imprimir", 0 );
}


///
/**
\return
**/
void PedidosClienteList::borrar()
{
    _depura ( "PedidosClienteList::borrar", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        mensajeInfo ( _( "Debe seleccionar una linea" ), this );
        return;
    } // end if
    try {
        m_idpedidocliente = mui_list->dbValue ( QString ( "idpedidocliente" ) );
        if ( modoEdicion() ) {
            PedidoClienteView * pcv = new PedidoClienteView(( BfCompany * ) mainCompany());
            if ( pcv->cargar ( m_idpedidocliente ) ) {
                throw - 1;
            } // end if
            pcv->on_mui_borrar_clicked();
            pcv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al borrar el pedido de cliente" ), this );
    } // end try
    _depura ( "END PedidosClienteList::borrar", 0 );
}


///
/**
\param comp
**/
void PedidosClienteList::setMainCompany ( BfCompany *comp )
{
    _depura ( "PedidosClienteList::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    m_cliente->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    _depura ( "END PedidosClienteList::setMainCompany", 0 );
}


///
/**
**/
QString PedidosClienteList::idpedidocliente()
{
    _depura ( "PedidosClienteList::idpedidocliente", 0 );
    _depura ( "END PedidosClienteList::idpedidocliente", 0 );
    return m_idpedidocliente;
}


///
/**
\param val
**/
void PedidosClienteList::setidcliente ( QString val )
{
    _depura ( "PedidosClienteList::setidcliente", 0 );
    m_cliente->setId ( val );
    _depura ( "END PedidosClienteList::setidcliente", 0 );
}

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
///
/**
\param parent
**/
PedidosClienteListSubform::PedidosClienteListSubform ( QWidget *parent, const char * ) : BfSubForm ( parent )
{
    _depura ( "PedidosClienteListSubform::PedidosClienteListSubform", 0 );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "PedidosClienteListSubform_PedidosClienteListSubform", this );
    if ( res != 0 )
        return;
    setDbTableName ( "pedidocliente" );
    setDbFieldId ( "idpedidocliente" );
    addSubFormHeader ( "idpedidocliente", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _( "ID pedido" ) );
    addSubFormHeader ( "numpedidocliente", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Pedido" ) );
    addSubFormHeader ( "fechapedidocliente", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Fecha" ) );
    addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Cliente" ) );
    addSubFormHeader ( "refpedidocliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Referencia" ) );
    addSubFormHeader ( "base", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Base Imponible" ) );
    addSubFormHeader ( "impuestos", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Impuestos" ) );
    addSubFormHeader ( "total", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Total" ) );
    addSubFormHeader ( "descpedidocliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Descripcion" ) );
    addSubFormHeader ( "contactpedidocliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Persona de contacto" ) );
    addSubFormHeader ( "telpedidocliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Telefono" ) );
    addSubFormHeader ( "comentpedidocliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Comentario" ) );
    addSubFormHeader ( "codigoalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Almacen" ) );
    addSubFormHeader ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "ID trabajador" ) );
    addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "ID cliente" ) );
    addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "ID almacen" ) );
    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END PedidosClienteListSubform::PedidosClienteListSubform", 0 );
}


///
/**
**/
void PedidosClienteListSubform::cargar()
{
    _depura ( "PedidosClienteListSubform::cargar", 0 );
    QString SQLQuery = "SELECT * FROM pedidocliente";
    BlSubForm::cargar ( SQLQuery );
    _depura ( "END PedidosClienteListSubform::cargar", 0 );
}


///
/**
\param query
**/
void PedidosClienteListSubform::cargar ( QString query )
{
    _depura ( "PedidosClienteListSubform::cargar", 0 );
    BlSubForm::cargar ( query );
    _depura ( "END PedidosClienteListSubform::cargar", 0 );
}

