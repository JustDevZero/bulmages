--
-- Modificacion de campos y funciones de la BD para la adaptacion para el plugin de
-- MODIFICADORES
--
\echo "********* INICIADO FICHERO DE ESTRUCTURA DEL PLUGIN DE MODIFICADORES *********"

\echo ":: Establecemos los mensajes minimos a avisos y otros parametros ... "
\echo -n ":: "
SET client_min_messages TO WARNING;
SET log_min_messages TO WARNING;
-- SET log_error_verbosity TO TERSE;
BEGIN;

--
-- Funcion auxiliar para borrar funciones limpiamente
--
create or replace function drop_if_exists_table (text) returns INTEGER AS '
DECLARE
    tbl_name ALIAS FOR $1;

BEGIN
    IF (select count(*) from pg_tables where tablename=$1) THEN
	EXECUTE ''DROP TABLE '' || $1;
	RETURN 1;
    END IF;

    RETURN 0;
END;
'
language 'plpgsql';


create or replace function drop_if_exists_proc (text,text) returns INTEGER AS '
DECLARE
    proc_name ALIAS FOR $1;
    proc_params ALIAS FOR $2;

BEGIN
    IF (select count(*) from pg_proc where proname=$1) THEN
	EXECUTE ''DROP FUNCTION '' || $1 || ''(''||$2||'') CASCADE'';
	RETURN 1;
    END IF;

    RETURN 0;
END;
'
language 'plpgsql';



-- ========================== VARIACION DE TARIFA =======================


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
    SELECT INTO rs attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''textolibremodificador'' AND relname=''lalbaran'';

    IF FOUND THEN
        ALTER TABLE lalbaran DROP COLUMN textolibremodificador;
        ALTER TABLE lalbaran DROP COLUMN imgmodificador; 
	ALTER TABLE lalbaran DROP COLUMN idmodificador1 CASCADE;
	ALTER TABLE lalbaran DROP COLUMN idmodificador2 CASCADE;
	ALTER TABLE lalbaran DROP COLUMN idmodificador3 CASCADE;
	ALTER TABLE lalbaran DROP COLUMN idmodificador4 CASCADE;
	ALTER TABLE lalbaran DROP COLUMN idmodificador5 CASCADE;
	ALTER TABLE lalbaran DROP COLUMN idmodificador6 CASCADE;
	ALTER TABLE lalbaran DROP COLUMN idmodificador7 CASCADE;
	ALTER TABLE lalbaran DROP COLUMN idmodificador8 CASCADE;
	ALTER TABLE lalbaran DROP COLUMN idmodificador9 CASCADE;
    END IF;

    SELECT INTO rs * FROM pg_tables  WHERE tablename=''modificador'';

    IF FOUND THEN
        DROP TABLE modificador CASCADE;
    END IF;

    RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Eliminamos la tabla modificador"

-- ======================== Actualizada la revision de la base de datos a la version. =====================


-- Agregamos nuevos parametros de configuracion.
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	rs RECORD;

BEGIN
	SELECT INTO rs * FROM configuracion WHERE nombre=''PluginBf_Modificadores'';

	IF FOUND THEN
		DELETE FROM CONFIGURACION WHERE nombre=''PluginBf_Modificadores'';
	END IF;

	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Eliminamos la revision de la base de datos"


DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text,text) CASCADE;


COMMIT;
