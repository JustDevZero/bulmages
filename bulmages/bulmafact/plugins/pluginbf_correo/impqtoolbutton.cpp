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

#include <QWidget>

#include "impqtoolbutton.h"
#include "blfunctions.h"
/// Necesarios para importacion de efactura
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>

#include "presupuestoview.h"
#include "blfixed.h"
#include "facturaview.h"
#include "bfcompany.h"
#include "bldb.h"

///
/**
\param pres
\param ped
\param alb
\param fac
\param parent
**/
EmailQToolButton::EmailQToolButton ( PresupuestoView *pres, PedidoClienteView *ped, AlbaranClienteView *alb, FacturaView *fac , QWidget *parent ) : QToolButton ( parent )
{
    _depura ( "EmailQToolButton::EmailQToolButton", 0 );
    m_presupuestoView = pres;
    m_pedidoClienteView = ped;
    m_albaranClienteView = alb;
    m_facturaView = fac;
    setBoton();
    _depura ( "END EmailQToolButton::EmailQToolButton", 0 );
}


///
/**
**/
EmailQToolButton::~EmailQToolButton()
{
    _depura ( "EmailQToolButton::~EmailQToolButton", 0 );
    _depura ( "END EmailQToolButton::~EmailQToolButton", 0 );
}


///
/**
**/
void EmailQToolButton::setBoton()
{
    _depura ( "EmailQToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( _ ( "Enviar por e-mail al cliente" ) );
    setToolTip ( _ ( "Enviar por e-mail al cliente" ) );
    setMinimumSize ( QSize ( 32, 32 ) );
    setMaximumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/mail-send.svg" ) ) );
    setIconSize ( QSize ( 22, 22 ) );
    _depura ( "END EmailQToolButton::setBoton", 0 );
}


///
/**
**/
void EmailQToolButton::click()
{
    _depura ( "ImpQToolButton::click", 0 );



    if ( m_presupuestoView != NULL ) {
        m_companyact = m_presupuestoView->mainCompany();


        QString id = m_presupuestoView->dbValue ( "idpresupuesto" );
        QString num = m_presupuestoView->dbValue ( "numpresupuesto" );
        QString ref = m_presupuestoView->dbValue ( "refpresupuesto" );
        QString idcliente = m_presupuestoView->dbValue ( "idcliente" );
        QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
        BlDbRecordSet *curs = m_companyact->loadQuery ( query );
        QString email = curs->valor ( "mailcliente" );
        delete curs;


        if ( m_presupuestoView->generaRML() ) {
            generaPDF ( "presupuesto" );

            QString cad = "mv " + g_confpr->valor ( CONF_DIR_USER ) + "presupuesto.pdf " + g_confpr->valor ( CONF_DIR_USER ) + "presupuesto" + num + ".pdf";
            system ( cad.toAscii().data() );

            cad = "kmail -s \"Presupuesto " + num + "\" --body \" Adjunto remito presupuesto numero " + num + ". Con referencia " + ref + "\n Atentamente\n\" --attach " + g_confpr->valor ( CONF_DIR_USER ) + "presupuesto" + num + ".pdf " + email;
            system ( cad.toAscii().data() );
        } // end if
    } // end if

    if ( m_pedidoClienteView != NULL ) {
        m_companyact = m_pedidoClienteView->mainCompany();


        QString id = m_pedidoClienteView->dbValue ( "idpedidocliente" );
        QString num = m_pedidoClienteView->dbValue ( "numpedidocliente" );
        QString ref = m_pedidoClienteView->dbValue ( "refpedidocliente" );

        QString idcliente = m_pedidoClienteView->dbValue ( "idcliente" );
        QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
        BlDbRecordSet *curs = m_companyact->loadQuery ( query );
        QString email = curs->valor ( "mailcliente" );
        delete curs;


        if ( m_pedidoClienteView->generaRML() ) {
            generaPDF ( "pedidocliente" );

            QString cad = "mv " + g_confpr->valor ( CONF_DIR_USER ) + "pedidocliente.pdf " + g_confpr->valor ( CONF_DIR_USER ) + "pedidocliente" + num + ".pdf";
            system ( cad.toAscii().data() );

            cad = "kmail -s \"Pedido " + num + "\" --body \" Adjunto remito pedido numero " + num + " con referencia   " + ref + "\n Atentamente\n\" --attach " + g_confpr->valor ( CONF_DIR_USER ) + "pedidocliente" + num + ".pdf " + email;
            system ( cad.toAscii().data() );
        } // end if
    } // end if


    if ( m_albaranClienteView != NULL ) {
        m_companyact = m_albaranClienteView->mainCompany();


        QString id = m_albaranClienteView->dbValue ( "idalbaran" );
        QString num = m_albaranClienteView->dbValue ( "numalbaran" );
        QString ref = m_albaranClienteView->dbValue ( "refalbaran" );

        QString idcliente = m_albaranClienteView->dbValue ( "idcliente" );
        QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
        BlDbRecordSet *curs = m_companyact->loadQuery ( query );
        QString email = curs->valor ( "mailcliente" );
        delete curs;


        if ( m_albaranClienteView->generaRML() ) {
            generaPDF ( "albaran" );

            QString cad = "mv " + g_confpr->valor ( CONF_DIR_USER ) + "albaran.pdf " + g_confpr->valor ( CONF_DIR_USER ) + "albaran" + num + ".pdf";
            system ( cad.toAscii().data() );

            cad = "kmail -s \"Pedido " + num + "\" --body \" Adjunto remito albaran numero " + num + " con referencia   " + ref + "\n Atentamente\n\" --attach " + g_confpr->valor ( CONF_DIR_USER ) + "albaran" + num + ".pdf " + email;
            system ( cad.toAscii().data() );
        } // end if
    } // end if


    if ( m_facturaView != NULL ) {
        m_companyact = m_facturaView->mainCompany();


        QString id = m_facturaView->dbValue ( "idfactura" );
        QString num = m_facturaView->dbValue ( "numfactura" );
        QString serie = m_facturaView->dbValue ( "codigoserie_factura" );
        QString ref = m_facturaView->dbValue ( "reffactura" );
        QString fecha = m_facturaView->dbValue ( "ffactura" );

        QString idcliente = m_facturaView->dbValue ( "idcliente" );
        QString query = "SELECT mailcliente from cliente WHERE idcliente=" + idcliente;
        BlDbRecordSet *curs = m_companyact->loadQuery ( query );
        QString email = curs->valor ( "mailcliente" );
        delete curs;

        if ( m_facturaView->generaRML() ) {
            generaPDF ( "factura" );

            QString cad = "mv " + g_confpr->valor ( CONF_DIR_USER ) + "factura.pdf " + g_confpr->valor ( CONF_DIR_USER ) + "factura" + serie + num + ".pdf";
            system ( cad.toAscii().data() );


            cad = "kmail -s \"Factura " + num + "\" --body \"Estimado cliente,\n\n";
            cad += "Adjunto le enviamos la factura numero " + serie + num + " con fecha " + fecha + "\n";
            cad += "Sin otro particular, reciba un cordial saludo:\n\n\n\"";
            cad += " --attach " + g_confpr->valor ( CONF_DIR_USER ) + "factura" + serie + num + ".pdf " + email;
            system ( cad.toAscii().data() );
        } // end if
    } // end if

    _depura ( "END ImpQToolButton::click", 0 );
}

