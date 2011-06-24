import sys
import ply.yacc
import ply.lex
import os
import rtti

from optparse import OptionParser

options = OptionParser()
options.set_usage('ddf.py [options] file1 [file2... fileN]')
options.add_option("-o", "--output", dest="folder", help="Places the output into <folder>", default='')
options.add_option("--cpp", dest="cpp", help="extension used for source implementation", default='.cc')
options.add_option("-d", dest="macro", action="append", help="define <macro> so that it will be removed during parsing")
options.add_option("-D", dest="macrofile", action="append", help="add the content of <macrofile> to the macros, one macro per line")
options.add_option("-p", "--pch", dest="pch", help="insert an include for precompiled header at the start of the file")


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

def doParse(source, output, temppath, macro = [], macrofile = [], pch=""):
	import cpp.lexer, cpp.parser
	lexer = ply.lex.lex(module=cpp.lexer)
	lexer.inside = 0
	lexer.sourcename = source
	lexer.error = 0
	yacc = ply.yacc.yacc(module=cpp.parser, method='LALR', debugfile='parser.out', picklefile=os.path.join(temppath, 'parsetab'), outputdir=temppath)
	yacc.namespace = rtti.Root(source)
	yacc.rtti = rtti

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


	yacc.parse(input.read(), lexer=lexer, debug=0)
	input.close()

	if lexer.error != 0:
		return lexer.error

	try:
		implementation = open(output, 'w')
	except IOError as e:
		raise Exception("cannot open output file %s : %s" % (output, str(e)))
	if pch:
		implementation.write("#include <%s>\n" % pch)
	yacc.namespace.dump(implementation)

	return 0


if __name__ == '__main__':
	(options, args) = options.parse_args()
	if not args:
		options.print_help()

	for arg in args:
		base,ext = os.path.splitext(arg)
		path,filename = os.path.split(base)
		sourcename = arg

		if options.folder:
			outputname = os.path.join(options.folder, filename+options.cpp)
		else:
			outputname = os.path.join(base+options.cpp)
		if os.path.normpath(outputname) == os.path.normpath(sourcename):
			raise Exception("source file and target file are the same: %s" % outputname)

		path = os.path.abspath(os.path.split(sys.argv[0])[0])
		if doParse(sourcename, outputname, path, options.macro, options.macrofile, options.pch) > 0:
			exit(1)
	exit(0)

