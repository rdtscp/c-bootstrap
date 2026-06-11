# TODO: ALl nodes

def PrintASTNode(astnode, internal_dict):
    position = astnode.GetChildMemberWithName("position")
    filepath = position.GetChildMemberWithName("file").GetSummary()
    filepath = str(filepath).replace("\"", "")
    line = position.GetChildMemberWithName("line").GetValueAsUnsigned()
    column = position.GetChildMemberWithName("column").GetValueAsUnsigned()
    return "{}:{}:{}".format(filepath, line, column)

def PrintOmitted(decl, internal_dict):
    return "ACC::Omitted"

def PrintIdentifier(identifier, internal_dict):
    value = identifier.GetChildMemberWithName("value").GetSummary()
    parentIdentifier = identifier.GetChildMemberWithName("parentIdentifier").GetChildMemberWithName("ptr").Dereference().GetSummary()
    if parentIdentifier == None:
        return value
    else:
        return "{}::{}".format(parentIdentifier, value)

def PrintNamespace(namespace, internal_dict):
    return namespace
    #identifier = namespace.GetChildMemberWithName("identifier").GetSummary()
    #decls = namespace.GetChildMemberWithName("namespaceDecls").GetSummary()
    #return str(identifier) + "{\n" + str(decls) + "\n}"

def PrintScope(scope, internal_dict):
    outerScope = scope.GetChildMemberWithName("outerScope").GetChildMemberWithName("ptr").GetValue()
    return "(outerScope: {})".format(outerScope)
    


def PrintVector(vector, internal_dict):
    num_elems = vector.GetChildMemberWithName("elements_used")
    
    return num_elems.GetValue()

class VectorSynthProvider:
    def __init__(self, valobj, dict):
        self.valobj = valobj;
        self.update() # initialize this provider

    def num_children(self):
        print(self.elements_used)
        return self.elements_used
    
    def get_child_index(self, name):
        try:
            return int(name.lstrip('[').rstrip(']'))
        except:
            return -1;

    def get_child_at_index(self, index):
        # Do bounds checking.
        if index < 0:
            return None
        if index >= self.num_children():
            return None;

        offset = index * self.type_size
        return self.begin.CreateChildAtOffset('['+str(index)+']',
                                              offset, self.data_type)

    def get_type_from_name(self):
        import re
        name = self.valobj.GetType().GetName()
        res = re.match("^(atl::)?vector<(.+)>$", name)
        if res:
            return res.group(2)
        res = re.match("^(atl::)?vector<(.+), \d+>$", name)
        if res:
            return res.group(2)
        return None

    def update(self):
        self.begin = self.valobj.GetChildMemberWithName('elements')
        self.elements_used = self.valobj.GetChildMemberWithName('elements_used').GetValue()
        data_type = self.get_type_from_name()
        print("data_type: {}".format(data_type))
        self.data_type = self.valobj.GetTarget().FindFirstType(data_type)
        print("self.data_type: {}".format(self.data_type))
        self.type_size = self.data_type.GetByteSize()
        print("self.type_size: {}".format(self.type_size))
        

def __lldb_init_module(debugger, internal_dict):
    debugger.HandleCommand("type summary add -F ast_debugging.PrintASTNode ACC::ASTNode")
    debugger.HandleCommand("type summary add -F ast_debugging.PrintOmitted ACC::Decl")
    debugger.HandleCommand("type summary add -F ast_debugging.PrintIdentifier ACC::Identifier")
    debugger.HandleCommand("type summary add -F ast_debugging.PrintNamespace ACC::Namespace")
    debugger.HandleCommand("type summary add -F ast_debugging.PrintString atl::string")
    debugger.HandleCommand("type summary add -F ast_debugging.PrintScope ACC::Scope")
    debugger.HandleCommand('type summary add -F ast_debugging.PrintOmitted -x "^atl::enable_shared_from_this<.+>$"')

    debugger.HandleCommand('type synthetic add -x "^atl::vector<.+>$" -l ast_debugging.VectorSynthProvider')
    return
