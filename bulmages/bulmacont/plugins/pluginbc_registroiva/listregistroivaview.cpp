/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include "listregistroivaview.h"
#include "bcregistroivaimprimirview.h"
#include "registroivaview.h"
#include "bccompany.h"
#include "asiento1view.h"

#include <QMenu>


///
/**
\param emp
\param parent
**/
ListRegistroIvaView::ListRegistroIvaView ( BcCompany * emp, QString, QWidget *parent )
        : QWidget ( parent )
{
    _depura ( "ListRegistroIvaView::ListRegistroIvaView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    m_companyact = emp;
    finicial->setText ( normalizafecha ( "01/01" ).toString ( "dd/MM/yyyy" ) );
    ffinal->setText ( normalizafecha ( "31/12" ).toString ( "dd/MM/yyyy" ) );
    emp->meteWindow ( windowTitle(), this );

    /// DEFINICIONES PARA LA TABLA DE IVA Soportado.
    mui_tablasoportado->setMainCompany ( emp );
    mui_tablasoportado->setDbTableName ( "registroiva" );
    mui_tablasoportado->setDbFieldId ( "idregistroiva" );
    mui_tablasoportado->addSubFormHeader ( "idregistroiva", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "Id" ) );
    mui_tablasoportado->addSubFormHeader ( "contrapartida", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite, _ ( "Concepto contable" ) );
    mui_tablasoportado->addSubFormHeader ( "baseimp", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "idcuenta" ) );
    mui_tablasoportado->addSubFormHeader ( "iva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "IVA" ) );
    mui_tablasoportado->addSubFormHeader ( "ffactura", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "Fecha de factura" ) );
    mui_tablasoportado->addSubFormHeader ( "femisionregistroiva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "descripcioncuenta" ) );
    mui_tablasoportado->addSubFormHeader ( "factura", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite, _ ( "descripcion" ) );
    mui_tablasoportado->addSubFormHeader ( "serieregistroiva", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite, _ ( "Debe" ) );
    mui_tablasoportado->addSubFormHeader ( "idborrador", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite, _ ( "Haber" ) );
    mui_tablasoportado->addSubFormHeader ( "incregistro", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "Contrapartida" ) );
    mui_tablasoportado->addSubFormHeader ( "regularizacion", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "Comentario" ) );
    mui_tablasoportado->addSubFormHeader ( "plan349", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "ID Canal" ) );
    mui_tablasoportado->addSubFormHeader ( "numorden", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "Conciliacion" ) );
    mui_tablasoportado->addSubFormHeader ( "cif", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "idc_coste" ) );
    mui_tablasoportado->addSubFormHeader ( "idfpago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "idapunte" ) );
    mui_tablasoportado->addSubFormHeader ( "factemitida", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "idtipoiva" ) );
    mui_tablasoportado->addSubFormHeader ( "rectificaaregistroiva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "orden" ) );
    mui_tablasoportado->addSubFormHeader ( "idasiento", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "idasiento" ) );
    mui_tablasoportado->setInsert ( FALSE );

    /// DEFINICIONES PARA LA TABLA DE IVA Repercutido.
    mui_tablarepercutido->setMainCompany ( emp );
    mui_tablarepercutido->setDbTableName ( "registroiva" );
    mui_tablarepercutido->setDbFieldId ( "idregistroiva" );
    mui_tablarepercutido->addSubFormHeader ( "idregistroiva", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "Id" ) );
    mui_tablarepercutido->addSubFormHeader ( "contrapartida", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite, _ ( "Concepto Contable" ) );
    mui_tablarepercutido->addSubFormHeader ( "baseimp", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "idcuenta" ) );
    mui_tablarepercutido->addSubFormHeader ( "iva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "codigo" ) );
    mui_tablarepercutido->addSubFormHeader ( "ffactura", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "tipocuenta" ) );
    mui_tablarepercutido->addSubFormHeader ( "femisionregistroiva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "descripcioncuenta" ) );
    mui_tablarepercutido->addSubFormHeader ( "factura", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "descripcion" ) );
    mui_tablarepercutido->addSubFormHeader ( "serieregistroiva", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Debe" ) );
    mui_tablarepercutido->addSubFormHeader ( "idborrador", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Haber" ) );
    mui_tablarepercutido->addSubFormHeader ( "incregistro", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Contrapartida" ) );
    mui_tablarepercutido->addSubFormHeader ( "regularizacion", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Comentario" ) );
    mui_tablarepercutido->addSubFormHeader ( "plan349", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "ID Canal" ) );
    mui_tablarepercutido->addSubFormHeader ( "numorden", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Conciliacion" ) );
    mui_tablarepercutido->addSubFormHeader ( "cif", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "idc_coste" ) );
    mui_tablarepercutido->addSubFormHeader ( "idfpago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "idapunte" ) );
    mui_tablarepercutido->addSubFormHeader ( "factemitida", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "idtipoiva" ) );
    mui_tablarepercutido->addSubFormHeader ( "rectificaaregistroiva", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "orden" ) );
    mui_tablarepercutido->addSubFormHeader ( "idasiento", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "idasiento" ) );
    mui_tablarepercutido->setInsert ( FALSE );

    /// Definiciones para la tabla de repercutido
    mui_totalRepercutido->setMainCompany ( emp );
    mui_totalRepercutido->setDbTableName ( "" );
    mui_totalRepercutido->setDbFieldId ( "" );
    mui_totalRepercutido->addSubFormHeader ( "nombretipoiva", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "nombretipoiva" ) );
    mui_totalRepercutido->addSubFormHeader ( "tivaiva", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite, _ ( "tivaiva" ) );
    mui_totalRepercutido->addSubFormHeader ( "tbaseiva", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite, _ ( "tbaseiva" ) );
    mui_totalRepercutido->setInsert ( FALSE );

    /// Definiciones para la tabla de soportado
    mui_totalSoportado->setMainCompany ( emp );
    mui_totalSoportado->setDbTableName ( "" );
    mui_totalSoportado->setDbFieldId ( "" );
    mui_totalSoportado->addSubFormHeader ( "nombretipoiva", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite, _ ( "nombretipoiva" ) );
    mui_totalSoportado->addSubFormHeader ( "tivaiva", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite, _ ( "tivaiva" ) );
    mui_totalSoportado->addSubFormHeader ( "tbaseiva", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite, _ ( "tbaseiva" ) );
    mui_totalSoportado->setInsert ( FALSE );

    /// Fin de las definiciones para la tabla de IVA.
    _depura ( "END ListRegistroIvaView::ListRegistroIvaView", 0 );
}

