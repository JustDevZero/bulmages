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

#include <cstdio>

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QTextCodec>
#include <QLocale>
#include <QInputDialog>

#include "pluginbc_asiento.h"
#include "bcasientoview.h"
#include "bccompany.h"
#include "bcasientolistview.h"

BcAsientoView *g_asiento;
BcAsientoListView *g_listasientos;



///
/**
**/
MyPluginAsiento::MyPluginAsiento()
{
    _depura ( "MyPluginAsiento::MyPluginAsiento", 0 );
    _depura ( "END MyPluginAsiento::MyPluginAsiento", 0 );
}


///
/**
**/
MyPluginAsiento::~MyPluginAsiento()
{
    _depura ( "MyPluginAsiento::~MyPluginAsiento", 0 );
    _depura ( "END MyPluginAsiento::~MyPluginAsiento", 0 );
}


///
/**
**/
void MyPluginAsiento::elslot()
{
    _depura ( "MyPluginAsiento::elslot", 0 );
    if (g_asiento == NULL) {
      g_asiento = new BcAsientoView ( ( BcCompany * ) mainCompany(), 0 );
      mainCompany() ->pWorkspace() -> addWindow ( g_asiento );
    } // end if
    g_asiento->hide();
    g_asiento->show();
    _depura ( "END MyPluginAsiento::elslot", 0 );
}


///
/**
**/
void MyPluginAsiento::elslot1()
{
    _depura ( "MyPluginAsiento::elslot1", 0 );
    if (g_listasientos == NULL) {
      g_listasientos = new BcAsientoListView ( ( BcCompany * ) mainCompany() );
      g_listasientos->presentar();
      mainCompany()-> pWorkspace() -> addWindow ( g_listasientos );
    } // end if
    g_listasientos->hide();
    g_listasientos->show();
    _depura ( "END MyPluginAsiento::elslot1", 0 );
}

/// Espaciar Asientos
/**
**/
void MyPluginAsiento::elslot2()
{
    _depura ( "MyPluginAsiento::elslot2", 0 );
    BlDbRecordSet *cur = NULL;
    try {
        cur = mainCompany()->loadQuery ( "SELECT abreasientos()" );
        g_main->statusBar() ->showMessage ( _ ( "Se han espaciado los asientos" ), 2000 );
        delete cur;
        _depura ( "END BcCompany::Abrirasientos", 0 );
    } catch ( ... ) {
        mensajeError ( "Ha habido un error al espaciar los asientos" );
        if ( cur ) delete cur;
        return;
    } // end try
    _depura ( "END MyPluginAsiento::elslot2", 0 );
}

/// Ordenar Asientos
/**
**/
void MyPluginAsiento::elslot3()
{
    _depura ( "MyPluginAsiento::elslot3", 0 );
    QString query = "SELECT reordenaasientosall()";
    BlDbRecordSet *cur = NULL;
    try {
        cur = mainCompany()->loadQuery ( query );
        g_main->statusBar() ->showMessage ( _ ( "Se han ordenado los asientos" ), 2000 );
        delete cur;
        _depura ( "END BcCompany::Ordenarasientos", 10 );
    } catch ( ... ) {
        mensajeError ( "Ha habido un error al ordenar los asientos" );
        if ( cur ) delete cur;
        return;
    }
    _depura ( "END MyPluginAsiento::elslot3", 0 );
}

/// Asiento de Apertura
/**
**/
void MyPluginAsiento::elslot4()
{
    _depura ( "MyPluginAsiento::elslot4", 0 );
    QString hoy = QDate::currentDate().toString ( "dd/MM/yyyy" );
    QString finicial = "01/01/" + hoy.right ( 4 );

    bool ok;
    QString text1 = QInputDialog::getText ( 0,
                                            _ ( "Fecha" ), _ ( "Fecha:" ), QLineEdit::Normal,
                                            hoy, &ok );
    if ( ok && !text1.isEmpty() ) {
        hoy = text1;
    } else {
        return;
    } // end if

    g_asiento->show();
    g_asiento->asiento_apertura ( hoy );
    _depura ( "END MyPluginAsiento::elslot4", 0 );
}

/// Asiento de Cierre
/**
**/
void MyPluginAsiento::elslot5()
{
    _depura ( "MyPluginAsiento::elslot5", 0 );
    QString hoy = QDate::currentDate().toString ( "dd/MM/yyyy" );
    QString finicial = "01/01/" + hoy.right ( 4 );

    bool ok;
    QString text = QInputDialog::getText ( 0,
                                           _ ( "Fecha inicial" ), _ ( "Fecha inicial ejercicio:" ), QLineEdit::Normal,
                                           finicial, &ok );
    if ( ok && !text.isEmpty() ) {
        finicial = text;
    } else {
        return;
    } // end if

    QString text1 = QInputDialog::getText ( 0,
                                            _ ( "Fecha final" ), _ ( "Fecha final ejercicio:" ), QLineEdit::Normal,
                                            hoy, &ok );
    if ( ok && !text1.isEmpty() ) {
        hoy = text1;
    } else {
        return;
    } // end if

    g_asiento->show();
    g_asiento->asiento_cierre ( finicial, hoy );
    _depura ( "END MyPluginAsiento::elslot5", 0 );
}

