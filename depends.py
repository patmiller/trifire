# Copyright 2025 Patrick Miller
import re
import subprocess

result = subprocess.run(['make', 'sources'], capture_output=True, text=True)

for c_file in sorted(result.stdout.split()):
    o_file = c_file[:-1] + 'o'
    source = open(c_file).read()
    headers = sorted(
        [match.group(1)+'.h'
         for match in re.finditer(r'#include\s+"(\w+)',source)])
    print(f"{o_file}: {c_file} {' '.join(headers)}")
