drop table users;
create table users (id int auto_increment primary key, username varchar(64) not null, password char(40) not null, admin bool default 0 not null);
insert into users (username, password) values ('foobar', 'c81c0eef7c0243fc309282e0bc1f2fd6836808dc');
