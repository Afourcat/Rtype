import sys
import CppHeaderParser
import collections
import re

UNTEMPLATE = re.compile("^(.*)<(.*)>$")

data = ["classDiagram"]
for file in sys.argv[1:]:
    cppHeader = CppHeaderParser.CppHeader(file)
    for cppClass in cppHeader.classes:
        data.append("class " + cppClass)
        for inheritance in cppHeader.classes[cppClass]['inherits']:
            template = "{}"
            name = inheritance['class']
            while name.find("<") != -1:
                template = template.format(name[:name.find("<")] + "<{}>")
                name = name[name.find("<") + 1:name.find(">")]
            if template != "{}":
                data.append("{} --|> {} : {}".format(cppClass, name, template.format("T")))
            else:
                data.append("{} --|> {}".format(cppClass, name))
        for publicMethod in cppHeader.classes[cppClass]['methods']['public']:
            data.append("{} : +{} {}({})".format(
                cppClass,
                publicMethod['rtnType'],
                publicMethod['name'],
                ", ".join(map(lambda x: "{} {}".format(x["type"], x["name"]), publicMethod["parameters"]))
            ))
            if 'A' <= publicMethod["rtnType"][0] <= 'Z':
                data.append("{} --o {} : produces".format(cppClass, publicMethod["rtnType"]))
        for privateMethod in cppHeader.classes[cppClass]['methods']['private']:
            if 'A' <= privateMethod["rtnType"][0] <= 'Z':
                data.append("{} --o {} : produces".format(cppClass, privateMethod["rtnType"]))
        for publicProperty in cppHeader.classes[cppClass]['properties']['public']:
            data.append("{} : +{} {}".format(
                cppClass,
                publicProperty['type'],
                publicProperty['name'],
            ))
            if 'A' <= publicProperty["type"][0] <= 'Z':
                search = UNTEMPLATE.search(publicProperty["type"])
                data.append("{} --o {} : contains".format(cppClass, publicProperty["type"] if search is None else search.groups()[0]))
        for privateProperty in cppHeader.classes[cppClass]['properties']['private']:
            if 'A' <= privateProperty["type"][0] <= 'Z':
                search = UNTEMPLATE.search(privateProperty["type"])
                data.append("{} --o {} : contains".format(cppClass, privateProperty["type"] if search is None else search.groups()[0]))

new_data = sorted(set(data), key=data.index)
new_data = filter(lambda x: x.find("--o T") == -1, new_data)
print("\n".join(new_data).replace("* ", "").replace("& ", "").replace(" *", "").replace(" &", "").replace("::", "_").replace("[ [ nodiscard ] ] ", ""))