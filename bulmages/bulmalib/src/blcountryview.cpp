/***************************************************************************
 *   Copyright (C) 2007 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include "blcountryview.h"
#include "blmaincompany.h"
#include "blsubform.h"


/// Esta clase se encarga de presentar los centros de coste, la ventana, y
/// de controlar la inserci&oacute;n de nuevos centros de coste, borrarlos, etc.
/**
\param emp
\param parent
**/
BlCountryView::BlCountryView ( BlMainCompany *emp, QWidget *parent )
        : BlForm ( emp, parent )
{
    _depura ( "BlCountryView::BlCountryView", 0 );

    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );

    mui_datospais->setDisabled ( TRUE );

    mui_list->setMainCompany ( emp );
    mui_listprovincias->setMainCompany ( emp );

    /// Preparamos la lista de paises.
    mui_list->setDbTableName ( "pais" );
    mui_list->setDbFieldId ( "idpais" );
    mui_list->addSubFormHeader ( "idpais", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _( "Id pais" ) );
    mui_list->addSubFormHeader ( "descpais", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Nombre pais" ) );
    mui_list->addSubFormHeader ( "cod2pais", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Codigo 2 digitos" ) );
    mui_list->addSubFormHeader ( "cod3pais", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _( "Codigo 3 digitos" ) );
    mui_list->setinsercion ( FALSE );
    mui_list->setDelete ( FALSE );
    mui_list->setSortingEnabled ( TRUE );

    /// Preparamos la lista de provincias.
    mui_listprovincias->setDbTableName ( "provincia" );
    mui_listprovincias->setDbFieldId ( "idprovincia" );
    mui_listprovincias->addSubFormHeader ( "idprovincia", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _( "Id provincia" ) );
    mui_listprovincias->addSubFormHeader ( "idpais", BlDbField::DbInt, BlDbField::DbNotNull , BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _( "Id pais" ) );
    mui_listprovincias->addSubFormHeader ( "provincia", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _( "Provincia" ) );
    mui_listprovincias->setinsercion ( TRUE );
    mui_listprovincias->setDelete ( TRUE );
    mui_listprovincias->setSortingEnabled ( FALSE );

    /// Establecemos cual es la tabla en la que basarse para los permisos
    setTitleName ( _( "Pais" ) );
    setDbTableName ( "pais" );
    setDbFieldId ( "idpais" );
    addDbField ( "idpais", BlDbField::DbInt, BlDbField::DbPrimaryKey, _( "Id pais" ) );
    addDbField ( "descpais", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Nombre pais" ) );
    addDbField ( "cod2pais", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Codigo 2 digitos" ) );
    addDbField ( "cod3pais", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Codigo 3 digitos" ) );

    m_countryId = "0";

    meteWindow ( windowTitle(), this );
    pintar();
    dialogChanges_cargaInicial();
    _depura ( "END BlCountryView::BlCountryView", 0 );
}


///
/**
**/
BlCountryView::~BlCountryView()
{
    _depura ( "BlCountryView::~BlCountryView", 0 );
    _depura ( "END BlCountryView::~BlCountryView", 0 );
}


///
/**
**/
void BlCountryView::pintar()
{
    _depura ( "BlCountryView::pintar", 0 );
    mui_list->cargar ( "SELECT * FROM pais ORDER BY descpais" );
    _depura ( "END BlCountryView::pintar", 0 );
}


///
/**
**/
void BlCountryView::on_mui_list_itemClicked ( QTableWidgetItem * )
{
    _depura ( "BlCountryView::on_mui_list_itemSelectionChanged", 0 );
    /// Busca el item correcto.
    QString previdpais = mui_list->dbValue ( "idpais" );

    if ( m_countryId != "0" && dialogChanges_hayCambios() ) {
        if ( QMessageBox::warning ( this,
                                    _( "Guardar pais" ),
                                    _( "Desea guardar los cambios?" ),
                                    _( "&Guardar" ), _( "&Cancelar" ), 0, 0, 1 ) == 0 ) {
            on_mui_guardar_clicked();
        } // end if
    } // end if
    m_countryId = previdpais;
    mostrarplantilla();
    _depura ( "END BlCountryView::on_mui_list_itemSelectionChanged", 0 );
}


///
/**
**/
void BlCountryView::mostrarplantilla()
{
    _depura ( "BlCountryView::mostrarplantilla", 0 );

    if ( m_countryId != "0" ) {
        mui_datospais->setEnabled ( TRUE );
        cargar ( m_countryId, FALSE );
        mui_descpais->setText ( dbValue ( "descpais" ) );
        mui_cod2pais->setText ( dbValue ( "cod2pais" ) );
        mui_cod3pais->setText ( dbValue ( "cod3pais" ) );

        mui_listprovincias->cargar ( "SELECT * FROM provincia WHERE idpais=" + m_countryId );

        dialogChanges_cargaInicial();
    } // end if

    _depura ( "END BlCountryView::mostrarplantilla", 0 );
}


///
/**
**/
int BlCountryView::guardar()
{
    _depura ( "BlCountryView::on_mui_guardar_clicked", 0 );
    QString id;

    if ( dbValue ( "idpais" ).isEmpty() ) {
        mensajeInfo ( _( "Seleccione un pais para guardar" ) );
    } else {
        setDbValue ( "descpais", mui_descpais->text() );
        setDbValue ( "cod2pais", mui_cod2pais->text() );
        setDbValue ( "cod3pais", mui_cod3pais->text() );
        mainCompany() ->begin();
        DBsave ( id );
        mui_listprovincias->setColumnValue ( "idpais", id );
        mui_listprovincias->guardar();
        mainCompany() ->commit();
        dialogChanges_cargaInicial();
        pintar();
    }
    _depura ( "END BlCountryView::on_mui_guardar_clicked", 0 );
    return 0;
}


///
/**
**/
void BlCountryView::on_mui_crear_clicked()
{
    _depura ( "BlCountryView::on_mui_crear_clicked", 0 );
    try {
        /// Si se ha modificado el contenido advertimos y guardamos.
        if ( dialogChanges_hayCambios() ) {
            if ( QMessageBox::warning ( this,
                                        _( "Guardar pais" ),
                                        _( "Desea guardar los cambios?" ),
                                        QMessageBox::Ok,
                                        QMessageBox::Cancel ) == QMessageBox::Ok ) {
                on_mui_guardar_clicked();
            } // end if
        } // end if

        QString query;
        query.sprintf ( "INSERT INTO pais (cod2pais, cod3pais, descpais) VALUES ('--', '---', 'Nuevo pais')" );
        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
        mainCompany() ->commit();
        pintar();
        mui_list->setCurrentItem ( mui_list->rowCount(), 1 );
        mostrarplantilla();
        _depura ( "END BlCountryView::on_mui_crear_clicked", 0 );
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el banco" ) );
        mainCompany() ->rollback();
    } // end try
}


///
/**
**/
void BlCountryView::on_mui_borrar_clicked()
{
    _depura ( "BlCountryView::on_mui_borrar_clicked", 0 );
    if ( m_countryId == "" | m_countryId == "0" ) {
        mensajeInfo ( _( "Debe seleccionar un elemento de la lista" ) );
        return;
    } // end if
    switch ( QMessageBox::warning ( this,
                                    _( "Borrar pais" ),
                                    _( "Se va a borrar el pais seleccionado.\nEsta operacion puede ocasionar perdida de datos." ),
                                    _( "&Borrar" ), _( "&Cancelar" ), 0, 0, 1 ) ) {
    case 0: /// Retry clicked or Enter pressed.
        QString query;
        try {
            query = "DELETE FROM pais WHERE idpais = " + m_countryId;
            mainCompany() ->begin();
            mainCompany() ->runQuery ( query );
            mainCompany() ->commit();
            m_countryId = "0";
            pintar();
            mui_datospais->setDisabled ( TRUE );
        } catch ( ... ) {
            mensajeInfo ( _( "Error al intentar borrar el pais" ) );
            mainCompany() ->rollback();
        } // end try
    } // end switch
    _depura ( "END BlCountryView::on_mui_borrar_clicked", 0 );
}

