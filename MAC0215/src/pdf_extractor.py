from pdfminer.layout import LTTextBoxHorizontal, LTChar, LAParams, LTTextLine
from pdfminer.pdfinterp import PDFResourceManager, PDFPageInterpreter
from pdfminer.converter import PDFPageAggregator
from pdfminer.pdfdevice import PDFDevice
from pdfminer.pdfpage import PDFPage

class pdf_page:
	def __init__ (self, encoding='utf-8'):
		self.text = []
		self.bbox = []
		self.encoding = encoding

class extractor:
	def __init__ (self, pdf_path):
		self.path = pdf_path
		self.pdf_file = open (self.path, 'rb')
		self.pages = []


		rsrcmgr = PDFResourceManager()
		device = PDFPageAggregator(rsrcmgr, laparams=LAParams())
		interpreter = PDFPageInterpreter(rsrcmgr, device)
		
		for page in PDFPage.get_pages (self.pdf_file):
			interpreter.process_page (page)
			layout = device.get_result ()
			self.pages.append (pdf_page ())
			self.layout_search (layout)

	def layout_search (self, layout):
		for element in layout:
			
			if isinstance (element, LTChar):
				encoding = self.pages[-1].encoding
				self.pages[-1].text.append (element.get_text ().encode (encoding))
				#self.pages[-1].bbox.append (element.x0)
				#self.pages[-1].bbox.append (element.y0)
				#self.pages[-1].bbox.append (element.x0 + element.width * element.size)
				#self.pages[-1].bbox.append (element.y0 + element.height * element.size)
				#print element.matrix
				for i in range (0, 4):
					self.pages[-1].bbox.append (element.bbox[i])
				#print (element.height)
				#print (element.y0 + element.height)
				
			elif isinstance (element, LTTextBoxHorizontal) or isinstance (element, LTTextLine):
				self.layout_search (element)


def extract (pdf_path):
	global x	
	x = extractor (pdf_path)
	return len (x.pages)

def extract_text (page_number=0):
	return ''.join(x.pages[page_number].text)

def extract_positions (page_number=0):
	return x.pages[page_number].bbox