///
/**
**/
ListRegistroIvaView::~ListRegistroIvaView()
{
    _depura ( "ListRegistroIvaView::~ListRegistroIvaView", 0 );
    m_companyact->sacaWindow ( this );
    _depura ( "END ListRegistroIvaView::~ListRegistroIvaView", 0 );
}


/// Al hacer doble click sobre la tabla de ivas se accede al asiento
/// que tiene dicha entrada.
/**
**/
void ListRegistroIvaView::on_mui_tablasoportado_cellDoubleClicked ( int, int )
{
    _depura ( "ListRegistroIvaView::on_mui_tablasoportado_itemDoubleClicked", 0 );
    int idasiento;
    idasiento = mui_tablasoportado->dbValue ( "idasiento" ).toInt();
    m_companyact->intapuntsempresa() ->muestraasiento ( idasiento );
    m_companyact->intapuntsempresa() ->show();
    m_companyact->intapuntsempresa() ->setFocus();
    _depura ( "END ListRegistroIvaView::on_mui_tablasoportado_itemDoubleClicked", 0 );
}


/// Al hacer doble click sobre la tabla de ivas se accede al asiento
/// que tiene dicha entrada.
/**
**/
void ListRegistroIvaView::on_mui_tablarepercutido_cellDoubleClicked ( int, int )
{
    _depura ( "ListRegistroIvaView::on_mui_tablarepercutido_itemDoubleClicked", 0 );
    int idasiento;
    idasiento = mui_tablarepercutido->dbValue ( "idasiento" ).toInt();
    m_companyact->intapuntsempresa() ->muestraasiento ( idasiento );
    m_companyact->intapuntsempresa() ->show();
    m_companyact->intapuntsempresa() ->setFocus();
    _depura ( "END ListRegistroIvaView::on_mui_tablarepercutido_itemDoubleClicked", 0 );
}


