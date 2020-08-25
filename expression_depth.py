def depth(expr):
    if not isinstance(expr, (list, tuple)):
        return 0
    max_depth = 0
    for i in expr:
        if depth(i) > max_depth:
            max_depth = depth(i)
    return max_depth+1

#it follow the path of tuple and count number of branch in tuple tree.In each node or section chose node that have max branches,so will be found max depth of expression.

test0 = 'x'
test1 = ('expt', 'x', 2)
test2 = ('+', ('expt', 'x', 2), ('expt', 'y', 2))
test3 = ('/', ('expt', 'x', 5), ('expt', ('-', ('expt', 'x', 2), 1), ('/', 5, 2)))

print(str(test0)+" => "+str(depth(test0)))
print(str(test1)+" => "+str(depth(test1)))
print(str(test2)+" => "+str(depth(test2)))
print(str(test3)+" => "+str(depth(test3)))
