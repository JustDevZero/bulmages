--
-- ACTUALIZACION DE LA BASE DE DATOS DE BULMAFACT
--


-- Ocultamos las noticias
SET client_min_messages TO warning;


BEGIN;

--
-- Estas primeras funciones cambiaran los tipos de columnas que estan como flotantes a NUMERIC.
-- Se trata de un parche que se desea aplicar para almacenar los tipos monetarios
-- ya que actualmente se encuantran almacenados como 'doubles' y es preferible
-- que se almacenen como tipo 'numeric'.
-- Todas devuelven como valor numerico el nmero de filas influenciadas por el cambio
-- NOTA: Si alguien sabe como pasar por parametro un nombre de tabla y campo a modificarse
-- hara mucho mas sencillito porque solo habra que implementar un funcion ya que siempre
-- hay que hacer lo mismo.
--

--
-- Funcion auxiliar para borrar funciones limpiamente
--
CREATE OR REPLACE FUNCTION drop_if_exists_table (text) RETURNS INTEGER AS '
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


CREATE OR REPLACE FUNCTION drop_if_exists_proc (text, text) RETURNS INTEGER AS '
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

-- SELECT drop_if_exists_proc ('calculacodigocompletoarticulo','');

-- ======================== COMPROBACION DE CUAL ES LA ULTIMA VERSION ==================================

CREATE OR REPLACE FUNCTION compruebarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre=''DatabaseRevision'' AND ( valor LIKE ''0.9.1%'' OR valor = ''0.5.9-0005'');
	IF FOUND THEN
		RETURN 0;
	ELSE
		RAISE EXCEPTION '' Version de la base de datos invalida, aplique parches anteriores'';
		RETURN -1;		 
	END IF;
END;
'   LANGUAGE plpgsql;
SELECT compruebarevision();
DROP FUNCTION compruebarevision() CASCADE;
\echo "Comprobada la revision"

-- ========================  FIN DE LA COMPROBACION ============================



CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM pg_attribute  WHERE attname=''ordenlpresupuesto'';
	IF NOT FOUND THEN
		ALTER TABLE lpresupuesto ADD COLUMN ordenlpresupuesto INTEGER;
	END IF;
	SELECT INTO as * FROM pg_attribute  WHERE attname=''ordenlpedidocliente'';
	IF NOT FOUND THEN
		ALTER TABLE lpedidocliente ADD COLUMN ordenlpedidocliente INTEGER;
		ALTER TABLE lfactura ADD COLUMN ordenlfactura INTEGER;
		ALTER TABLE lfacturap ADD COLUMN ordenlfacturap INTEGER;
		ALTER TABLE lalbaranp ADD COLUMN ordenlalbaranp INTEGER;
		ALTER TABLE lpedidoproveedor ADD COLUMN ordenlpedidoproveedor INTEGER;
	END IF;
	SELECT INTO as * FROM pg_attribute  WHERE attname=''ordenlalbaran'';
	IF NOT FOUND THEN
		ALTER TABLE lalbaran ADD COLUMN ordenlalbaran INTEGER;
	END IF;

	SELECT INTO as * FROM pg_attribute WHERE attname=''porcentretasa_iva'';
	IF NOT FOUND THEN
		ALTER TABLE tasa_iva ADD COLUMN porcentretasa_iva NUMERIC(5,2);
		ALTER TABLE tasa_iva ALTER COLUMN porcentretasa_iva SET DEFAULT 0;
		ALTER TABLE cliente ADD COLUMN recargoeqcliente BOOLEAN;
		ALTER TABLE cliente ALTER COLUMN recargoeqcliente SET DEFAULT FALSE;
		ALTER TABLE cliente ADD COLUMN idforma_pago integer;
		ALTER TABLE cliente ADD CONSTRAINT idforma_pagofk FOREIGN KEY (idforma_pago) REFERENCES forma_pago(idforma_pago);
		ALTER TABLE lpedido ADD COLUMN reqeqlpedido NUMERIC(12,2);
		ALTER TABLE lpedido ALTER COLUMN reqeqlpedido SET DEFAULT 0;
		ALTER TABLE lpresupuesto ADD COLUMN reqeqlpresupuesto NUMERIC(12,2);
		ALTER TABLE lpresupuesto ALTER COLUMN reqeqlpresupuesto SET DEFAULT 0;
		ALTER TABLE lpedidocliente ADD COLUMN reqeqlpedidocliente NUMERIC(12,2);
		ALTER TABLE lpedidocliente ALTER COLUMN reqeqlpedidocliente SET DEFAULT 0;
		ALTER TABLE lfactura ADD COLUMN reqeqlfactura NUMERIC(12,2);
		ALTER TABLE lfactura ALTER COLUMN reqeqlfactura SET DEFAULT 0;
		ALTER TABLE lfacturap ADD COLUMN reqeqlfacturap NUMERIC(12,2);
		ALTER TABLE lfacturap ALTER COLUMN reqeqlfacturap SET DEFAULT 0;
		ALTER TABLE lalbaran ADD COLUMN reqeqlalbaran NUMERIC(12,2);
		ALTER TABLE lalbaran ALTER COLUMN reqeqlalbaran SET DEFAULT 0;
		ALTER TABLE lpedidoproveedor ADD COLUMN reqeqlpedidoproveedor NUMERIC(12,2);
		ALTER TABLE lpedidoproveedor ALTER COLUMN reqeqlpedidoproveedor SET DEFAULT 0;
		ALTER TABLE lalbaranp ADD COLUMN reqeqlalbaranp NUMERIC(12,2);
		ALTER TABLE lalbaranp ALTER COLUMN reqeqlalbaranp SET DEFAULT 0;
	END IF;

	SELECT INTO as * FROM pg_attribute WHERE attname=''regimenfiscalcliente'';
	IF NOT FOUND THEN
		ALTER TABLE cliente ADD COLUMN regimenfiscalcliente CHARACTER VARYING(50);
		UPDATE cliente SET regimenfiscalcliente = ''Normal'';
		ALTER TABLE cliente ALTER COLUMN regimenfiscalcliente SET NOT NULL;
	END IF;
	-- Cambiamos el nombre del campo descontlalbaran en la tabla lalbaran
	SELECT INTO as * FROM pg_attribute WHERE attname=''descontlalbaran'';
	IF FOUND THEN
		ALTER TABLE lalbaran RENAME COLUMN descontlalbaran TO descuentolalbaran;
	END IF;

	-- Cambiamos el nombre del campo descontlalbaran en la tabla lalbaran
	SELECT INTO as * FROM pg_attribute WHERE attname=''descontlalbaranp'';
	IF FOUND THEN
		ALTER TABLE lalbaranp RENAME COLUMN descontlalbaranp TO descuentolalbaranp;
	END IF;

	SELECT INTO as * FROM pg_attribute WHERE attname=''regimenfiscalproveedor'';
	IF NOT FOUND THEN
		ALTER TABLE proveedor ADD COLUMN recargoeqproveedor BOOLEAN;
		ALTER TABLE proveedor ALTER COLUMN recargoeqproveedor SET DEFAULT FALSE;
		ALTER TABLE proveedor ADD COLUMN idforma_pago integer;
		ALTER TABLE proveedor ADD CONSTRAINT idforma_pagofk FOREIGN KEY (idforma_pago) REFERENCES forma_pago(idforma_pago);
		ALTER TABLE proveedor ADD COLUMN regimenfiscalproveedor CHARACTER VARYING(50);
		UPDATE proveedor SET regimenfiscalproveedor = ''Normal'';
		ALTER TABLE proveedor ALTER COLUMN regimenfiscalproveedor SET NOT NULL;
	END IF;
	SELECT INTO as * FROM pg_attribute WHERE attname=''irpfproveedor'';
	IF NOT FOUND THEN
		ALTER TABLE proveedor ADD COLUMN irpfproveedor NUMERIC(12,2);
		UPDATE proveedor SET irpfproveedor = 0;
		ALTER TABLE proveedor ALTER COLUMN irpfproveedor SET DEFAULT 0;
	END IF;


	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;
