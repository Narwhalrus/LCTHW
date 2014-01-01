import re
line = '''\
1 h words words words 
blah 2 h words words
'''

scanner = re.Scanner([
    (r"([0-9]+(?:\ h|h))", lambda scanner, token: ("HOURS", token)),
    (r'.', lambda scanner, token: None),
    ], flags = re.DOTALL)

results, remainder = scanner.scan(line)
print(results)
