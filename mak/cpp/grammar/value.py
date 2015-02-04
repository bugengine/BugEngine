import cpp

class Constant(cpp.yacc.Nonterm):
    "%nonterm"

    def constant_string(self, constant):
        "%reduce STRING"
        self.value = constant.value
    def constant_wstring(self, constant):
        "%reduce WSTRING"
        self.value = constant.value
    def constant_charconst(self, constant):
        "%reduce CHARCONST"
        self.value = constant.value
    def constant_wchar(self, constant):
        "%reduce WCHAR"
        self.value = constant.value
    def constant_decimal(self, constant):
        "%reduce DECIMAL"
        self.value = constant.value
    def constant_octal(self, constant):
        "%reduce OCTAL"
        self.value = constant.value
    def constant_hex(self, constant):
        "%reduce HEX"
        self.value = constant.value
    def constant_floating(self, constant):
        "%reduce FLOATING"
        self.value = constant.value

class Value(cpp.yacc.Nonterm):
    "%nonterm"
    def value(self, skip):
        "%reduce SkipListNoComma"
        self.value = skip.value

