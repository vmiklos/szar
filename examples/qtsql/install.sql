CREATE TABLE users (
	id INT AUTO_INCREMENT PRIMARY KEY,
	username VARCHAR(64) NOT NULL,
	password CHAR(40) NOT NULL,
	admin BOOL DEFAULT 0 NOT NULL
) ENGINE = InnoDB;
INSERT INTO users (username, password, admin)
	VALUES ("admin", SHA1("admin"), 1);
