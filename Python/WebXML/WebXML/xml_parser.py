from xml.sax.handler import ContentHandler
from xml.sax import parse
import os


class Dispatcher:

	def dispatch(self, prefix, name, attrs=None):
		mname = prefix + name.capitalize()
		dname = 'default' + prefix.capitalize()
		method = getattr(self, mname, None)
		if callable(method): args = ()
		else:
			method = getattr(self, dname, None)
			args = name,
		if prefix == 'start': args += attrs,
		if callable(method): method(*args)

	def startElement(self, name, attrs):
		self.dispatch('start', name, attrs)

	def endElement(self, name):
		self.dispatch('end', name)

class WebsiteConstructor(Dispatcher, ContentHandler):
	in_page = False

	def __init__(self, directory):
		self.directory = [directory]
		self.ensureDirectory()

	def ensureDirectory(self):
		path = os.path.join(*self.directory)
		if not os.path.isdir(path): os.makedirs(path)

	def characters(self, chars):
		if self.in_page: self.out.write(chars)

	def defaultStart(self, name, attrs):
		if self.in_page:
			self.out.write('<' + name)
			for key, val in attrs.items():
				self.out.write(' %s=%s' % (key, val))
			self.out.write('>')

	def defaultEnd(self, name):
		if self.in_page:
			self.out.write('</%s>' % name)

	def startDirectory(self, attrs):
		self.directory.append(attrs['name'])
		self.ensureDirectory()

	def endDirectory(self):
		self.directory.pop()

	def startPage(self, attrs):
		filename = os.path.join(*self.directory+[attrs['name']+'.html'])
		self.out = open(filename, 'w')
		self.writeHeader(attrs['title'])
		self.in_page = True

	def endPage(self):
		self.in_page = False
		self.writeFooter()
		self.out.close()

	def writeHeader(self, title):
		self.out.write('<html>\n <head>\n <title>')
		self.out.write(title)
		self.out.write('</title>\n </head>\n <body>\n')

	def writeFooter(self):
		self.out.write('\n </body>\n</html>\n')

curr_dir = os.path.dirname(os.path.abspath(__file__))
xml_path = os.path.join(curr_dir, os.path.pardir, "website.xml")
parse(xml_path, WebsiteConstructor('public_html'))