///
/**
**/
void ListRegistroIvaView::boton_print()
{
    _depura ( "ListRegistroIvaView::boton_print", 0 );
    BcRegistroIVAImprimirView *print = new BcRegistroIVAImprimirView ( m_companyact, 0 );
    print->inicializa1 ( finicial->text(), ffinal->text() );
    print->exec();
    delete print;
    _depura ( "END ListRegistroIvaView::boton_print", 0 );
}


///
/**
**/
void ListRegistroIvaView::on_mui_actualizar_clicked()
{
    _depura ( "ListRegistroIvaView::on_mui_actualizar_clicked", 0 );
    inicializa();
    _depura ( "END ListRegistroIvaView::on_mui_actualizar_clicked", 0 );
}


///
/**
**/
void ListRegistroIvaView::inicializa()
{
    _depura ( "ListRegistroIvaView::inicializa", 0 );

    QString query;
    QString sbaseimp, siva;
    QString cbaseimp, civa, ctotal;
    BlDbRecordSet *cur;

    QString SQLQuery = "SELECT * FROM cuenta, tipoiva LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva, sum(ivaiva) AS tivaiva FROM iva  WHERE iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='" + finicial->text() + "' AND ffactura <='" + ffinal->text() + "' AND factemitida) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta";

    mui_totalRepercutido->cargar ( SQLQuery );


    SQLQuery = "SELECT * FROM cuenta, tipoiva  LEFT JOIN (SELECT idtipoiva, SUM(baseiva) AS tbaseiva, SUM(ivaiva) AS tivaiva FROM iva WHERE iva.idregistroiva IN (SELECT idregistroiva FROM registroiva WHERE ffactura >='" + finicial->text() + "' AND ffactura <='" + ffinal->text() + "' AND NOT factemitida) GROUP BY idtipoiva) AS dd ON dd.idtipoiva=tipoiva.idtipoiva WHERE tipoiva.idcuenta = cuenta.idcuenta";
    mui_totalSoportado->cargar ( SQLQuery );

    SQLQuery = "SELECT SUM(baseimp) AS tbaseimp, sum(iva) AS tbaseiva FROM registroiva WHERE factemitida AND ffactura >='" + finicial->text() + "' AND ffactura <='" + ffinal->text() + "'";
    cur = m_companyact->loadQuery ( SQLQuery );
    m_baseimps->setText ( cur->valor ( "tbaseimp" ) );
    m_ivas->setText ( cur->valor ( "tbaseiva" ) );
    delete cur;

    SQLQuery = "SELECT SUM(baseimp) AS tbaseimp, sum(iva) AS tbaseiva FROM registroiva WHERE NOT factemitida AND ffactura >='" + finicial->text() + "' AND ffactura <='" + ffinal->text() + "'";
    cur = m_companyact->loadQuery ( SQLQuery );
    m_baseimpr->setText ( cur->valor ( "tbaseimp" ) );
    m_ivar->setText ( cur->valor ( "tbaseiva" ) );
    delete cur;

    query.sprintf ( "SELECT *, (registroiva.baseimp + registroiva.iva) AS totalfactura FROM registroiva LEFT JOIN (SELECT  * FROM cuenta, borrador, asiento  WHERE cuenta.idcuenta = borrador.idcuenta AND asiento.idasiento = borrador.idasiento ) AS t1 ON t1.idborrador = registroiva.idborrador WHERE factemitida AND ffactura >= '%s' AND ffactura <= '%s' ", finicial->text().toAscii().constData(), ffinal->text().toAscii().constData() );
    mui_tablasoportado->cargar ( query );

    /// Hacemos el c&aacute;culo de los que no pertenecen a IVA soportado porque
    /// as&iacute; entran todos.
    query.sprintf ( "SELECT *, (registroiva.baseimp + registroiva.iva) AS totalfactura FROM registroiva LEFT JOIN (SELECT * FROM cuenta, borrador, asiento  WHERE cuenta.idcuenta = borrador.idcuenta AND asiento.idasiento = borrador.idasiento) AS t1 ON t1.idborrador = registroiva.idborrador WHERE NOT factemitida AND ffactura >= '%s' AND ffactura <= '%s'", finicial->text().toAscii().constData(), ffinal->text().toAscii().constData() );
    mui_tablarepercutido->cargar ( query );
    _depura ( "END ListRegistroIvaView::inicializa", 0 );
}


