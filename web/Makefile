all:home.cgi login.cgi

home.cgi:
	sudo gcc home.c -o home.cgi -lsqlite3 -Wall

login.cgi:
	sudo gcc login.c -o login.cgi -Wall

install:
	sudo cp home.cgi login.cgi /usr/lib/cgi-bin/
	sudo cp styles.css /var/www/html/

clean:
	sudo rm *.cgi
