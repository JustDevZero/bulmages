# -*- coding: utf-8 -*-

import sys
import os
from PyQt4.QtGui import *
from PyQt4.QtCore import *
from modificarusuariobase import *
# Libreria de acceso a bases de datos PostgreSQL
import psycopg2

class ModificarUsuario(QtGui.QDialog, Ui_ModificarUsuario):

    def __init__(self, parent = None):
        QtGui.QDialog.__init__(self,parent)
        self.setupUi(self)
        self.process = QtCore.QProcess()
        self.initListaUsuarios()

    def initListaUsuarios(self):
        try:
            # Aqui puede verse que hace falta agregar al usuario root como
            # superusuario de bases de datos de postgres para que este script
            # funcione correctamente. Con el usuario postgres basta con poner:
            # createuser root
            conn = psycopg2.connect("dbname='template1' user='root'")
        except:
            print "Fallo en la conexion con PostgreSQL."
            sys.exit()

        cur = conn.cursor()
        
        try:
            cur.execute("SELECT * FROM pg_user")
        except:
            print "Fallo en la consulta para obtener los usuarios de PostgreSQL del sistema."
            sys.exit()
            
        usuarios = cur.fetchall()
        texto = ""
        
        # Rellenamos la lista con los usuarios de PostgreSQL
        for row in usuarios:
            texto = row[0]
            
            if (row[3] == True):
                texto = texto + "  (su)"
            
            self.listWidgetUser.addItem(QString(texto))
                        
        self.connect(self.listWidgetUser, SIGNAL("itemSelectionChanged()"), self.initListaDatabase)
        self.connect(self.checkBox_all, SIGNAL("stateChanged(int)"), self.checkBox_change)

    def initListaDatabase(self):
    
        # Vaciamos la lista, ya que esta se regenera cada vez que cambiamos la base de datos seleccionada
        self.listWidgetDatabase.clear()
        
        try:
            conn = psycopg2.connect("dbname='template1' user='root'")
        except:
            print "Fallo en la conexion con PostgreSQL."
            sys.exit()

        cur = conn.cursor()
        
        #Buscamos todas las bases de datos y las guardamos en un array.
        try:
            cur.execute("SELECT datname FROM pg_database ORDER BY datname")
        except:
            print "Fallo en la consulta para obtener las bases de datos de PostgreSQL."
            sys.exit()
            
        dbs = cur.fetchall()
                
        # Recorremos todas las bases de datos a partir del array
        for row in dbs:
        
            # Conectamos a cada base de datos excepto a template0 que no permite conexiones
            if str(row[0]) != 'template0':
                try:
                    conn2 = psycopg2.connect("dbname='" + str(row[0]) + "' user='root'")
                except:
                    print "Se produjo un error al intentar conectar con la base de datos " + str(row[0]) + " de PostgreSQL."
                    sys.exit()
                    
                cur = conn2.cursor()

                # Averiguamos el tipo de base de datos con la cual estamos tratando.
                try:
                    cur.execute("select valor from configuracion where nombre = 'Tipo'")
                    tipo = cur.fetchone()
                except:
                    continue
                    
                # Rellenamos la lista de bases de datos.
                if str(tipo) == "('BulmaFact',)":
                    texto = "BulmaFact  -  " + str(row[0])
            
                    self.listWidgetDatabase.addItem(QString(texto))
                    
                if str(tipo) == "('BulmaCont',)":
                    texto = "BulmaCont  -  " + str(row[0])
            
                    self.listWidgetDatabase.addItem(QString(texto))
                    
        self.connect(self.listWidgetDatabase, SIGNAL("itemSelectionChanged()"), self.capturaDatabase)
          
    def capturaDatabase(self):
        # Pasamos el nombre de la base de datos seleccionada en listWidgetDatabase a la variable database
        numero = self.listWidgetDatabase.count()
        temp = QtGui.QListWidgetItem()
        global dbase
        
        for x in range (numero):
                temp = self.listWidgetDatabase.item(x)
                
                if (temp.isSelected()):
                        dbase = temp.text()
                        break
                        
        if dbase.contains("BulmaFact  -  "):
            dbase.remove("BulmaFact  -  ")
            
        if dbase.contains("BulmaCont  -  "):
            dbase.remove("BulmaCont  -  ")

        self.initListaTablas()
              
    def initListaTablas(self):
        # Vaciamos la lista, ya que esta se regenera cada vez que cambiamos la base de datos seleccionada
        self.listWidgetTable.clear()

        try:
            conn = psycopg2.connect("dbname='" + str(dbase) + "' user='root'")
        except:
            print "Fallo en la conexion con PostgreSQL."
            sys.exit()

        cur = conn.cursor()
        
        #Buscamos todas las tablas de la base de datos seleccionada y las guardamos en un array.
        try:
            cur.execute("SELECT relname FROM pg_class WHERE relkind = 'r' AND relname NOT LIKE ('pg_%') AND relname NOT LIKE ('sql_%') ORDER BY relname")
        except:
            print "Fallo en la consulta para obtener tablas de PostgreSQL."
            sys.exit()
              
        tablas = cur.fetchall()
              
        # Rellenamos la lista con los usuarios de PostgreSQL
        for row in tablas:
            texto = row[0]
                        
            self.listWidgetTable.addItem(QString(texto))


    def checkBox_change(self):
        if (self.checkBox_all.isChecked()):
            self.checkBox_select.setCheckState(Qt.Checked)
            self.checkBox_insert.setCheckState(Qt.Checked)
            self.checkBox_update.setCheckState(Qt.Checked)
            self.checkBox_delete.setCheckState(Qt.Checked)
            self.checkBox_rule.setCheckState(Qt.Checked)
            self.checkBox_references.setCheckState(Qt.Checked)
            self.checkBox_trigger.setCheckState(Qt.Checked)

        else:
            self.checkBox_select.setCheckState(Qt.Unchecked)
            self.checkBox_insert.setCheckState(Qt.Unchecked)
            self.checkBox_update.setCheckState(Qt.Unchecked)
            self.checkBox_delete.setCheckState(Qt.Unchecked)
            self.checkBox_rule.setCheckState(Qt.Unchecked)
            self.checkBox_references.setCheckState(Qt.Unchecked)
            self.checkBox_trigger.setCheckState(Qt.Unchecked)

          
          
    def on_mui_guardar_released(self):
        #Conectamos con la base de datos
        try:
            conn = psycopg2.connect("dbname='" + str(dbase) + "' user='root'")
        except:
            print "Fallo en la conexion con PostgreSQL."
            sys.exit()
            
        cur = conn.cursor()
        
        # Pasamos el nombre del usuario seleccionado en listWidgetUser a la variable username
        numero = self.listWidgetUser.count()
        temp = QtGui.QListWidgetItem()
        
        for x in range (numero):
                temp = self.listWidgetUser.item(x)
                
                if (temp.isSelected()):
                        username = temp.text()
                        break
                        
        if username.contains("  (su)"):
            username.remove("  (su)")
        
        # Pasamos el nombre de la base de datos seleccionada en listWidgetDatabase a la variable database
        numero = self.listWidgetDatabase.count()
        
        for x in range (numero):
                temp = self.listWidgetDatabase.item(x)
                
                if (temp.isSelected()):
                        database = temp.text()
                        break
                        
        if database.contains("BulmaFact  -  "):
            database.remove("BulmaFact  -  ")
            
        if database.contains("BulmaCont  -  "):
            database.remove("BulmaCont  -  ")
            
        # Pasamos el nombre de la tabla seleccionada en listWidgetTable a la variable table
        numero = self.listWidgetTable.count()
        
        for x in range (numero):
                temp = self.listWidgetTable.item(x)
                
                if (temp.isSelected()):
                        table = temp.text()
                        break

        
        if (self.checkBox_dball.isChecked()):
            try:
                cur.execute("grant all on database " + str(database) + " to " + str(username) + ";")
            except:
                print "Se produjo un error al intentar cambiar los permisos (grant all) de las bases de datos de PostgreSQL."
                sys.exit()

        if (self.checkBox_dbrevoke.isChecked()):
            try:
                cur.execute("revoke all on database " + str(database) + " to " + str(username) + ";")
            except:
                print "Se produjo un error al intentar cambiar los permisos (revoke all) de las bases de datos de PostgreSQL."
                sys.exit()


        
        if (self.checkBox_select.isChecked()):
            try:
                cur.execute("grant select on " + str(table) + " to " + str(username) + ";")
            except:
                print "Se produjo un error al intentar cambiar los permisos (select) de las bases de datos de PostgreSQL."
                sys.exit()
        else:
            try:
                cur.execute("revoke select on " + str(table) + " to " + str(username))
            except:
                print "Se produjo un error al intentar cambiar los permisos (select) de las bases de datos de PostgreSQL."
                sys.exit()

        if (self.checkBox_insert.isChecked()):
            try:
                cur.execute("grant insert on " + str(table) + " to " + str(username) + ";")
            except:
                print "Se produjo un error al intentar cambiar los permisos (insert) de las bases de datos de PostgreSQL."
                sys.exit()

        if (self.checkBox_update.isChecked()):
            try:
                cur.execute("grant update on " + str(table) + " to " + str(username) + ";")
            except:
                print "Se produjo un error al intentar cambiar los permisos (update) de las bases de datos de PostgreSQL."
                sys.exit()

        if (self.checkBox_delete.isChecked()):
            try:
                cur.execute("grant delete on " + str(table) + " to " + str(username) + ";")
            except:
                print "Se produjo un error al intentar cambiar los permisos (delete) de las bases de datos de PostgreSQL."
                sys.exit()

        if (self.checkBox_rule.isChecked()):
            try:
                cur.execute("grant rule on " + str(table) + " to " + str(username) + ";")
            except:
                print "Se produjo un error al intentar cambiar los permisos (rule) de las bases de datos de PostgreSQL."
                sys.exit()

        if (self.checkBox_references.isChecked()):
            try:
                cur.execute("grant references on " + str(table) + " to " + str(username) + ";")
            except:
                print "Se produjo un error al intentar cambiar los permisos (references) de las bases de datos de PostgreSQL."
                sys.exit()

        if (self.checkBox_trigger.isChecked()):
            try:
                cur.execute("grant trigger on " + str(table) + " to " + str(username) + ";")
            except:
                print "Se produjo un error al intentar cambiar los permisos (trigger) de las bases de datos de PostgreSQL."
                sys.exit()

        if (self.checkBox_create.isChecked()):
            try:
                cur.execute("grant create on database " + str(database) + " to " + str(username) + ";")
            except:
                print "Se produjo un error al intentar cambiar los permisos (create) de las bases de datos de PostgreSQL."
                sys.exit()

        if (self.checkBox_temporary.isChecked()):
            try:
                cur.execute("grant temporary on database " + str(database) + " to " + str(username) + ";")
            except:
                print "Se produjo un error al intentar cambiar los permisos (temporary) de las bases de datos de PostgreSQL."
                sys.exit()

        #if (self.checkBox_execute.isChecked()):
            #try:
                #cur.execute("grant execute on " + str(table) + " to " + str(username) + ";")
            #except:
                #print "Se produjo un error al intentar cambiar los permisos (execute) de las bases de datos de PostgreSQL."
                #sys.exit()

        #if (self.checkBox_usage.isChecked()):
            #try:
                #cur.execute("grant usage on " + str(table) + " to " + str(username) + ";")
            #except:
                #print "Se produjo un error al intentar cambiar los permisos (usage) de las bases de datos de PostgreSQL."
                #sys.exit()

def main(args):
    app=QtGui.QApplication(args)
    win=ModificarUsuario()
    win.exec_()
    sys.exit(app.exec_())

if __name__=="__main__":
    main(sys.argv)