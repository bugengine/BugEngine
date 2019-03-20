from io import BytesIO
from .document import Document


class ClcCWriter:
    def __init__(self, out_file):
        self.out_file = out_file

    def create_document(self):
        return Document(self.out_file)
