try:
    from PIL import Image
except ModuleNotFoundError:
    print("You have to do a % pip3 install PIL")
    raise
import sys
import os

png = sys.argv[1]
_,root = os.path.split(png)
base,_ = os.path.splitext(root)
im = Image.open(png)

# OK, first we make a header file
with open(base+'.h','w') as out:
    print('#pragma once',file=out)
    print('#include <stdint.h>',file=out)
    print(file=out)
    print(f'#define {base.upper()}_WIDTH = ({im.size[0]})',file=out)
    print(f'#define {base.upper()}_HEIGHT = ({im.size[1]})',file=out)
    print(f'extern uint32_t {base}[{base.upper()}_WIDTH * {base.upper()}_HEIGHT];',file=out)

# OK, then take the pixels and pack them into that array in the .c file
im = im.convert("RGB")
pixels = im.load()
n = 0
with open(base+'.c','w') as out:
    print(f'#include "{base}.h"',file=out)
    print('#include <stdint.h>',file=out)
    print(file=out)
    print(f'uint32_t {base}[{base.upper()}_WIDTH * {base.upper()}_HEIGHT] = {{',file=out,end='')
    for j in range(im.size[1]):
        for i in range(im.size[0]):
            if n%8 == 0:
                print(file=out)
                print('  ',file=out,end='')
            n += 1
            px = pixels[i,j]
            r,g,b = px
            rgb = ( r << 16 ) | ( g << 8 ) | (b)
            print((f'{hex(rgb)},').rjust(9),end='',file=out)
    print("\n};",file=out)
