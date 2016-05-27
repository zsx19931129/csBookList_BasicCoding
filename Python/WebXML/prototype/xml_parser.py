from xml.sax.handler import ContentHandler
from xml.sax import parse
import os.path

class TestHandler(ContentHandler):
	def startElement(self, name, attrs):
		print name, attrs.keys()

class HeadlineHandler(ContentHandler):

	in_headline = False
	def __init__(self, headlines):
		ContentHandler.__init__(self)
		self.headlines = headlines
		self.data = []

	def startElement(self, name, attrs):
		if name == 'h1':
			self.in_headline = True

	def endElement(self, name):
		if name == 'h1':
			text = ''.join(self.data)
			self.data = []
			self.in_headline = False
			self.headlines.append(text)

	def characters(self, string):
		if self.in_headline:
			# print 'string:', string
			self.data.append(string)


headlines = []

curr_dir = os.path.dirname(os.path.abspath(__file__))
xml_path = os.path.join(curr_dir, os.path.pardir, "website.xml")
print 'xml_path:', xml_path

# parse(xml_path, TestHandler())

# parse(xml_path, HeadlineHandler(headlines))

# print 'The following <h1> elements were found:'
# for h in headlines:
# 	print h

class PageMaker(ContentHandler):
	in_page = False
	def startElement(self, name, attrs):
		if name == 'page':
			self.in_page = True
			self.out = open(attrs['name'] + '.html', 'w')
			self.out.write('<html><head>\n')
			self.out.write('<title>%s</title>\n' % attrs['title'])
			self.out.write('</head><body>\n')
		elif self.in_page:
			# not change content
			self.out.write('<' + name)
			for key, val in attrs.items():
				self.out.write(' %s=%s' % (key, val))
			self.out.write('>')

	
	def endElement(self, name):
		if name == 'page':
			self.in_page = False
			self.out.write('\n</body></html>\n')
			self.out.close()
		elif self.in_page:
			self.out.write('</%s>' % name)

	def characters(self, string):
		if self.in_page:
			self.out.write(string)

parse(xml_path, PageMaker())
