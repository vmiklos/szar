CREATE TABLE users (
	id INT AUTO_INCREMENT PRIMARY KEY,
	username VARCHAR(64) NOT NULL UNIQUE,
	password CHAR(40) NOT NULL DEFAULT 'x',
	admin BOOL DEFAULT 0 NOT NULL
) ENGINE = InnoDB;
INSERT INTO users (username, password, admin)
	VALUES ("admin", SHA1("admin"), 1);

CREATE TABLE acl (
	user_id INT(11) NOT NULL,
	model_id INT(11) NOT NULL,
	rights ENUM('Read', 'ReadWrite') NOT NULL,
  PRIMARY KEY (user_id, model_id)
) ENGINE = InnoDB;

CREATE TABLE models (
	id INT(11) AUTO_INCREMENT PRIMARY KEY,
	locked INT(11) DEFAULT NULL,
	name VARCHAR(128) NOT NULL UNIQUE
) ENGINE = InnoDB;

CREATE TABLE revisions (
	revnum INT(11) NOT NULL DEFAULT 0,
	model_id INT(11) NOT NULL,
	user_id INT(11) NOT NULL,
	created TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
	content LONGBLOB NOT NULL,
	PRIMARY KEY (revnum, model_id),
	KEY (user_id)
) ENGINE = InnoDB;

DELIMITER //
CREATE TRIGGER revnum BEFORE INSERT ON revisions
FOR EACH ROW BEGIN
	SET NEW.revnum = (SELECT IFNULL(MAX(revnum), 0) + 1
		FROM revisions WHERE model_id = new.model_id);
END
//
DELIMITER ;

ALTER TABLE acl
	ADD CONSTRAINT acl_fk_model FOREIGN KEY (model_id)
		REFERENCES models (id) ON DELETE CASCADE,
	ADD CONSTRAINT acl_fk_user FOREIGN KEY (user_id)
		REFERENCES users (id) ON DELETE CASCADE;

ALTER TABLE revisions
	ADD CONSTRAINT revisions_fk_user FOREIGN KEY (user_id)
		REFERENCES users (id) ON DELETE CASCADE,
	ADD CONSTRAINT revisions_fk_model FOREIGN KEY (model_id)
		REFERENCES models (id) ON DELETE CASCADE;

ALTER TABLE models
	ADD CONSTRAINT models_fk_user FOREIGN KEY (locked)
		REFERENCES users (id) ON DELETE SET NULL;
