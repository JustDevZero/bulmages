/***************************************************************************
*   Copyright (C) 2009 by Tomeu Borras Riera                              *
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

#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QLabel>
#include <QDialog>
#include <QCheckBox>

#include "claseview.h"
#include "bfcompany.h"


/// Constructor de la clase inicializa la clase y llama a la clase de pintar para que pinte.
/**
\param emp
\param parent
\return
**/
ClaseView::ClaseView ( BfCompany *emp, QWidget *parent )
        : BfForm ( emp, parent )
{
    _depura ( "ClaseView::ClaseView", 0 );

    setTitleName ( _ ( "Clase" ) );
    setDbTableName ( "clase" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_tab->setDisabled ( TRUE );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ClaseView_ClaseView", this );
    if ( res != 0 ) {
        return;
    } // end if
    m_archivoimagen = "";
    setModoEdicion();
    m_cursorclasees = NULL;
    m_item = NULL;
    res = g_plugins->lanza ( "ClaseView_ClaseView_Post", this );
    if ( res != 0 ) {
        return;
    } // end if
    pintar();
    meteWindow ( windowTitle(), this, FALSE );
    _depura ( "END ClaseView::ClaseView", 0 );
}


///
/**
**/
void ClaseView::imprimir()
{
    _depura ( "ClaseView::imprimir", 0 );
    _depura ( "END ClaseView::imprimir", 0 );
}

/// Carga el query de la base de datos y carga el qlistview.
/**
**/
void ClaseView::pintar()
{
    _depura ( "ClaseView::pintar", 0 );

    mui_lista->clear();

    if ( m_cursorclasees != NULL ) {
        delete m_cursorclasees;
    } // end if
    m_cursorclasees = mainCompany() ->loadQuery ( "SELECT * FROM clase ORDER BY ascii(nomclase), nomclase" );
    while ( !m_cursorclasees->eof() ) {
        new QListWidgetItem ( m_cursorclasees->valor ( "nomclase" ), mui_lista );
        m_cursorclasees->nextRecord();
    } // end while

    /// Comprobamos cual es la cadena inicial.
    dialogChanges_cargaInicial();
    _depura ( "END ClaseView::pintar", 0 );
}


///
/**
**/
ClaseView::~ClaseView()
{
    _depura ( "ClaseView::~ClaseView", 0 );
    if ( m_cursorclasees != NULL ) {
        delete m_cursorclasees;
    } // end if
    _depura ( "END ClaseView::~ClaseView", 0 );
}


///
/**
\param cur
\return
**/
void ClaseView::on_mui_lista_currentItemChanged ( QListWidgetItem *cur, QListWidgetItem * )
{
    _depura ( "on_mui_lista_currentItemChanged", 0 );
    if ( !cur ) return;
    mui_tab->setEnabled ( TRUE );

    int row = mui_lista->row ( cur );
    trataModificado();
    mdb_idclase = m_cursorclasees->valor ( "idclase", row );
    m_nomclase->setText ( m_cursorclasees->valor ( "nomclase", row ) );

    m_item = cur;
    /// Comprobamos cual es la cadena inicial.
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ClaseView_on_mui_lista_currentItemChanged_Post", this );
    if ( res != 0 ) {
        return;
    } // end if

    dialogChanges_cargaInicial();

    _depura ( "END on_mui_lista_currentItemChanged", 0 );
}


///
/**
\return
**/
void ClaseView::on_mui_guardar_released()
{
    _depura ( "ClaseView::on_mui_guardar_released", 0 );


    try {
        /// Disparamos los plugins.
        int res = g_plugins->lanza ( "ClaseView_on_mui_guardar_released", this );
        if ( res != 0 ) {
            return;
        } // end if

        QString query = "UPDATE clase SET ";
        query += "  nomclase='" + mainCompany() ->sanearCadena ( m_nomclase->text() ) + "'";
        query += " WHERE idclase=" + mainCompany() ->sanearCadena ( mdb_idclase );

        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
        mainCompany() ->commit();

        /// Si hay un cursor en activo lo borramos para recargarlo
        pintar();


        /// Emitimos la senyal apropiada en el qapplication2
        g_theApp->tablaCambiada1 ( "clase" );

        /// Comprobamos cual es la cadena inicial.
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al guardar el clase" ) );
        mainCompany() ->rollback();
    } // end try


    _depura ( "END ClaseView::on_mui_guardar_released", 0 );
}


///
/**
\return
**/
bool ClaseView::trataModificado()
{
    _depura ( "ClaseView::trataModificado", 0 );
    /// Si se ha modificado el contenido advertimos y guardamos.
    if ( dialogChanges_hayCambios() ) {
        if ( QMessageBox::warning ( this,
                                    _ ( "Guardar datos del clase" ),
                                    _ ( "Desea guardar los cambios?" ),
                                    _ ( "&Si" ), _ ( "&No" ), 0, 0, 1 ) == 0 )
            on_mui_guardar_released();
        return ( TRUE );
    } // end if
    _depura ( "END ClaseView::trataModificado", 0 );
    return ( FALSE );
}


/// SLOT que responde a la pulsacion del boton de nuevo tipo de iva.
/// Inserta en la tabla de ivas
/**
\return
**/
void ClaseView::on_mui_nuevo_released()
{
    _depura ( "ClaseView::on_mui_nuevo_released", 0 );
    try {
        /// Si se ha modificado el contenido advertimos y guardamos.
        trataModificado();
        QString query = "INSERT INTO clase (nomclase) VALUES ('*** NUEVO TIPO DE ACTIVIDAD ***')";
        mainCompany() ->begin();
        mainCompany() ->runQuery ( query );
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT max(idclase) AS idclase FROM clase" );
        mainCompany() ->commit();
        mdb_idclase = cur->valor ( "idclase" );
        delete cur;
        pintar();
        _depura ( "END ClaseView::on_mui_nuevo_released", 0 );
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al crear un nuevo Clase" ) );
        mainCompany() ->rollback();
    } // end try
}


/// SLOT que responde a la pulsacion del boton de borrar la familia que se esta editando.
/// Lo que hace es que se hace un update de todos los campos.
/**
\return
**/
void ClaseView::on_mui_borrar_released()
{
    _depura ( "ClaseView::on_mui_borrar_released", 0 );
    try {
        mui_tab->setDisabled ( TRUE );
        trataModificado();
        mainCompany() ->begin();
        QString query = "DELETE FROM clase WHERE idclase = " + mdb_idclase;
        mainCompany() ->runQuery ( query );
        mainCompany() ->commit();
        mdb_idclase = "";
        pintar();
        _depura ( "END ClaseView::on_mui_borrar_released", 0 );
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al borrar el Clase" ) );
        mainCompany() ->rollback();
    }// end try
}




///
/**
\return
**/
QString ClaseView::idclase()
{
    _depura ( "ClaseView::idclase", 0 );
    _depura ( "END ClaseView::idclase", 0 );
    return mdb_idclase;
}


