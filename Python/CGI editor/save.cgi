#!/usr/bin/python

# password foobar
# this file and data dir should be put in your cgi-bin dir

print 'Content-type: text/html\n'

from os.path import join, abspath
import cgi, sha, sys

BASE_DIR = abspath('data')

form = cgi.FieldStorage()

text = form.getvalue('text')
filename = form.getvalue('filename')
password = form.getvalue('password')

if not (filename and text and password):
	print 'Invalid parameters.'
	sys.exit()

if sha.sha(password).hexdigest() != '8843d7f92416211de9ebb963ff4ce28125932878':
	print 'Invalid password'
	sys.exit()

print 'Will be written to file'

f = open(join(BASE_DIR, filename), 'w')
if not f:
	print 'cannot open file in write'
f.write(text)
f.close()

print 'The file has been saved.'