///
/**
\param menu
**/
void ListRegistroIvaView::on_mui_tablarepercutido_pintaMenu ( QMenu *menu )
{
    _depura ( "ListRegistroIvaView::on_mui_tablarepercutido_pintaMenu", 0 );
    m_verreg = menu->addAction ( "Editar registro" );
    m_verasiento = menu->addAction ( "Asiento contable" );
    menu->addSeparator();
    _depura ( "END ListRegistroIvaView::on_mui_tablarepercutido_pintaMenu", 0 );
}


///
/**
\param menu
**/
void ListRegistroIvaView::on_mui_tablasoportado_pintaMenu ( QMenu *menu )
{
    _depura ( "ListRegistroIvaView::on_mui_tablasoportado_pintaMenu", 0 );
    m_verreg = menu->addAction ( "Editar registro" );
    m_verasiento = menu->addAction ( "Asiento contable" );
    menu->addSeparator();
    _depura ( "END ListRegistroIvaView::on_mui_tablasoportado_pintaMenu", 0 );
}


///
/**
\param ac
**/
void ListRegistroIvaView::on_mui_tablasoportado_trataMenu ( QAction *ac )
{
    _depura ( "ListRegistroIvaView::on_mui_tablasoportado_trataMenu", 0 );
    if ( m_verreg == ac ) {
        QString idborrador = mui_tablasoportado->dbValue ( "idborrador" );
        RegistroIvaView *nuevae = new RegistroIvaView ( m_companyact, 0 );
        nuevae->inicializa1 ( idborrador.toInt() );
        m_companyact->pWorkspace() ->addWindow ( nuevae );
        nuevae->show();
    } // end if
    if ( m_verasiento == ac ) {
        int idasiento;
        idasiento = mui_tablasoportado->dbValue ( "idasiento" ).toInt();
        m_companyact->intapuntsempresa() ->muestraasiento ( idasiento );
        m_companyact->intapuntsempresa() ->show();
        m_companyact->intapuntsempresa() ->setFocus();
    } // end if
    _depura ( "END ListRegistroIvaView::on_mui_tablasoportado_trataMenu", 0 );
}


///
/**
\param ac
**/
void ListRegistroIvaView::on_mui_tablarepercutido_trataMenu ( QAction *ac )
{
    _depura ( "ListRegistroIvaView::on_mui_tablarepercutido_trataMenu", 0 );
    if ( m_verreg == ac ) {
        QString idborrador = mui_tablarepercutido->dbValue ( "idborrador" );
        RegistroIvaView *nuevae = new RegistroIvaView ( m_companyact, 0 );
        nuevae->inicializa1 ( idborrador.toInt() );
        m_companyact->pWorkspace() ->addWindow ( nuevae );
        nuevae->show();
    } // end if
    if ( m_verasiento == ac ) {
        int idasiento;
        idasiento = mui_tablarepercutido->dbValue ( "idasiento" ).toInt();
        m_companyact->intapuntsempresa() ->muestraasiento ( idasiento );
        m_companyact->intapuntsempresa() ->show();
        m_companyact->intapuntsempresa() ->setFocus();
    } // end if
    _depura ( "END ListRegistroIvaView::on_mui_tablarepercutido_trataMenu", 0 );
}
