# subset of plistlib functionality from official Python

import binascii
import codecs
import datetime
from io import BytesIO
import itertools
import os
import re
import struct
from xml.parsers.expat import ParserCreate



#
# XML support
#


# XML 'header'
PLISTHEADER = b"""\
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
"""


# Regex to find any control chars, except for \t \n and \r
_controlCharPat = re.compile(
    r"[\x00\x01\x02\x03\x04\x05\x06\x07\x08\x0b\x0c\x0e\x0f"
    r"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f]")

def _encode_base64(s, maxlinelength=76):
    # copied from base64.encodebytes(), with added maxlinelength argument
    maxbinsize = (maxlinelength//4)*3
    pieces = []
    for i in range(0, len(s), maxbinsize):
        chunk = s[i : i + maxbinsize]
        pieces.append(binascii.b2a_base64(chunk))
    return b''.join(pieces)

def _decode_base64(s):
    if isinstance(s, str):
        return binascii.a2b_base64(s.encode("utf-8"))

    else:
        return binascii.a2b_base64(s)

# Contents should conform to a subset of ISO 8601
# (in particular, YYYY '-' MM '-' DD 'T' HH ':' MM ':' SS 'Z'.  Smaller units
# may be omitted with #  a loss of precision)
_dateParser = re.compile(r"(?P<year>\d\d\d\d)(?:-(?P<month>\d\d)(?:-(?P<day>\d\d)(?:T(?P<hour>\d\d)(?::(?P<minute>\d\d)(?::(?P<second>\d\d))?)?)?)?)?Z")


def _date_from_string(s):
    order = ('year', 'month', 'day', 'hour', 'minute', 'second')
    gd = _dateParser.match(s).groupdict()
    lst = []
    for key in order:
        val = gd[key]
        if val is None:
            break
        lst.append(int(val))
    return datetime.datetime(*lst)


def _date_to_string(d):
    return '%04d-%02d-%02dT%02d:%02d:%02dZ' % (
        d.year, d.month, d.day,
        d.hour, d.minute, d.second
    )

def _escape(text):
    m = _controlCharPat.search(text)
    if m is not None:
        raise ValueError("strings can't contains control characters; "
                         "use bytes instead")
    text = text.replace("\r\n", "\n")       # convert DOS line endings
    text = text.replace("\r", "\n")         # convert Mac line endings
    text = text.replace("&", "&amp;")       # escape '&'
    text = text.replace("<", "&lt;")        # escape '<'
    text = text.replace(">", "&gt;")        # escape '>'
    return text

class _PlistParser:
    def __init__(self):
        self.stack = []
        self.current_key = None
        self.root = None

    def parse(self, fileobj):
        self.parser = ParserCreate()
        self.parser.StartElementHandler = self.handle_begin_element
        self.parser.EndElementHandler = self.handle_end_element
        self.parser.CharacterDataHandler = self.handle_data
        self.parser.ParseFile(fileobj)
        return self.root

    def handle_begin_element(self, element, attrs):
        self.data = []
        handler = getattr(self, "begin_" + element, None)
        if handler is not None:
            handler(attrs)

    def handle_end_element(self, element):
        handler = getattr(self, "end_" + element, None)
        if handler is not None:
            handler()

    def handle_data(self, data):
        self.data.append(data)

    def add_object(self, value):
        if self.current_key is not None:
            if not isinstance(self.stack[-1], type({})):
                raise ValueError("unexpected element at line %d" %
                                 self.parser.CurrentLineNumber)
            self.stack[-1][self.current_key] = value
            self.current_key = None
        elif not self.stack:
            # this is the root object
            self.root = value
        else:
            if not isinstance(self.stack[-1], type([])):
                raise ValueError("unexpected element at line %d" %
                                 self.parser.CurrentLineNumber)
            self.stack[-1].append(value)

    def get_data(self):
        data = ''.join(self.data)
        self.data = []
        return data

    # element handlers

    def begin_dict(self, attrs):
        d = dict()
        self.add_object(d)
        self.stack.append(d)

    def end_dict(self):
        if self.current_key:
            raise ValueError("missing value for key '%s' at line %d" %
                             (self.current_key,self.parser.CurrentLineNumber))
        self.stack.pop()

    def end_key(self):
        if self.current_key or not isinstance(self.stack[-1], type({})):
            raise ValueError("unexpected key at line %d" %
                             self.parser.CurrentLineNumber)
        self.current_key = self.get_data()

    def begin_array(self, attrs):
        a = []
        self.add_object(a)
        self.stack.append(a)

    def end_array(self):
        self.stack.pop()

    def end_true(self):
        self.add_object(True)

    def end_false(self):
        self.add_object(False)

    def end_integer(self):
        self.add_object(int(self.get_data()))

    def end_real(self):
        self.add_object(float(self.get_data()))

    def end_string(self):
        self.add_object(self.get_data())

    def end_data(self):
        self.add_object(_decode_base64(self.get_data()))

    def end_date(self):
        self.add_object(_date_from_string(self.get_data()))


def _is_fmt_xml(header):
    prefixes = (b'<?xml', b'<plist')

    for pfx in prefixes:
        if header.startswith(pfx):
            return True

    # Also check for alternative XML encodings, this is slightly
    # overkill because the Apple tools (and plistlib) will not
    # generate files with these encodings.
    for bom, encoding in (
                (codecs.BOM_UTF8, "utf-8"),
                (codecs.BOM_UTF16_BE, "utf-16-be"),
                (codecs.BOM_UTF16_LE, "utf-16-le"),
                # expat does not support utf-32
                #(codecs.BOM_UTF32_BE, "utf-32-be"),
                #(codecs.BOM_UTF32_LE, "utf-32-le"),
            ):
        if not header.startswith(bom):
            continue

        for start in prefixes:
            prefix = bom + start.decode('ascii').encode(encoding)
            if header[:len(prefix)] == prefix:
                return True

    return False

#
# Binary Plist
#


class InvalidFileException (ValueError):
    def __init__(self, message="Invalid file"):
        ValueError.__init__(self, message)

_BINARY_FORMAT = {1: 'B', 2: 'H', 4: 'L', 8: 'Q'}

_undefined = object()

class _BinaryPlistParser:
    """
    Read or write a binary plist file, following the description of the binary
    format.  Raise InvalidFileException in case of error, otherwise return the
    root object.

    see also: http://opensource.apple.com/source/CF/CF-744.18/CFBinaryPList.c
    """
    def __init__(self):
        pass

    def parse(self, fp):
        try:
            # The basic file format:
            # HEADER
            # object...
            # refid->offset...
            # TRAILER
            self._fp = fp
            self._fp.seek(-32, os.SEEK_END)
            trailer = self._fp.read(32)
            if len(trailer) != 32:
                raise InvalidFileException()
            (
                offset_size, self._ref_size, num_objects, top_object,
                offset_table_offset
            ) = struct.unpack('>6xBBQQQ', trailer)
            self._fp.seek(offset_table_offset)
            self._object_offsets = self._read_ints(num_objects, offset_size)
            self._objects = [_undefined] * num_objects
            return self._read_object(top_object)

        except (OSError, IndexError, struct.error, OverflowError,
                UnicodeDecodeError):
            raise InvalidFileException()

    def _get_size(self, tokenL):
        """ return the size of the next object."""
        if tokenL == 0xF:
            m = self._fp.read(1)[0] & 0x3
            s = 1 << m
            f = '>' + _BINARY_FORMAT[s]
            return struct.unpack(f, self._fp.read(s))[0]

        return tokenL

    def _read_ints(self, n, size):
        data = self._fp.read(size * n)
        if size in _BINARY_FORMAT:
            return struct.unpack('>' + _BINARY_FORMAT[size] * n, data)
        else:
            if not size or len(data) != size * n:
                raise InvalidFileException()
            return tuple(int.from_bytes(data[i: i + size], 'big')
                         for i in range(0, size * n, size))

    def _read_refs(self, n):
        return self._read_ints(n, self._ref_size)

    def _read_object(self, ref):
        """
        read the object by reference.

        May recursively read sub-objects (content of an array/dict/set)
        """
        result = self._objects[ref]
        if result is not _undefined:
            return result

        offset = self._object_offsets[ref]
        self._fp.seek(offset)
        token = self._fp.read(1)[0]
        tokenH, tokenL = token & 0xF0, token & 0x0F

        if token == 0x00:
            result = None

        elif token == 0x08:
            result = False

        elif token == 0x09:
            result = True

        # The referenced source code also mentions URL (0x0c, 0x0d) and
        # UUID (0x0e), but neither can be generated using the Cocoa libraries.

        elif token == 0x0f:
            result = b''

        elif tokenH == 0x10:  # int
            result = int.from_bytes(self._fp.read(1 << tokenL),
                                    'big', signed=tokenL >= 3)

        elif token == 0x22: # real
            result = struct.unpack('>f', self._fp.read(4))[0]

        elif token == 0x23: # real
            result = struct.unpack('>d', self._fp.read(8))[0]

        elif token == 0x33:  # date
            f = struct.unpack('>d', self._fp.read(8))[0]
            # timestamp 0 of binary plists corresponds to 1/1/2001
            # (year of Mac OS X 10.0), instead of 1/1/1970.
            result = (datetime.datetime(2001, 1, 1) +
                      datetime.timedelta(seconds=f))

        elif tokenH == 0x40:  # data
            s = self._get_size(tokenL)
            result = self._fp.read(s)

        elif tokenH == 0x50:  # ascii string
            s = self._get_size(tokenL)
            result =  self._fp.read(s).decode('ascii')
            result = result

        elif tokenH == 0x60:  # unicode string
            s = self._get_size(tokenL)
            result = self._fp.read(s * 2).decode('utf-16be')

        # tokenH == 0x80 is documented as 'UID' and appears to be used for
        # keyed-archiving, not in plists.

        elif tokenH == 0xA0:  # array
            s = self._get_size(tokenL)
            obj_refs = self._read_refs(s)
            result = []
            self._objects[ref] = result
            result.extend(self._read_object(x) for x in obj_refs)

        # tokenH == 0xB0 is documented as 'ordset', but is not actually
        # implemented in the Apple reference code.

        # tokenH == 0xC0 is documented as 'set', but sets cannot be used in
        # plists.

        elif tokenH == 0xD0:  # dict
            s = self._get_size(tokenL)
            key_refs = self._read_refs(s)
            obj_refs = self._read_refs(s)
            result = dict()
            self._objects[ref] = result
            for k, o in zip(key_refs, obj_refs):
                result[self._read_object(k)] = self._read_object(o)

        else:
            raise InvalidFileException()

        self._objects[ref] = result
        return result


def _is_fmt_binary(header):
    return header[:8] == b'bplist00'


#
# Generic bits
#

_FORMATS={
    'FMT_XML': dict(
        detect=_is_fmt_xml,
        parser=_PlistParser,
    ),
    'FMT_BINARY': dict(
        detect=_is_fmt_binary,
        parser=_BinaryPlistParser,
    )
}


def load(fp):
    """Read a .plist file. 'fp' should be a readable and binary file object.
    Return the unpacked root object (which usually is a dictionary).
    """
    header = fp.read(32)
    fp.seek(0)
    for info in _FORMATS.values():
        if info['detect'](header):
            P = info['parser']
            break

    else:
        raise InvalidFileException()

    p = P()
    return p.parse(fp)
