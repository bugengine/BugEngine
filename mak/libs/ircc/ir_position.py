class IrPosition:
    def __init__(self, filename, line_number, start, end, lexdata):
        # type: (str, int, int, int, str) -> None
        self.filename = filename
        self.line_number = line_number
        self.start_position = start
        self.end_position = end
        self.lexdata = lexdata