/// Asiento de Regularización
/**
**/
void MyPluginAsiento::elslot6()
{
    _depura ( "MyPluginAsiento::elslot6", 0 );


    QString hoy = QDate::currentDate().toString ( "dd/MM/yyyy" );
    QString finicial = "01/01/" + hoy.right ( 4 );

    bool ok;
    QString text = QInputDialog::getText ( 0,
                                           _ ( "Fecha inicial" ), _ ( "Fecha inicial regularizacion:" ), QLineEdit::Normal,
                                           finicial, &ok );
    if ( ok && !text.isEmpty() ) {
        finicial = text;
    } else {
        return;
    } // end if

    QString text1 = QInputDialog::getText ( 0,
                                            _ ( "Fecha final" ), _ ( "Fecha final regularizacion:" ), QLineEdit::Normal,
                                            hoy, &ok );
    if ( ok && !text1.isEmpty() ) {
        hoy = text1;
    } else {
        return;
    } // end if


    g_asiento->show();
    g_asiento->asiento_regularizacion ( finicial, hoy );


    _depura ( "END MyPluginAsiento::elslot6", 0 );
}

///
/**
\param bges
**/
void MyPluginAsiento::inicializa ( BcBulmaCont *bges )
{
    _depura ( "MyPluginAsiento::inicializa", 0 );

    /// Creamos el men&uacute;.
    setMainCompany ( bges->empresaactual() );

    /// Creamos la ventana de asientos que va a ser fija.
    g_asiento = new BcAsientoView ( ( BcCompany * ) mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addWindow ( g_asiento );

    m_bulmacont = bges;
    QMenu *pPluginMenu;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( _ ( "menuAsiento" ) );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( _ ( "&Asiento" ), bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuAsiento" ) );
    } // end if

    QAction *accion = new QAction ( _ ( "&Asiento Contable" ), 0 );
    accion->setStatusTip ( _ ( "Permite ver y modificar asientos" ) );
    accion->setWhatsThis ( _ ( "Podra disponer de la informacion del asiento" ) );
    accion->setIcon(QIcon(QString::fromUtf8(":/Images/book.png")));
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    pPluginMenu->addAction ( accion );
    bges->toolBar->addAction ( accion );


    QAction *accion2 = new QAction ( _ ( "&Lista de Asientos Contables" ), 0 );
    accion2->setStatusTip ( _ ( "Permite ver el listado de asientos" ) );
    accion2->setWhatsThis ( _ ( "Permite ver el listado de asientos" ) );
    accion2->setIcon(QIcon(QString::fromUtf8(":/Images/book-list.png")));
    connect ( accion2, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );
    pPluginMenu->addAction ( accion2 );
    bges->toolBar->addAction ( accion2 );

    pPluginMenu->addSeparator();

    QAction *accion3 = new QAction ( _ ( "&Espaciar Asientos Contables" ), 0 );
    accion3->setStatusTip ( _ ( "Ordena los asientos dejando espacios entre ellos" ) );
    accion3->setWhatsThis ( _ ( "Ordena los asientos dejando espacios entre ellos" ) );
    connect ( accion3, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );
    pPluginMenu->addAction ( accion3 );


    QAction *accion4 = new QAction ( _ ( "&Reordenar Asientos Contables" ), 0 );
    accion4->setStatusTip ( _ ( "Ordena los asientos" ) );
    accion4->setWhatsThis ( _ ( "Ordena los asientos" ) );
    connect ( accion4, SIGNAL ( activated() ), this, SLOT ( elslot3() ) );
    pPluginMenu->addAction ( accion4 );

    pPluginMenu->addSeparator();

    QAction *accion5 = new QAction ( _ ( "Asiento de a&pertura" ), 0 );
    accion5->setStatusTip ( _ ( "Asiento de apertura" ) );
    accion5->setWhatsThis ( _ ( "Asiento de apertura" ) );
    connect ( accion5, SIGNAL ( activated() ), this, SLOT ( elslot4() ) );
    pPluginMenu->addAction ( accion5 );

    QAction *accion6 = new QAction ( _ ( "Asiento de &cierre" ), 0 );
    accion6->setStatusTip ( _ ( "Asiento de cierre" ) );
    accion6->setWhatsThis ( _ ( "Asiento de cierre" ) );
    connect ( accion6, SIGNAL ( activated() ), this, SLOT ( elslot5() ) );
    pPluginMenu->addAction ( accion6 );

    QAction *accion7 = new QAction ( _ ( "Asiento de &regularizacion" ), 0 );
    accion7->setStatusTip ( _ ( "Asiento de regularizacion" ) );
    accion7->setWhatsThis ( _ ( "Asiento de regularizacion" ) );
    connect ( accion7, SIGNAL ( activated() ), this, SLOT ( elslot6() ) );
    pPluginMenu->addAction ( accion7 );

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->insertMenu ( bges->menuMaestro->menuAction(), pPluginMenu );

    _depura ( "END MyPluginAsiento::inicializa", 0 );
}



///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    _depura ( "entryPoint::entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbc_asiento", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_asiento = NULL;

    MyPluginAsiento *plug = new MyPluginAsiento();
    plug->inicializa ( bcont );
    _depura ( "END entryPoint::entryPoint", 0 );
    return 0;
}

/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewBcAsientoView ( BcCompany * )
{
    g_plugParams = g_asiento;
    return 1;
}
