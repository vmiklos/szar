truncate table users;
insert into users (username, password, admin) values ("admin", SHA1("admin"), 1);
insert into users (username, password, admin) values ("john", SHA1("foo"), 0);
truncate table models;
insert into models (name) values ("foo");
truncate table acl;
insert into acl (user_id, model_id, rights) values (1, 1, "ReadWrite");
