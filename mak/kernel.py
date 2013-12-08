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
		kernel_name = os.path.splitext(os.path.splitext(os.path.basename(output))[0])[0]
		fullname = [i.capitalize() for i in name.split('_')] + [kernel_name.capitalize()]
		implementation.write('/* BugEngine / 2008-2012  Nicolas MERCIER <mercier.nicolas@gmail.com>\n')
		implementation.write('   see LICENSE for detail */\n\n')
		implementation.write('#ifndef BE_%s_%s_SCRIPT_HH_\n'%(name.upper(), kernel_name.upper()))
		implementation.write('#define BE_%s_%s_SCRIPT_HH_\n'%(name.upper(), kernel_name.upper()))
		implementation.write('/*****************************************************************************/\n')
		if pch:
			implementation.write("#include    <%s>\n" % pch)
		implementation.write('#include    <scheduler/kernel/kernel.script.hh>\n')
		implementation.write('#include    <scheduler/task/itask.hh>\n')
		implementation.write('#include    <scheduler/kernel/product.hh>\n')

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

		implementation.write('\n')
		for ns in fullname[:-1]:
			implementation.write('namespace %s { ' % ns)
		implementation.write('namespace Kernels\n{\n\n')
		implementation.write('class %s : public BugEngine::Kernel::KernelDescription\n{\n' % fullname[-1])
		implementation.write('};\n\n')
		implementation.write('}' * len(fullname))
		implementation.write('\n\n/*****************************************************************************/\n')
		implementation.write('#endif\n')

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

