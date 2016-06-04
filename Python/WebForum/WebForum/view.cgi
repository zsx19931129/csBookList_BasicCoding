#! /usr/bin/python

print 'Content-type: text/html\n'
import cgitb; cgitb.enable()

import MySQLdb as db
conn = db.connect(user='root', passwd='helloworld',
	db='test', host='localhost')
curs = conn.cursor()

import cgi,sys
form = cgi.FieldStorage()
id = form.getvalue('id')

# low performance when resultset is large
def dictfetchall(cursor):
	"Returns all rows from a cursor as a dict"
	desc = cursor.description
	column_name = [col[0] for col in desc]
	return [
		dict(zip(column_name, row))
		for row in cursor.fetchall()
	]


print """
<html>
	<head>
		<title>View Message</title>
	</head>
	<body>
		<h1>View Message</h1>
		"""

try:
	id = int(id)
except:
	print 'Invaild message ID'
	sys.exit()

curs.execute('SELECT * FROM messages WHERE id = %i' % id)
rows = dictfetchall(curs)

if not rows:
	print 'Unknown message ID'
	sys.exit()

row = rows[0]

print """
		<p>
			<b>Subject:</b> %(subject)s<br />
			<b>Sender:</b> %(sender)s<br />
			<pre>%(text)s</pre>
		</p>
		<hr />
		<a href='/cgi-bin/WebForum/main.cgi'>Back to the main page</a>
		| <a href='/cgi-bin/WebForum/edit.cgi?reply_to=%(id)s'>Reply</a>
	</body>
</html>
""" % row