\echo "Agregamos los campos de totales para los presupuestos."


\echo -n ':: Funcion que calcula el total del albaran de proveedor ... '
CREATE OR REPLACE FUNCTION calctotalalbpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    res RECORD;

BEGIN
    total := 0;
    FOR res IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) * (1 + ivalalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	total := total + res.subtotal1;
    END LOOP;
    FOR res IN SELECT proporciondalbaranp FROM dalbaranp WHERE idalbaranp = idp LOOP
	total := total * (1 - res.proporciondalbaranp / 100);
    END LOOP;
    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula la Base Imponible total de un albaran de proveedor ... '
CREATE OR REPLACE FUNCTION calcbimpalbpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    res RECORD;

BEGIN
    total := 0;
    FOR res IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	total := total + res.subtotal1;
    END LOOP;
    FOR res IN SELECT proporciondalbaranp FROM dalbaranp WHERE idalbaranp = idp LOOP
	total := total * (1 - res.proporciondalbaranp / 100);
    END LOOP;
    RETURN total;
END;
' LANGUAGE plpgsql;

\echo -n ':: Funcion que calcula el total de impuestos de un albaran de proveedor ... '
CREATE OR REPLACE FUNCTION calcimpuestosalbpro(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    res RECORD;

BEGIN
    total := 0;
    FOR res IN SELECT cantlalbaranp * pvplalbaranp * (1 - descuentolalbaranp / 100) * (ivalalbaranp / 100) AS subtotal1 FROM lalbaranp WHERE idalbaranp = idp LOOP
	total := total + res.subtotal1;
    END LOOP;
    FOR res IN SELECT proporciondalbaranp FROM dalbaranp WHERE idalbaranp = idp LOOP
	total := total * (1 - res.proporciondalbaranp / 100);
    END LOOP;
    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula el total de un albaran a cliente ... '
CREATE OR REPLACE FUNCTION calctotalalbaran(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    res RECORD;

BEGIN
    total := 0;
    FOR res IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) * (1 + ivalalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
	total := total + res.subtotal1;
    END LOOP;
    FOR res IN SELECT proporciondalbaran FROM dalbaran WHERE idalbaran = idp LOOP
    	total := total * (1 - res.proporciondalbaran / 100);
    END LOOP;
    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula la Base Imponible total de un albaran a cliente ... '
CREATE OR REPLACE FUNCTION calcbimpalbaran(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    res RECORD;

BEGIN
    total := 0;
    FOR res IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
	total := total + res.subtotal1;
    END LOOP;
    FOR res IN SELECT proporciondalbaran FROM dalbaran WHERE idalbaran = idp LOOP
	total := total * (1 - res.proporciondalbaran / 100);
    END LOOP;
    RETURN total;
END;
' LANGUAGE plpgsql;


\echo -n ':: Funcion que calcula los impuestos totales de un albaran a cliente ... '
CREATE OR REPLACE FUNCTION calcimpuestosalbaran(integer) RETURNS numeric(12, 2)
AS '
DECLARE
    idp ALIAS FOR $1;
    total numeric(12, 2);
    res RECORD;

BEGIN
    total := 0;
    FOR res IN SELECT cantlalbaran * pvplalbaran * (1 - descuentolalbaran / 100) * (ivalalbaran / 100) AS subtotal1 FROM lalbaran WHERE idalbaran = idp LOOP
    	total := total + res.subtotal1;
    END LOOP;
    FOR res IN SELECT proporciondalbaran FROM dalbaran WHERE idalbaran = idp LOOP
    	total := total * (1 - res.proporciondalbaran / 100);
    END LOOP;
    RETURN total;
END;
' LANGUAGE plpgsql;


CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre = ''IRPF'';
	IF NOT FOUND THEN
		INSERT INTO configuracion (nombre, valor) VALUES (''IRPF'', ''0''); 		 
	END IF;

	SELECT INTO as * FROM pg_tables  WHERE tablename=''banco'';
	IF NOT FOUND THEN
		CREATE TABLE banco (
			idbanco serial PRIMARY KEY,
			nombanco character varying(50),
			dirbanco character varying(150),
			poblbanco character varying(50),
			cpbanco character varying(20),
			telbanco character varying(20),
			faxbanco character varying(20),
			emailbanco character varying(100),
			contactobanco character varying,
			codentidadbanco character varying(14),
			codagenciabanco character varying(17),
			numcuentabanco  character varying(17),
			dcbanco character varying(2),
			comentbanco character varying,
			webbanco character varying(100)
		);
	END IF;

	SELECT INTO as attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''idbanco'' AND relname=''cobro'';
	IF NOT FOUND THEN
		ALTER TABLE cobro ADD COLUMN idbanco INTEGER;
		ALTER TABLE cobro ADD CONSTRAINT idbancofk FOREIGN KEY (idbanco) REFERENCES banco(idbanco);
		ALTER TABLE pago ADD COLUMN idbanco INTEGER;
		ALTER TABLE pago ADD CONSTRAINT idbancofk FOREIGN KEY (idbanco) REFERENCES banco(idbanco);
	END IF;

	SELECT INTO as attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''productofisicofamilia'' AND relname=''familia'';
	IF NOT FOUND THEN
		ALTER TABLE familia ADD COLUMN productofisicofamilia BOOLEAN DEFAULT TRUE;
		UPDATE familia SET productofisicofamilia = TRUE;
		ALTER TABLE familia ALTER COLUMN productofisicofamilia SET NOT NULL;
	END IF;

	SELECT INTO as attname, relname FROM pg_attribute LEFT JOIN pg_class ON pg_attribute.attrelid=pg_class.oid WHERE attname=''pesoundarticulo'' AND relname=''articulo'';
	IF NOT FOUND THEN
		ALTER TABLE articulo ADD COLUMN pesoundarticulo NUMERIC(12,2) DEFAULT 0;
		ALTER TABLE articulo ADD COLUMN volumenundarticulo NUMERIC(12,2) DEFAULT 0;
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;


\echo -n ':: Funcion que actualiza la tabla de proveedor ... '
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
BEGIN
    ALTER TABLE proveedor RENAME COLUMN cbancproveedor TO cbancproveedor_viejo;
    ALTER TABLE proveedor ADD COLUMN cbancproveedor_nuevo character(120);
    UPDATE proveedor SET cbancproveedor_nuevo = cbancproveedor_viejo;
    ALTER TABLE proveedor DROP COLUMN cbancproveedor_viejo;
    ALTER TABLE proveedor RENAME COLUMN cbancproveedor_nuevo TO cbancproveedor;
    RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;


\echo -n ':: Funcion que actualiza la tabla de cliente ... '
CREATE OR REPLACE FUNCTION aux() RETURNS INTEGER AS '
BEGIN
    ALTER TABLE cliente RENAME COLUMN bancocliente TO bancocliente_viejo;
    ALTER TABLE cliente ADD COLUMN bancocliente character(120);
    UPDATE cliente SET bancocliente = bancocliente_viejo;
    ALTER TABLE cliente DROP COLUMN bancocliente_viejo;
    RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT aux();
DROP FUNCTION aux() CASCADE;





-- SELECT drop_if_exists_table('controlstock');
-- SELECT drop_if_exists_table('inventario');
-- SELECT drop_if_exists_table('stock_almacen');

SELECT drop_if_exists_proc('narticulo','');
SELECT drop_if_exists_proc('darticulo','');
SELECT drop_if_exists_proc('nalmacen','');
SELECT drop_if_exists_proc('dalmacen','');
SELECT drop_if_exists_proc('modificadostockalmacen', '');
SELECT drop_if_exists_proc('disminuyecontrolstock', '');
SELECT drop_if_exists_proc('disminuyecontrolstock1', '');
SELECT drop_if_exists_proc('disminuyecontrolstock2', '');
SELECT drop_if_exists_proc('aumentacontrolstock', '');
SELECT drop_if_exists_proc('cambiaalbaran', '');
SELECT drop_if_exists_proc('cambiaalbaranp', '');


SELECT drop_if_exists_proc('disminuyestock', '');
\echo -n ':: Funcion para disminuir stock ... '
CREATE FUNCTION disminuyestock() RETURNS "trigger"
AS '
DECLARE
BEGIN
    -- Hacemos el update del stock del articulo
    UPDATE articulo SET stockarticulo = stockarticulo + OLD.cantlalbaran WHERE idarticulo= OLD.idarticulo;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que disminuye stock al borrar o actualizar el detalle de un albaran a cliente ... '
CREATE TRIGGER disminuyestockt
    AFTER DELETE OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE disminuyestock();

SELECT drop_if_exists_proc('aumentastock', '');
\echo -n ':: Funcion para aumentar stock ... '
CREATE FUNCTION aumentastock() RETURNS "trigger"
AS '
DECLARE
BEGIN
    -- Hacemos el update del stock del articulo
    UPDATE articulo SET stockarticulo = stockarticulo - NEW.cantlalbaran WHERE idarticulo = NEW.idarticulo;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que aumenta stock al insertar o actualizar el detalle de un albaran a cliente ... '
CREATE TRIGGER aumentastockt
    AFTER INSERT OR UPDATE ON lalbaran
    FOR EACH ROW
    EXECUTE PROCEDURE aumentastock();

SELECT drop_if_exists_proc('disminuyestockp', '');
\echo -n ':: Funcion disminuye stockp ... '
CREATE FUNCTION disminuyestockp() RETURNS "trigger"
AS '
DECLARE

BEGIN
    -- Hacemos el update del stock del articulo
    UPDATE articulo SET stockarticulo = stockarticulo - OLD.cantlalbaranp WHERE idarticulo= OLD.idarticulo;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que disminuye stock al borrar o actualizar el detalle de un albaran de proveedor ... '
CREATE TRIGGER disminuyestockpt
    AFTER DELETE OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE disminuyestockp();

SELECT drop_if_exists_proc('aumentastockp', '');
\echo -n ':: Funcion que aumenta el stock al recibir mercancias de un proveedor ... '
CREATE FUNCTION aumentastockp() RETURNS "trigger"
AS '
DECLARE
BEGIN
    UPDATE articulo SET stockarticulo = stockarticulo + NEW.cantlalbaranp WHERE idarticulo = NEW.idarticulo;
    RETURN NEW;
END;
' LANGUAGE plpgsql;


\echo -n ':: Disparador que aumenta stock al insertar o actualizar el detalle de un albaran de proveedor ... '
CREATE TRIGGER aumentastockpt
    AFTER INSERT OR UPDATE ON lalbaranp
    FOR EACH ROW
    EXECUTE PROCEDURE aumentastockp(); 


-- ================================== ACTUALIZACION  ===================================
-- =====================================================================================

-- Agregamos nuevos parametros de configuracion
--
CREATE OR REPLACE FUNCTION actualizarevision() RETURNS INTEGER AS '
DECLARE
	as RECORD;
BEGIN
	SELECT INTO as * FROM configuracion WHERE nombre = ''DatabaseRevision'';
	IF FOUND THEN
		UPDATE CONFIGURACION SET valor = ''0.9.1-0011'' WHERE nombre = ''DatabaseRevision'';
	ELSE
		INSERT INTO configuracion (nombre, valor) VALUES (''DatabaseRevision'', ''0.9.1-0011'');
	END IF;
	RETURN 0;
END;
'   LANGUAGE plpgsql;
SELECT actualizarevision();
DROP FUNCTION actualizarevision() CASCADE;
\echo "Actualizada la revision de la base de datos a la version 0.9.1"

DROP FUNCTION drop_if_exists_table(text) CASCADE;
DROP FUNCTION drop_if_exists_proc(text, text) CASCADE;


COMMIT;





