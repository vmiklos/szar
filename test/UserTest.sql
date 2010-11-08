truncate table users;
insert into users (username, password, admin) values ("admin", SHA1("admin"), 1);
