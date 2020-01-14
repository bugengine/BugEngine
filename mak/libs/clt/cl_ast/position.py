class Position:
    def __init__(self, filename, line_number, start, end):
        # type: (str, int, int, int) -> None
        self.filename = filename
        self.line_number = line_number
        self.start_position = start
        self.end_position = end
