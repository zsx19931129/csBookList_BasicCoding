#coding:utf-8

from nntplib import NNTP
from time import strftime, time, localtime

def ascii2utf_8(string):
	return string.decode('ascii').encode('UTF-8')

day = 24*60*60

# yesterday = localtime(time() - day)
offday = localtime(time() - 20*day)

date = strftime('%y%m%d', offday)
hour = strftime('%H%M%S', offday)

servername = 'news.newsfan.net'
group = 'alt.language.English'
server = NNTP(servername)

ids = server.newnews(group, date, hour)[1]

print 'ids_nums:%d' % len(ids)

for id in ids:
	head = server.head(id)[3]
	for line in head:
		if line.lower().startswith('subject:'):
			subject = line[9:]
			break

	body = server.body(id)[3]

	print 'type of subject:', type(subject)

	subject = subject.decode('gb2312').encode('utf-8')

	body = '\n'.join(body)
	body = body.decode('gb2312').encode('utf-8')

	print subject
	print '-'*len(subject)
	print body

server.quit()