/***************************************************************************
 *                                                                         *
 *   BulmaTPV remote client for Nintendo DS console.                       *
 *   -----------------------------------------------                       *
 *                                                                         *
 *   Copyright (C) 2010 by Fco. Javier M. C.                               *
 *   comercial@todo-redes.com                                              *
 *   http://www.todo-redes.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include "bndsticketslistview.h"


BndsTicketsListView::BndsTicketsListView() : BndsListUi()
{
	for (int i = 1; i <= 40; i++ ) {
	    char str [50];
	    sprintf(str,"%i", i);
	    
	    string t = string("MESA ") + string(str);
	    m_nombreTickets.push_back( t );
	} // end for
  
	/// Establece el numero de items a mostrar en la lista.
	setTotalItems( m_nombreTickets.size() );
}


BndsTicketsListView::~BndsTicketsListView()
{
}


void BndsTicketsListView::show()
{

	/// Inicia la consola.
	PrintConsole conSub = g_video->consoleSub();
  
	consoleSelect( &conSub );

	iprintf("\x1b[0;0H");
	iprintf("\x1b[2J");
	//printf("\x1b[0;0H%-15.15s - Total: %.2f", g_db->currentTicket()->nomTicket().c_str(), g_db->currentTicket()->total() );
	
	/// Borra las lineas de texto donde hay que escribir.
	int lineaInicio = 2;
	for (int i = lineaInicio; i < m_maxItemsPerPage + lineaInicio; i++) {
		iprintf("\x1b[%i;0H", i);
	        iprintf("\x1b[2K");
	} // end for

	/// Muestra la paginacion.
	iprintf("\x1b[23;0H");
	iprintf("\x1b[2K");
	iprintf("\x1b[23;0HPag: %i / %i - MESAS -", m_currentPage, m_totalPages);
	

	int primerElemento = (m_currentPage * m_maxItemsPerPage) - m_maxItemsPerPage;
	int ultimoElemento = primerElemento + m_maxItemsPerPage;
	int totalElementosPaginaActual = ((int) m_nombreTickets.size() - (m_maxItemsPerPage * (m_currentPage - 1))) >= m_maxItemsPerPage ? m_maxItemsPerPage : ((int) m_nombreTickets.size() - (m_maxItemsPerPage * (m_currentPage - 1)));
	
	/// Dibuja la plantilla tactil en la pantalla principal.
	g_video->showTemplateListA( totalElementosPaginaActual, 5, primerElemento );
	
	int contador = 1;
	for (list<string>::iterator itNombreTickets = m_nombreTickets.begin(); itNombreTickets != m_nombreTickets.end(); itNombreTickets++) {

	  /// Muestra los items en pantalla.
	  if ((contador > primerElemento) && (contador <= ultimoElemento) ) {

	    consoleSelect( &conSub );
	    iprintf("\x1b[%i;0H%2i %-21.21s", lineaInicio, lineaInicio - 1 + primerElemento, (*itNombreTickets).c_str() );

	    lineaInicio++;
	    
	  } // end if
	  
	  contador++;

	} // end for

}


int BndsTicketsListView::exec()
{
      /// Muestra la pantalla.
      show();
  
      while(1) {

	  swiWaitForVBlank();

	  scanKeys();
	  
	  if (keysDown() & KEY_LEFT) {
	      previousPage();
	  } else if (keysDown() & KEY_RIGHT) {
	      nextPage();
	  } // end if

	  /// Procesa los eventos de la pantalla tactil.
	  int itemSelected = g_video->eventTemplateListA();

	  if (itemSelected == -1) {
	      /// Necesita repintarse.
	      show();
	  } // end if

	  if (itemSelected == -2) {
	      previousPage();
	  } // end if

	  if (itemSelected == -3) {
	      nextPage();
	  } // end if

	  /// itemSelected = 0 => No se ha pulsado ningun recuadro. No se hace nada.
	  if (itemSelected > 0) {
	      /// Devuelve la posicion de la categoria dentro de la lista de categorias.
	      /// El primer elemento es el 1.
	      return itemSelected + ((m_currentPage - 1) * m_maxItemsPerPage);
	  } // end if

      } // end while

    return 0;
}


list<string> BndsTicketsListView::nombreTickets()
{
    return m_nombreTickets;
}


