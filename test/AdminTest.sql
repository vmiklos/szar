truncate table users;
truncate table models;
insert into users (username, password, admin) values ("admin", SHA1("admin"), 1);
