#!/usr/bin/env python

import codecs
from collections import defaultdict
import json
from pprint import pprint
import re
import sys
from subprocess import Popen, PIPE

template_dir = (
    '/Applications'
    '/Xcode.app'
    '/Contents'
    '/Developer'
    '/Library'
    '/Xcode'
    '/Templates'
    '/Project Templates'
    )

def shorten(fname):
    pat = r'\A%s/' % template_dir
    fname = re.sub(pat, '', fname)
    fname = re.sub(r'/TemplateInfo.plist\Z', '/', fname)
    return fname

def template_files():
    cmd = ['locate', '.xctemplate']
    for line in Popen(cmd, stdout=PIPE).stdout:
        line = line.decode().strip()
        if line.endswith('.xctemplate'):
            yield line

    cmd = 'find ~/Library/Developer/Xcode/Templates -name \*.xctemplate'
    for line in Popen(cmd, shell=True, stdout=PIPE).stdout:
        line = line.decode().strip()
        yield line

def load_templates():
    d = dict()
    for f in template_files():
        ti = f + '/TemplateInfo.plist'
        # print ('file \'%s/TemplateInfo.plist\'' % f)
        cmd = 'plutil -convert json -o - -r'.split() + [ti]
        f = json.load(codecs.getreader('utf-8')(Popen(cmd, stdout=PIPE).stdout))
        if 'ProjectTemplate' not in f.get('Kind', ''):
            continue
        d[ti] = f
    return d

def filter_templates(templates, predicate):
    return {file: temp
            for (file, temp) in templates.items()
            if predicate(file, temp)}

def template_dict(templates):
    return {t['Identifier']: t for t in templates.values()}

def parent_dict(templates):
    return {i: t.get('Ancestors', [])
            for (i, t) in template_dict(templates).items()}

def descendant_dict(templates):
    dd = defaultdict(list)
    for (ident, temp) in template_dict(templates).items():
        for anc in temp.get('Ancestors', []):
            dd[anc].append(temp['Identifier'])
    return dd
    
def patriarchs(templates):
    roots = []
    for temp in templates.values():
        ident = temp['Identifier']
        if not temp.get('Ancestors'):
            roots.append(ident)
    return roots

def ancestors(templates, ident):
    td = template_dict(templates)
    a = [ident]
    def add_parents(ident):
        new_parents = [p for p in td[ident].get('Ancestors', []) if p not in a]
        a.extend(new_parents)
        for p in new_parents:
            add_parents(p)
    add_parents(ident)
    return a

def is_project_template(file, template):
    return template.get('Kind', '') == 'Xcode.Xcode3.ProjectTemplateUnitKind'

def print_identifiers(templates):
    # idents = [t['Identifier'] + ' ' + f for (f, t) in templates.items()]
    idents = [t['Identifier'] for t in templates.values()]
    for ident in sorted(idents):
        print(ident)

def print_keys(templates):
    keys = set()
    for temp in templates.values():
        keys |= temp.keys()
    for key in sorted(keys):
        print(key)

def print_hierarchy(templates):
    dd = descendant_dict(templates)
    roots = patriarchs(templates)
    seen = set()
    def print_tree(node, indent=''):
        print('%s%s' % (indent, node))
        if node in seen:
            print('%s[...]' % (indent + '  '))
        else:
            seen.add(node)
            for desc in sorted(dd[node]):
                print_tree(desc, indent + '  ')
    for r in sorted(roots):
        print_tree(r)

def print_sort_orders(templates):
    for (f, temp) in sorted(templates.items()):
        print(temp.get('SortOrder', ' '), temp['Identifier'])

def print_concretenesses(templates):
    for (f, temp) in sorted(templates.items()):
        print('%5s %s' % (temp.get('Concrete', ' '), temp['Identifier']))
        if temp.get('Concrete') is False:
            print('     ', temp.get('Description'))

def print_option_types(templates):
    types = set()
    for temp in templates.values():
        for opt in temp.get('Options', ()):
            typ = opt.get('Type')
            if typ:
                types.add(typ)
    for typ in sorted(types):
        print(typ)

def print_ancestors(ident, templates):
    id2f = {t['Identifier']: f for (f, t) in templates.items()}
    for anc in ancestors(templates, ident):
        print('%-30s %s' % (anc, id2f[anc]))

def print_ancestor_nodes(ident, templates):
    td = template_dict(templates)
    a = ancestors(templates, ident)
    id2f = {t['Identifier']: f for (f, t) in templates.items()}
    for i in a:
        print(i)
        for j in td[i].get('Nodes', []):
            print('   ', j)
        dir = shorten(id2f[i])
        print(dir)
        sys.stdout.flush()
        cmd = "cd '%s/%s' && find * -type f" % (template_dir, dir)
        find = Popen(cmd, shell=True, stdout=PIPE)
        for fname in sorted(codecs.getreader('utf-8')(find.stdout)):
            print('   ', fname.rstrip())
        find.wait()
        print()


def print_keys(v):
    for vv in v.values():
        if isinstance(vv, dict):
            for (k, v) in vv.items():
                print('%s: %s' % (k, v))


def print_description_keys(templates):
    def walk_template(temp):
        if isinstance(temp, dict):
            for (k, v) in temp.items():
                if k == 'Definitions':
                    print_keys(v)
                else:
                    walk_template(v)
        elif isinstance(temp, list):
            for i in temp:
                walk_template(i)

    for (f, temp) in templates.items():
        print(f)
        walk_template(temp)
        print()

my_ident = 'com.example.dt.unit.audiounit.aufx'
my_ident = 'com.apple.dt.unit.cocoaApplication'

templates = load_templates()
project_templates = filter_templates(templates, is_project_template)
# print_identifiers(templates)
# print_keys(project_templates)
# print_hierarchy(project_templates)
# print_sort_orders(project_templates)
# print_concretenesses(project_templates)
# print_option_types(project_templates)
# print_ancestors(my_ident, project_templates)
# print_ancestor_nodes(my_ident, project_templates)
print_description_keys(project_templates)


