import sys
import cpp
import os

from optparse import OptionParser

options = OptionParser()
options.set_usage('ddf.py [options] file1 [file2... fileN]')
options.add_option("-o", "--output", dest="file", help="Output file name", default='')
options.add_option("-d", dest="macro", action="append", help="define <macro> so that it will be removed during parsing")
options.add_option("-D", dest="macrofile", action="append", help="add the content of <macrofile> to the macros, one macro per line")
options.add_option("-p", "--pch", dest="pch", help="insert an include for precompiled header at the start of the file")
options.add_option("-n", "--namespace", dest="namespace", help="namespace root")


global_macro_map = {
	"__declspec": True,
	"__attribute__": True,
	"CALLBACK": False,
	"WINAPI": False,
	"__cdecl": False,
	"__fastcall": False,
	"__stdcall": False,
	"PASCAL": False,
}

def doParse(source, output, temppath, macro = [], macrofile = [], pch="", name=""):
	with open(output, 'w') as implementation:
		if pch:
			implementation.write("#include    <%s>\n" % pch)
		lexer = cpp.lex.lex(module=cpp.lexer)
		lexer.inside = 0
		lexer.sourcename = source
		lexer.error = 0
		lexer.output = implementation
		yacc = cpp.parser.Parser(output, '', name, source, pch)

		lexer.macro_map = dict(global_macro_map)
		if macro:
			for m in macro:
				if m.endswith('()'):
					lexer.macro_map[m[:-2].strip()] = True
				else:
					lexer.macro_map[m.strip()] = False
		if macrofile:
			for f in macrofile:
				try:
					macros = open(f, 'r')
				except IOError as e:
					raise Exception("cannot open macro file %s : %s" % (f, str(e)))
				for m in macros.readlines():
					m = m.strip()
					if m.endswith('()'):
						lexer.macro_map[m[:-2].strip()] = True
					else:
						lexer.macro_map[m.strip()] = False

		try:
			input = open(source, 'r')
		except IOError as e:
			raise Exception("cannot open input file %s : %s" % (source, str(e)))


		try:
			yacc.parse(input.read(), lexer=lexer)
			input.close()

			if lexer.error != 0:
				return lexer.error
		except:
			return 1

		try:
			m = yacc.root.members.methods['kmain']
		except KeyError:
			raise Exception("could not locate method kmain in kernel")
		if len(m) > 1:
			raise Exception("cannot overload metod kmain")
		m = m[0]
		arg0 = m.value.args.args[0]
		if arg0.type != 'u32' and arg0.type != 'const u32':
			raise Exception("invalid signature for method kmain")
		arg1 = m.value.args.args[1]
		if arg1.type != 'u32' and arg1.type != 'const u32':
			raise Exception("invalid signature for method kmain")
		for arg in m.value.args.args[2:]:
			print(arg.name)
	return 0

if __name__ == '__main__':
	(options, args) = options.parse_args()
	if not args:
		options.print_help()

	for arg in args:
		base,ext = os.path.splitext(arg)
		path,filename = os.path.split(base)
		sourcename = arg

		outputname = options.file
		if os.path.normpath(outputname) == os.path.normpath(sourcename):
			raise Exception("source file and target file are the same: %s" % outputname)

		path = os.path.abspath(os.path.split(sys.argv[0])[0])
		if doParse(sourcename, outputname, path, options.macro, options.macrofile, options.pch, options.namespace) > 0:
			exit(1)
	exit(0)

