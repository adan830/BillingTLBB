-- Copyright (c) Dark.Hades
-- https://github.com/HadesD/TLBB.Billing

CREATE TABLE account (
  id        INT(11) NOT NULL AUTO_INCREMENT,
  name      VARCHAR(32) NOT NULL,
  password  CHAR(32) NOT NULL,
  point     INT(11) NOT NULL DEFAULT 0,
  is_online TINYINT(1) NOT NULL DEFAULT 0,
  is_lock   TINYINT(1) NOT NULL DEFAULT 0,

  PRIMARY   KEY (id, name),
  UNIQUE    KEY id USING BTREE (id),
  UNIQUE    KEY name (name)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

