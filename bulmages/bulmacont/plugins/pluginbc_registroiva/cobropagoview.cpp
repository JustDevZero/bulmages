/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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

#include <QComboBox>

#include "cobropagoview.h"
#include "blpostgresqlclient.h"
#include "bccompany.h"
#include "bcasientointeligenteview.h"
#include "bcbuscarcuenta.h"


/// Inicia los encabezados de la tabla y llama a la presentacion del listado.
/**
\param emp
\param parent
**/
cobropagoview::cobropagoview ( BcCompany *emp, QWidget *parent )
        : BlForm ( parent )
{
    _depura ( "cobropagoview::cobropagoview", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    m_companyact = emp;
    mui_listado->setMainCompany ( m_companyact );
    /// Inicializamos el listado.
    mui_listado->setDbTableName ( "prevcobro" );
    mui_listado->setDbFieldId ( "idprevcobro" );
    mui_listado->addSubFormHeader ( "idprevcobro", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite , _ ( "idprevcobro" ) );
    mui_listado->addSubFormHeader ( "fprevistaprevcobro", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNoWrite , _ ( "fprevistaprevcobro" ) );
    mui_listado->addSubFormHeader ( "fcobroprevcobro", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite , _ ( "fcobroprevcobro" ) );
    mui_listado->addSubFormHeader ( "idctacliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "idctacliente" ) );
    mui_listado->addSubFormHeader ( "idfpago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite , _ ( "idfpago" ) );
    mui_listado->addSubFormHeader ( "idcuenta", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "idcuenta" ) );
    mui_listado->addSubFormHeader ( "idasiento", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "idasiento" ) );
    mui_listado->addSubFormHeader ( "cantidadprevistaprevcobro", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "cantidadprevistaprevcobro" ) );
    mui_listado->addSubFormHeader ( "cantidadprevcobro", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "cantidadprevcobro" ) );
    mui_listado->addSubFormHeader ( "idregistroiva", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "idregistroiva" ) );
    mui_listado->addSubFormHeader ( "tipoprevcobro", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "tipoprevcobro" ) );
    mui_listado->addSubFormHeader ( "docprevcobro", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "docprevcobro" ) );
    mui_listado->setInsert ( FALSE );
    /// Dejamos de inicializar el listado.
    m_cuenta->setMainCompany ( emp );
    on_mui_actualizar_clicked();
    m_companyact->meteWindow ( windowTitle(), this );
    _depura ( "END cobropagoview::cobropagoview", 0 );
}


///
/**
**/
cobropagoview::~cobropagoview()
{
    _depura ( "cobropagoview::~cobropagoview", 0 );
    m_companyact->sacaWindow ( this );
    _depura ( "END cobropagoview::~cobropagoview", 0 );
}


/// SLOT que responde a la pulsacion del boton de actualizar.
/**
**/
void cobropagoview::on_mui_actualizar_clicked()
{
    _depura ( "cobropagoview::s_actualizar", 0 );

    /// Hacemos la presentacion con la nueva clase
    QString cadwhere = "";
    mui_listado->cargar ( "SELECT * FROM prevcobro "
                          " LEFT JOIN cuenta ON cuenta.idcuenta=prevcobro.idcuenta "
                          " LEFT JOIN (SELECT idcuenta AS idctacliente, codigo AS codigoctacliente, descripcion AS nomctacliente FROM cuenta) AS T1 ON t1.idctacliente = prevcobro.idctacliente "
                          " WHERE 1=1 " + cadwhere );
    s_recalculaSaldo();
    _depura ( "END cobropagoview::s_actualizar", 0 );
}


///
/**
**/
void cobropagoview::s_guardar()
{
    _depura ( "cobropagoview::s_guardar", 0 );
    mui_listado->guardar();
    _depura ( "END cobropagoview::s_guardar", 0 );
}


///
/**
**/
void cobropagoview::s_recalculaSaldo()
{
    _depura ( "s_recalculaSaldo()", 0 );
    BlFixed totalcobro ( "0" );
    BlFixed totalpago ( "0" );
    for ( int i = 0; i < mui_listado->rowCount(); i++ ) {
        BlDbSubFormRecord *rec = mui_listado->lineaat ( i );
        if ( rec ) {
            if ( rec->dbValue ( "tipoprevcobro" ) == "f" ) {
                totalcobro = totalcobro + BlFixed ( rec->dbValue ( "cantidadprevcobro" ) );
            } else {
                totalpago = totalpago + BlFixed ( rec->dbValue ( "cantidadprevcobro" ) );
            } // end if
        } // end if
    } // end for
    m_totalCobros->setText ( totalcobro.toQString() );
    m_totalPagos->setText ( totalpago.toQString() );
    _depura ( "END s_recalculaSaldo()", 0 );
}
