# -*- coding: utf-8 -*-
import sys
import os
from config import *
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from listempresasbase import *
from empresa import Empresa
from modificarfacturacion import ModificarFacturacion
from modificarcontabilidad import ModificarContabilidad

class ListEmpresas(Ui_ListEmpresasBase, Empresa):
    def __init__(self, parent = None):
        Empresa.__init__(self,parent)
        self.setupUi(self)
        self.show()

        # Desabilitamos el Sorting para que en versiones previas se rellenen bien los campos
        self.mui_listado.setSortingEnabled(False)

        self.buscarEmpresas()
        self.mui_listado.resizeColumnsToContents()


        # Desabilitamos el Sorting para que en versiones previas se rellenen bien los campos
        self.mui_listado.setSortingEnabled(True)


    def buscarEmpresas(self):
        # Ponemos la pestanya de consola como la visible
        self.tabWidget.setCurrentIndex(1)
        # Conectamos a la base de datos template1 para despues sacar la lista de bases de datos
        self.conectar('template1')
        self.databases = self.execute("SELECT datname FROM pg_database ORDER BY datname")
        # Desconectamos de template1
        self.desconectar()
        self.mui_listado.setRowCount(0)
        self.i = 0

        for row in self.databases:
            if (str(row[0]) != 'template0' and str(row[0]) != 'template1' ):
                self.conectar(str(row[0]))
                existconf = self.executeone('SELECT * FROM pg_tables WHERE tablename=\'configuracion\'')
                if existconf != None:
                    nombre = self.executeone('SELECT valor FROM configuracion where nombre =\'NombreEmpresa\'')
                    tipo = self.executeone('SELECT valor FROM configuracion where nombre =\'Tipo\'')
                    databaserevision = self.executeone('SELECT valor FROM configuracion where nombre =\'DatabaseRevision\'')
                    if (nombre != None and tipo != None and databaserevision != None):
                        self.mui_listado.setRowCount(self.mui_listado.rowCount() + 1)
                        self.mui_listado.setItem(self.i, 0 , QTableWidgetItem(str(nombre[0]).decode('string-escape').decode("utf-8")))
                        self.mui_listado.setItem(self.i, 1 , QTableWidgetItem(row[0]))
                        self.mui_listado.setItem(self.i, 2 , QTableWidgetItem(str(tipo[0])))
                        self.mui_listado.setItem(self.i, 3 , QTableWidgetItem(str(databaserevision[0])))
                        self.mui_listado.showRow(self.i)
                        self.i = self.i + 1
                self.desconectar()

        # Ponemos la pestanya principal como la visible
        self.tabWidget.setCurrentIndex(0)


    def on_mui_listado_cellDoubleClicked(self, row, col):
        if (self.mui_listado.item(row,2).text() == QString('BulmaFact')):
            self.fact = ModificarFacturacion(str(self.mui_listado.item(row,1).text()))
            self.fact.exec_()
        if (self.mui_listado.item(row,2).text() == QString('BulmaCont')):
            self.cont = ModificarContabilidad(str(self.mui_listado.item(row,1).text()))
            self.cont.exec_()

    def on_mui_actualizar_released(self):
        Yes = 'Si'
        No = 'No'
        message = QtGui.QMessageBox(self)
        message.setText(QtGui.QApplication.translate("Empresa", "Desea actualizar todas las empresas. Esta operacion puede tardar un ratito largo", None, QtGui.QApplication.UnicodeUTF8))
        message.setWindowTitle('Atencion!')
        message.setIcon(QtGui.QMessageBox.Warning)
        message.addButton(Yes, QtGui.QMessageBox.AcceptRole)
        message.addButton(No, QtGui.QMessageBox.RejectRole)
        message.exec_()
        respuesta = message.clickedButton().text()
        if respuesta == Yes:
            print "Vamos a modificar todas y cada una de las empresas"
            i = 0;
            while (i < self.mui_listado.rowCount()):
                print "Tratando la empresa " + self.mui_listado.item(i,1).text()
                if (self.mui_listado.item(i,2).text() == QString('BulmaFact')):
                    self.fact = ModificarFacturacion(str(self.mui_listado.item(i,1).text()))
                    self.fact.mui_actualizarDB.setCheckState(Qt.Checked)
                    self.fact.show()
                    self.fact.on_mui_aceptar_released()
                    self.fact.close()
                    #self.fact.exec_()
                if (self.mui_listado.item(i,2).text() == QString('BulmaCont')):
                    self.cont = ModificarContabilidad(str(self.mui_listado.item(i,1).text()))
                    self.cont.mui_actualizarDB.setCheckState(Qt.Checked)
                    self.cont.show()
                    self.cont.on_mui_aceptar_released()
                    self.cont.close()
                    #self.cont.exec_()
                i = i + 1


def main(args):
    app=QtGui.QApplication(args)
    win=ListEmpresas()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)
