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

#include <QLineEdit>
#include <QTextEdit>
#include <QTableWidget>
#include <QComboBox>

#include "bldialogchanges.h"


///
/**
\param ob
**/
BlDialogChanges::BlDialogChanges ( QObject *ob )
{
    _depura ( "BlDialogChanges::BlDialogChanges", 0 );
    m_obje = ob;
    _depura ( "END BlDialogChanges::BlDialogChanges", 0 );
}


///
/**
**/
BlDialogChanges::~BlDialogChanges()
{
    _depura ( "BlDialogChanges::~BlDialogChanges", 0 );
    _depura ( "END BlDialogChanges::~BlDialogChanges", 0 );
}


///
/**
\param item
\return
**/
bool BlDialogChanges::objExcluido ( QObject *item )
{
    _depura ( "BlDialogChanges::objExcluido", 0 );
    bool excluido = FALSE;

    QListIterator<QObject *> it_excluidos ( m_listaExcluidos );
    /// Itera por los QObjects que estan excluidos de comprobacion de cambios.
    it_excluidos.toFront();
    while ( it_excluidos.hasNext() ) {
        /// Comparamos
        if ( it_excluidos.next() == ( QObject * ) item ) {
            excluido = TRUE;
        } // end if
    } // end while
    _depura ( "END BlDialogChanges::objExcluido", 0 );
    return excluido;
}


///
/**
**/
void BlDialogChanges::dialogChanges_cargaInicial()
{
    _depura ( "BlDialogChanges::dialogChanges_cargaInicial", 0 );
    try {
        m_maxQText = 0;
        m_maxQLine = 0;
        m_maxQTable = 0;
        m_maxQComboBox = 0;

        QListIterator<QObject *> it_excluidos ( m_listaExcluidos );

        QList<QComboBox *> l4 = m_obje->findChildren<QComboBox *>();
        QListIterator<QComboBox *> it4 ( l4 );
        while ( it4.hasNext() ) {
            QComboBox * item = it4.next();
            if ( item->objectName().startsWith ( "mui_" ) && !objExcluido ( item ) ) {
                m_listaQComboBox[m_maxQComboBox++] = item;
            } // end if
        } // end while


        QList<QTextEdit *> l1 = m_obje->findChildren<QTextEdit *>();
        QListIterator<QTextEdit *> it1 ( l1 );
        while ( it1.hasNext() ) {
            QTextEdit * item = it1.next();
            if ( item->objectName().startsWith ( "mui_" ) && !objExcluido ( item ) ) {
                m_listaQText[m_maxQText++] = item;
            } // end if
        } // end while

        QList<QLineEdit *> l2 = m_obje->findChildren<QLineEdit *>();
        QListIterator<QLineEdit *> it2 ( l2 );
        while ( it2.hasNext() ) {
            QLineEdit * item = it2.next();
            if ( item->objectName().startsWith ( "mui_" ) && !objExcluido ( item ) ) {
                m_listaQLine[m_maxQLine++] = item;
            } // end if
        } // end while

        QList<QTableWidget *> l3 = m_obje->findChildren<QTableWidget *>();
        QListIterator<QTableWidget *> it3 ( l3 );
        while ( it3.hasNext() ) {
            QTableWidget * item = it3.next();
            if ( item->objectName().startsWith ( "mui_" ) && !objExcluido ( item ) ) {
                m_listaQTable[m_maxQTable++] = item;
            } // end if
        } // end while

        m_valorinicial = calculateValues();
        _depura ( "END BlDialogChanges::dialogChanges_cargaInicial", 0, m_valorinicial.toAscii() );
    } catch ( ... ) {
        _depura ( "ERROR BlDialogChanges::dialogChanges_cargaInicial", 0, " error en el calculo" );
        return;
    } // end try
}


///
/**
\return
**/
bool BlDialogChanges::dialogChanges_hayCambios()
{
    _depura ( "BlDialogChanges::dialogChanges_hayCambios", 0 );
    QString valorfinal = calculateValues();
    _depura ( "END BlDialogChanges::dialogChanges_hayCambios", 0, m_valorinicial + "==== " + valorfinal );
    return ( m_valorinicial != valorfinal );
}


