import cpp


class Unit(cpp.yacc.Nonterm):
	"%start"

	def __init__(self, parser):
		cpp.yacc.Nonterm.__init__(self, parser)
		parser.root = self

	def exprs(self, exprs):
		"%reduce Exprs"
		self.members = exprs

	def dump(self, file, instances):
		instances.write("#include    <rtti/stdafx.h>\n")
		instances.write("#include    <%s>\n" % self.parser.source)
		instances.write("#include    <rtti/typeinfo.hh>\n")
		instances.write("#include    <rtti/classinfo.script.hh>\n")
		#instances.write("#line 1 \"%s\"\n" % (self.parser.source.replace("\\", "\\\\")))

		file.write("#include    <rtti/stdafx.h>\n")
		file.write("#include    <%s>\n" % self.parser.source)
		file.write("#include    <rtti/typeinfo.hh>\n")
		file.write("#include    <rtti/typeinfo.inl>\n")
		file.write("#include    <rtti/value.hh>\n")
		file.write("#include    <rtti/value.inl>\n")
		file.write("#include    <rtti/classinfo.script.hh>\n")
		file.write("#include    <rtti/engine/array.hh>\n")
		file.write("#include    <rtti/engine/objectinfo.script.hh>\n")
		file.write("#include    <rtti/engine/methodinfo.script.hh>\n")
		file.write("#include    <rtti/engine/propertyinfo.script.hh>\n")
		file.write("#include    <rtti/engine/taginfo.script.hh>\n")
		file.write("#include    <rtti/engine/helper/get.hh>\n")
		file.write("#include    <rtti/engine/helper/set.hh>\n")
		file.write("#include    <rtti/engine/helper/method.hh>\n")
		file.write("#include    <rtti/tags/documentation.script.hh>\n")
		file.write("\n")
		file.write("\n")
		#file.write("#line 1 \"%s\"\n" % (self.parser.source.replace("\\", "\\\\")))

		self.members.predecl((file, instances), [], [], False)
		instances.write("namespace BugEngine\n{\n\n")
		self.members.dumpObjects((file, instances), [], [])
		self.members.dump((file, instances), [], [], '::BugEngine::be_%s_Namespace()'%self.parser.plugin)
		instances.write("\n}\n\n")





