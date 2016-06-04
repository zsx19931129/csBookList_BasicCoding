#! /usr/bin/python
# execute these command to avoid not image error
# otool -L /Library/Python/2.7/site-packages/_mysql.so
# sudo install_name_tool -change libmysqlclient.18.dylib /usr/local/mysql/lib/libmysqlclient.18.dylib /Library/Python/2.7/site-packages/_mysql.so 


print 'Content-type: text/html\n'

import cgitb; cgitb.enable()

import MySQLdb as db
conn = db.connect(user='root', passwd='helloworld',
	db='test', host='localhost')

curs = conn.cursor()

print """
<html>
	<head>
		<title>The FooBar Bulletin Board</title>
	</head>
	<body>
		<h1>The FooBar Bulletin Board</h1>
		"""
# low performance when resultset is large
def dictfetchall(cursor):
	"Returns all rows from a cursor as a dict"
	desc = cursor.description
	column_name = [col[0] for col in desc]
	return [
		dict(zip(column_name, row))
		for row in cursor.fetchall()
	]

curs.execute('SELECT * FROM messages')
rows = dictfetchall(curs)
toplevel=[]
children={}

for row in rows:
	parent_id = row['reply_to']
	if parent_id is None:
		toplevel.append(row)
	else:
		children.setdefault(parent_id,[]).append(row)

def format(row):
	print '<p><a href="/cgi-bin/WebForum/view.cgi?id=%(id)i">%(subject)s</a></p>' % row
	try:
		kids = children[row['id']]
	except KeyError:
		pass
	else:
		print '<blockquote>'
		for kid in kids:
			format(kid)
		print '</blockquote>'

print '<p>'

for row in toplevel:
	format(row)

print """
			</p>
		<hr />
		<p><a href="/cgi-bin/WebForum/edit.cgi">Post message</a></p>
	</body>
</html>
"""