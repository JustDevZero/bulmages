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

#include <QWidget>

#include "bctipoivaview.h"
#include "bccompany.h"
#include "bcbuscarcuenta.h"


/// El constructor de la clase prepara las variables globales y llama a la funcion pintar.ç
/**
\param emp
\param parent
**/
BcTipoIVAView::BcTipoIVAView ( BcCompany *emp, QWidget *parent )
        : BcForm ( emp, parent )
{
    _depura ( "BcTipoIVAView::BcTipoIVAView", 0 );
    this->setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    setTitleName ( _ ( "Tipo IVA" ) );
    /// Establecemos cual es la tabla en la que basarse para los permisos
    setDbTableName ( "tipoiva" );
    setDbFieldId ( "idtipoiva" );
    addDbField ( "idtipoiva", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID tipo iva" ) );
    addDbField ( "nombretipoiva", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre Tipo" ) );
    addDbField ( "porcentajetipoiva", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "Porcentaje" ) );
    addDbField ( "idcuenta", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Cuenta" ) );

    mui_idcuenta->setMainCompany ( emp );
    m_curtipoiva = NULL;

    dialogChanges_setQObjectExcluido ( mui_comboTipoIVA );

    pintar();
    dialogChanges_cargaInicial();
    meteWindow ( windowTitle(), this );
    _depura ( "END BcTipoIVAView::BcTipoIVAView", 0 );
}


/// El destructor de la clase guarda los datos (por si ha habido cambios)
/// y libera la memoria que se haya ocupado.
/**
**/
BcTipoIVAView::~BcTipoIVAView()
{
    _depura ( "BcTipoIVAView::~BcTipoIVAView", 0 );
    if ( m_curtipoiva != NULL )
        delete m_curtipoiva;
    mainCompany() ->sacaWindow ( this );
    _depura ( "END BcTipoIVAView::~BcTipoIVAView", 0 );
}


/// Pinta la ventana, recarga el combo y si se pasa el parametro muestra el identificador
/// indicado.
/**
**/
void BcTipoIVAView::pintar ( QString idtipoiva )
{
    _depura ( "BcTipoIVAView::pintar", 0 );
    int posicion = 0;
    /// Vamos a inicializar el combo de los tipos de IVA.
    if ( m_curtipoiva != NULL )
        delete m_curtipoiva;
    QString query = "SELECT * from tipoiva left join cuenta ON tipoiva.idcuenta = cuenta.idcuenta ORDER BY nombretipoiva";
    m_curtipoiva = mainCompany() ->loadQuery ( query );
    mui_comboTipoIVA->clear();
    int i = 0;
    while ( !m_curtipoiva->eof() ) {
        mui_comboTipoIVA->insertItem ( i, m_curtipoiva->valor ( "nombretipoiva" ) );
        if ( idtipoiva == m_curtipoiva->valor ( "idtipoiva" ) )
            posicion = i;
        m_curtipoiva->nextRecord();
        i++;
    } // end while
    _depura ( "END BcTipoIVAView::pintar", 0 );
}


/// Esta funci&oacute;n muestra el tipo de IVA en la ventana.
/** \param pos, si es distinto de cero se busca en el combo la posici&oacute;n indicada
    sino se usa la posicion actual del combo. */
void BcTipoIVAView::mostrarplantilla ( int pos )
{
    _depura ( "BcTipoIVAView::mostrarplantilla", 0 );
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_hayCambios() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar tipo de IVA" ),
                                    _ ( "Desea guardar los cambios?" ),
                                    QMessageBox::Ok,
                                    QMessageBox::Cancel ) == QMessageBox::Ok )
            on_mui_guardar_clicked();
    } // end if
    if ( mui_comboTipoIVA->count() > 0 ) {
        if ( pos != 0 )
            mui_comboTipoIVA->setCurrentIndex ( pos );
        m_posactual = mui_comboTipoIVA->currentIndex();

        cargar ( m_curtipoiva->valor ( "idtipoiva", m_posactual ) );

        /// Comprobamos cual es la cadena inicial.
        dialogChanges_cargaInicial();
    } // end if
    _depura ( "END BcTipoIVAView::mostrarplantilla", 0 );
}


/// Esta funci&oacute;n sirve para hacer el cambio sobre un centro de coste .
/**
**/
void BcTipoIVAView::on_mui_comboTipoIVA_currentIndexChanged ( int )
{
    _depura ( "BcTipoIVAView::on_mui_comboTipoIVA_currentIndexChanged", 0 );
    mostrarplantilla();
    _depura ( "END BcTipoIVAView::on_mui_comboTipoIVA_currentIndexChanged", 0 );
}



/// SLOT que responde a la pulsaci&oacute;n del bot&oacute;n de nuevo tipo de IVA.
/** Inserta en la tabla de IVAs. */
/**
**/
void BcTipoIVAView::on_mui_crear_clicked()
{
    _depura ( "BcTipoIVAView::on_mui_nuevo2_clicked()", 0 );
    try {
        /// Si se ha modificado el contenido advertimos y guardamos.
        if ( dialogChanges_hayCambios() && m_curtipoiva->numregistros() != 0 ) {
            if ( QMessageBox::warning ( this,
                                        _ ( "Guardar tipo de IVA" ),
                                        _ ( "Desea guardar los cambios?" ),
                                        QMessageBox::Ok,
                                        QMessageBox::Cancel ) == QMessageBox::Ok )
                on_mui_guardar_clicked();
        } // end if

        if ( mui_idcuenta->text() == "" ) {
            mensajeInfo ( "Debe seleccionar una cuenta para asociarle el tipo de IVA" );
            return;
        } // end if

        QString nombreTipoIVA = "NUEVO TIPO IVA";
        if ( mui_nombretipoiva->text() == "" ) {
            nombreTipoIVA = mui_nombretipoiva->text();
        } // end if

        QString query = "INSERT INTO tipoiva (nombretipoiva, porcentajetipoiva, idcuenta) VALUES ('" + nombreTipoIVA + "', 0, id_cuenta('" + mui_idcuenta->text() + "'))";
        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT max(idtipoiva) AS idtipoiva FROM tipoiva" );
        mainCompany() ->commit();
        pintar ( cur->valor ( "idtipoiva" ) );
        delete cur;
    } catch ( ... ) {
        mainCompany() ->rollback();
        return;
    } // end try
    _depura ( "END BcTipoIVAView::on_mui_crear_clicked()", 0 );
}
