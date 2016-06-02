# sudo /Library/StartupItems/MySQLCOM/MySQLCOM start 启动MySQL服务
# sudo /Library/StartupItems/MySQLCOM/MySQLCOM stop  停止MySQL服务
# sudo /Library/StartupItems/MySQLCOM/MySQLCOM restart 重启MySQL服务

# sudo pip install MYSQL-Python to install package, and if there is
# lib errors, export DYLD_LIBRARY_PATH=/usr/local/mysql/lib

CREATE TABLE messages(
	id INT NOT NULL AUTO_INCREMENT,
	subject VARCHAR(100) NOT NULL,
	sender	VARCHAR(15) NOT NULL, 
	reply_to INT,
	text MEDIUMTEXT NOT NULL,
	PRIMARY KEY(id)
)

