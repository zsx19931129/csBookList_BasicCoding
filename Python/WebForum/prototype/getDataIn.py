#! /usr/bin/python
# addmessage.py

import MySQLdb as db

conn = db.connect(user='root', passwd='helloworld', 
	host='localhost', db='test')
curs = conn.cursor()

reply_to = raw_input('Reply to: ')
subject = raw_input('Subject: ')
sender = raw_input('Sender: ')
text = raw_input('Text: ')

if reply_to:
	query = """
	INSERT INTO messages(reply_to, sender, subject, text)
	VALUES(%s, '%s', '%s', '%s')""" % (reply_to, sender, subject, text)
else:
	query = """
	INSERT INTO messages(sender, subject, text)
	VALUES('%s', '%s', '%s')""" % (sender, subject, text)

curs.execute(query)
conn.commit()