///
/**
\return
**/
QString BlDialogChanges::calculateValues()
{
    _depura ( "BlDialogChanges::calculateValues", 0 );
    QString values = retrieveValues ( "QTableWidget" );
    values += retrieveValues ( "QLineEdit" );
    values += retrieveValues ( "QTextEdit" );
    values += retrieveValues ( "QComboBox" );
    _depura ( "END BlDialogChanges::calculateValues", 0 );
    return values;
}


///
/**
\param objetoexcluido
**/
void BlDialogChanges::dialogChanges_setQObjectExcluido ( QObject *objetoexcluido )
{
    _depura ( "BlDialogChanges::dialogChanges_setQObjectExcluido", 0 );
    m_listaExcluidos.append ( objetoexcluido );
    _depura ( "END BlDialogChanges::dialogChanges_setQObjectExcluido", 0 );
}


///
/**
\param qsWidget
\return
**/
QString BlDialogChanges::retrieveValues ( QString qsWidget )
{
    _depura ( "BlDialogChanges::retrieveValues", 0, qsWidget );
    try {
        QString values = "";
        QListIterator<QObject *> it_excluidos ( m_listaExcluidos );


        if ( qsWidget == "QComboBox" ) {
            for ( int i = 0; i < m_maxQComboBox; i++ ) {
                if ( m_listaQComboBox[i] != NULL ) {
                    if ( ( ( QComboBox* ) m_listaQComboBox[i] ) ->objectName().startsWith ( "mui_" ) ) {
                        values += ( ( QComboBox* ) m_listaQComboBox[i] ) ->objectName();
                        values += ( ( QComboBox* ) m_listaQComboBox[i] ) ->currentText();
                    } // end if
                } // end if
            } // end for
        } // end if


        if ( qsWidget == "QLineEdit" ) {
            for ( int i = 0; i < m_maxQLine; i++ ) {
                if ( m_listaQLine[i] != NULL ) {
                    if ( ( ( QLineEdit* ) m_listaQLine[i] ) ->objectName().startsWith ( "mui_" ) ) {
                        values += ( ( QLineEdit* ) m_listaQLine[i] ) ->objectName();
                        values += ( ( QLineEdit* ) m_listaQLine[i] ) ->text();

                    } // end if
                } // end if
            } // end for
        } // end if

        if ( qsWidget == "QTextEdit" ) {
            for ( int i = 0; i < m_maxQText; i++ ) {
                if ( m_listaQText[i] != NULL ) {
                    if ( ( ( QTextEdit* ) m_listaQText[i] ) ->objectName().startsWith ( "mui_" ) )
                        values += ( ( QTextEdit* ) m_listaQText[i] ) ->objectName();
                    values += ( ( QTextEdit* ) m_listaQText[i] ) ->toPlainText();
                } // end if
            } // end for
        } // end if


        if ( qsWidget == "QTableWidget" ) {
            for ( int i = 0; i < m_maxQTable; i++ ) {
                if ( m_listaQTable[i] != NULL ) {
                    for ( int k = 0; k < ( ( QTableWidget* ) m_listaQTable[i] ) ->rowCount(); k++ ) {
                        for ( int l = 0; l < ( ( QTableWidget* ) m_listaQTable[i] ) ->columnCount(); l++ ) {
                            values += ( ( ( QTableWidget* ) m_listaQTable[i] ) ) ->objectName();
                            values += ( ( ( QTableWidget* ) m_listaQTable[i] ) ) ->item ( k, l ) ->text();
                        } // end for
                    } // end for
                } // end if
            } // end for
        } // end if

        _depura ( "END BlDialogChanges::retrieveValues", 0, values );
        return values;
    } catch ( ... ) {
        _depura ( "ERROR BlDialogChanges::retrieveValue", 2, "error en el calculo" );
        return "";
    } // end try
}
