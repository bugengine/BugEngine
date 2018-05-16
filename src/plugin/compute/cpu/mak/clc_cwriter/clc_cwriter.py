class ClcCWriter:
    def __init__(self, out_file):
        self.out_file = out_file

    def begin_document(self):
        pass

    def end_document(self):
        self.out_file.write(b'\n')
