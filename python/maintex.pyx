# distutils: language=c++
from boilerpy3 import extractors

cpdef public str maintexExtraction(url):
    extractor = extractors.ArticleExtractor()
    doc = extractor.get_doc_from_url(url)
    cpdef str title = doc.title
    cpdef str content = doc.content
    return content