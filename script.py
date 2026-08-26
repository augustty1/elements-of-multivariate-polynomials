"""
Given an SMT-LIB2 benchmark whose (assert ...) is a conjuction (and c1 c2 ... cn) of polynomials 
(in)equalities like QF_NRA this scripts:
(i) Pareses the file as an S-expression
(ii) Recursively flattens every (and ...) found inside the assert(s) into a flat list of individual constrainst c1, c2, ... , cn
(iii) Normalizes eah constraint into the form p(x1,x2,...,xn) >= 0 (for the sos)
(iv) Writes one .smt file per contraint
"""
import argparse
import os
import sys

RELATIONS = {"=", "<", "<=", ">", ">="}
DELIMITERS = set(" \t\r\n();|\"")

def tokenize(s):
    tokens = []
    i, n = 0, len(s)
    while i < n:
        c = s[i]
        if c in " \t\r\n":
            i += 1
        elif c == ";":
            while i < n and s[i] != "\n":
                i += 1
        elif c in "()":
            tokens.append(c)
            i += 1
        elif c == "|":
            j = s.index("|", i + 1)
            tokens.append(s[i:j + 1])
            i = j + 1
        elif c == '"':
            j = i + 1
            while True:
                j = s.index('"', j)
                if j + 1 < n and s[j + 1] == '"':
                    j+=2
                    continue
                break
            tokens.append(s[i:j + 1])
            i = j + 1
        else:
            j = i
            while j < n and s[j] not in DELIMITERS:
                j += 1
            tokens.append(s[i:j])
            i = j
    return tokens

def parse(tokens):
    pos = 0
    def read():
        nonlocal pos
        tok = tokens[pos]
        pos += 1
        if tok == "(":
            out = []
            while tokens[pos] != ")":
                out.append(read())
            pos += 1
            return out
        return tok
    forms = []
    while pos < len(tokens):
        forms.append(read())
    return forms

def to_str(e):
    if isinstance(e, list):
        return "(" + " ".join(to_str(x) for x in e) + ")"
    return e

def flatten_and(e):
    if isinstance(e, list) and e and e[0] == "and":
        out = []
        for a in e[1:]:
            out += flatten_and(a)
        return out
    return [e]

def collect_vars(e, out):
    if isinstance(e, list):
        for x in e:
            collect_vars(x, out)
    else:
        out.add(e)

def relation_pairs(c):
    if not (isinstance(c, list) and len(c) >= 3 and c[0] in RELATIONS):
        return None
    op, *args = c
    return [(op, args[i], args[i + 1]) for i in range(len(args) - 1)]

def normalize(op, a, b):
    return ["-", a, b] if op in (">=", ">", "=") else ["-", b, a]

def process(path, outdir):
    forms = parse(tokenize(open(path).read()))

    logic = "QF_NRA"
    decls = {}
    asserts = []
    for f in forms:
        if not isinstance(f, list) or not f:
            continue
        h = f[0]
        if h == "set-logic":
            logic = f[1]
        elif h in ("declare-fun", "declare-const"):
            decls[f[1]] = f
        elif h == "assert":
            asserts.append(f[1])
    constraints = []
    for a in asserts:
        constraints += flatten_and(a)
    base = os.path.splitext(os.path.basename(path))[0]
    os.makedirs(outdir, exist_ok=True)
    written = []
    idx=0
    for c in constraints:
        pairs = relation_pairs(c)
        if pairs is None:
            print(f": {to_str(c)[:60]}", file=sys.stderr)
            continue
        for op, a, b in pairs:
            idx += 1
            poly = normalize(op, a, b)
            used = set()
            collect_vars(poly, used)

            out_path = os.path.join(outdir, f"{base}_{idx}.smt2")
            with open(out_path, "w") as f:
                f.write(f"(set-logic {logic})\n")
                for v, d in decls.items():
                    if v in used:
                        f.write(to_str(d) + "\n")
                f.write(f"(assert (>= {to_str(poly)} 0))\n")
                f.write("(check-sat)\n(exit)\n")
            written.append(out_path)
    return written

if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("input")
    ap.add_argument("-o", "--outdir", default="split_out")
    ap.add_argument("--relations", default=",".join(RELATIONS),
                     help="binary relations")
    args = ap.parse_args()
    RELATIONS.clear()
    RELATIONS.update(args.relations.split(","))
    written = process(args.input, args.outdir)
    print(f"ok")
