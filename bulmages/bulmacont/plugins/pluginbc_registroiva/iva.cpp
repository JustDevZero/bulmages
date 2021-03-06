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

#ifdef TOMEU

#include "iva.h"
#include "bccompany.h"


///
/**
**/
void Iva::definetabla()
{
    BL_FUNC_DEBUG
    setDbTableName ( "iva" );
    setDbFieldId ( "idiva" );
    addDbField ( "idiva", BlDbField::DbInt, BlDbField::DbPrimaryKey, "Identificador" );
    addDbField ( "idtipoiva", BlDbField::DbInt, BlDbField::DbNotNull, "Id tipo IVA" );
    addDbField ( "idregistroiva", BlDbField::DbInt, BlDbField::DbNotNull, "Id registro IVA" );
    addDbField ( "baseiva", BlDbField::DbNumeric, BlDbField::DbNotNull, "Cantidad" );
    addDbField ( "ivaiva", BlDbField::DbNumeric, BlDbField::DbNotNull, "Precio linea presupuesto" );
    addDbField ( "idcuenta", BlDbField::DbInt, BlDbField::DbNoSave, "Id. cuenta" );
    addDbField ( "codigo", BlDbField::DbVarChar, BlDbField::DbNoSave, "Codigo" );
    addDbField ( "nombretipoiva", BlDbField::DbVarChar, BlDbField::DbNoSave, "Nombre tipo IVA" );
    
}


///
/**
\param comp
**/
Iva::Iva ( BcCompany *comp ) : BlDbRecord ( comp )
{
    BL_FUNC_DEBUG
    m_companyact = comp;
    definetabla();
    
}


///
/**
\param comp
\param idiva
**/
Iva::Iva ( BcCompany *comp, QString idiva ) : BlDbRecord ( comp )
{
    BL_FUNC_DEBUG
    m_companyact = comp;
    definetabla();
    QString SQLQuery = "SELECT * FROM iva LEFT JOIN tipoiva ON iva.idtipoiva = tipoiva.idtipoiva LEFT JOIN cuenta ON cuenta.idcuenta = tipoiva.idtipoiva WHERE idiva = " + idiva;
    BlDbRecordSet *cur = m_companyact->loadQuery ( SQLQuery );
    if ( !cur->eof() ) {
        DBload ( cur );
    } else {
        vaciaIva();
    } // end if
    
}


///
/**
\param comp
\param cur
**/
Iva::Iva ( BcCompany *comp, BlDbRecordSet *cur ) : BlDbRecord ( comp )
{
    BL_FUNC_DEBUG
    m_companyact = comp;
    definetabla();
    DBload ( cur );
    
}


///
/**
**/
Iva::~Iva()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void Iva::vaciaIva()
{
    BL_FUNC_DEBUG
    DBclear();
    
}


///
/**
\return
**/
int Iva::remove()
{
    BL_FUNC_DEBUG
    if ( dbValue ( "idiva" ) != "" ) {
        m_companyact->begin();
        int error = m_companyact->runQuery ( "DELETE FROM iva WHERE idiva = " + dbValue ( "idiva" ) );
        if ( error ) {
            m_companyact->rollback();
            return -1;
        } // end if
        m_companyact->commit();
        vaciaIva();
    } // end if
    
    return 0;
}


///
/**
\return
**/
void Iva::guardaIva()
{
    BL_FUNC_DEBUG
    QString id;
    m_companyact->begin();
    int error = dbSave ( id );
    if ( error ) {
        m_companyact->rollback();
        return;
    } // end if
    setDbValue ( "idiva", id );
    m_companyact->commit();
    
}


///
/**
\param val
**/
void Iva::setidtipoiva ( const QString &val )
{
    BL_FUNC_DEBUG
    QString SQLQuery = "SELECT * FROM tipoiva LEFT JOIN cuenta ON cuenta.idcuenta = tipoiva.idcuenta WHERE idtipoiva = " + val;
    BlDbRecordSet *cur = m_companyact->loadQuery ( SQLQuery );
    if ( !cur->eof() ) {
        setDbValue ( "idcuenta", cur->value( "idcuenta" ) );
        setDbValue ( "codigo", cur->value( "codigo" ) );
        setDbValue ( "nombretipoiva", cur->value( "nombretipoiva" ) );
    } // end if
    delete cur;
    setDbValue ( "idtipoiva", val );
    
}

#endif