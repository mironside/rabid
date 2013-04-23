# pyOL
#

def indent(f, i):
    for ii in range(0,i):
        f.write("  ")



class olDef:
    def __init__(self, t, n=""):
        self.type = t
        self.name = n
        self.dict = {}
        self.defs = []


    def __setitem__(self, key, value):
        if isinstance(value, olDef):
            print "### ERROR: Attempted to insert olDef into dict at '%s'. " \
                  "Stop it!" % (key)
            return
        self.dict[key] = value


    def __getitem__(self, key):
        if self.dict.has_key(key):
            return self.dict[key]
        return 0


    def add_def(self, d):
        self.defs.append(d)


    def get_defs(self):
        return self.defs



    # WRITE
    def write(self, f, i=0):
        indent(f,i)

        c0 = '['
        c1 = ']'
        if i == 0:
            c0 = '<'
            c1 = '>'

        f.write(c0 + self.type)
        if self.name != "":
            f.write(" " + self.name)
        f.write(c1 + "\n")

        if len(self.dict.keys()):
            indent(f, i)
            f.write("{\n")

            i += 1
            # write dict
            for k in self.dict.keys():
                item = self.dict[k]

                if isinstance(item, float):
                    indent(f,i)
                    f.write(k + ' = ' + str(item) + ';\n')
                if isinstance(item, int):
                    indent(f,i)
                    f.write(k + ' = ' + str(item) + ';\n')
                if isinstance(item, str):
                    indent(f,i)
                    f.write(k + ' = "' + str(item) + '";\n')

            for d in self.defs:
                f.write('\n')
                d.write(f, i)

            i -= 1
            indent(f, i)
            f.write("}\n")


    def read(self, f):
        # read ol from file
        print "this is unimplemented"
