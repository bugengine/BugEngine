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

def split_type(name):
	name = name.strip()
	template_begin = name.find('<')
	if template_begin == -1:
		raise Exception('invalid kernel input type: %s' % name)
	template_name = name[0:template_begin].strip()
	type_name = name[template_begin+1:-1].strip()
	if template_name not in ['in', 'out', 'inout']:
		raise Exception('invalid kernel input type: %s' % name)
	return (template_name, type_name)

def doParse(source, output, macro = [], macrofile = [], pch="", name=""):
	with open(output, 'w') as implementation:
		kernel_name = os.path.splitext(os.path.splitext(os.path.basename(output))[0])[0]
		fullname = [i.capitalize() for i in name.split('_')] + [kernel_name.capitalize()]
		idname = [i for i in name.split('_')] + [kernel_name]
		if pch:
			implementation.write("#include    <%s>\n" % pch)
		implementation.write("#include    <kernel/compilers.hh>\n")
		implementation.write("#include    <kernel/simd.hh>\n")
		implementation.write("#include    <kernel/input.hh>\n")
		implementation.write("#include    <plugin/dynobjectlist.hh>\n")
		implementation.write("using namespace Kernel;\n")
		implementation.write("#include \"%s\"\n" % source)
		lexer = cpp.lex.lex(module=cpp.lexer)
		lexer.inside = 0
		lexer.sourcename = source
		lexer.error = 0
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
		arguments = []
		arg0 = m.value.args.args[0]
		if arg0.type != 'u32' and arg0.type != 'const u32':
			raise Exception("invalid signature for method kmain")
		arg1 = m.value.args.args[1]
		if arg1.type != 'u32' and arg1.type != 'const u32':
			raise Exception("invalid signature for method kmain")
		for arg in m.value.args.args[2:]:
			template_name, type_name = split_type(arg.type)
			arguments.append((arg.name, type_name, template_name))

		implementation.write('\n')
		implementation.write('struct Parameter\n')
		implementation.write('{\n')
		implementation.write('    void* begin;\n')
		implementation.write('    void* end;\n')
		implementation.write('};\n')
		implementation.write('_BE_PLUGIN_EXPORT void _kmain(const u32 index, const u32 total, Parameter argv[])\n')
		implementation.write('{\n')
		implementation.write('    kmain(index, total')
		i = 0
		for arg_name, arg_type, input_type in arguments:
			implementation.write(',\n        %s< %s >((%s*)argv[%d].begin, (%s*)argv[%d].end)' % (input_type, arg_type, arg_type, i, arg_type, i))
			i += 1
		implementation.write('\n    );\n')
		implementation.write('}\n')
		implementation.write('_BE_REGISTER_PLUGIN(BE_KERNELID, BE_KERNELNAME);\n')
		implementation.write('_BE_REGISTER_METHOD(BE_KERNELID, _kmain);\n')

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
		if doParse(sourcename, outputname, options.macro, options.macrofile, options.pch, options.namespace) > 0:
			exit(1)
	exit(0)

