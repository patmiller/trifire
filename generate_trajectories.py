# Copyright 2025 Patrick Miller
import sys

PENROSE_HEIGHT = 80
PENROSE_WIDTH = 88

class Coords:
    def __init__(self,x=65535,y=65535):
        self.x = x
        self.y = y
        return

    def __repr__(self):
        return f'{{{self.x}, {self.y}}}'

    def __hash__(self):
        return hash((self.x, self.y))

    def __eq__(self,other):
        return self.x == other.x and self.y == other.y

    def __lt__(self,other):
        if self.x < other.x: return True
        if self.x == other.x and self.y < other.y: return True
        return False

# Each array starts with (0,0) so that we can just advance the offset
# at the end of the play switch.  We move forward one position into
# the first "real" (x,y) for the cannon ball.
T = [ [] for i in range(18*3+1) ]
XSTEP = 32
YSTEP = XSTEP*1.73
ZERO = 300
deltas = [(-XSTEP,-YSTEP), (XSTEP,0), (-XSTEP,-YSTEP)]
xy = [(PENROSE_WIDTH//2-7,-PENROSE_HEIGHT),(PENROSE_WIDTH+8,0),(0,0)]
for i,body in enumerate(T):
    if i == 0: continue
    rotation = (i-1)//18
    step = (i-1)%18
    if step == 0: print()
    body.append(Coords(0,0))

    dx,dy = deltas[rotation]
    x0,y0 = xy[rotation]
    x0 += step*XSTEP
    y0 += ZERO
    if x0 < 640:
        body.append(Coords(x0,y0))
    while True:
        x0 += dx
        y0 = int(y0 + dy)
        if x0 < 0 or x0 >= 640:
            break
        if y0 < 0 or y0 >= 480:
            break
        if x0 == 640: xx
        body.append(Coords(x0,y0))

    body.append(Coords())

updates = ''
for i,body in enumerate(T):
    if i == 0: continue
    updates += f'Coords traj{i}[] = {{\n  '
    updates += ',\n  '.join([str(x) for x in body])
    updates += "};\n"

# The coins can only live where a cannon ball can get them
coinset = set()
for trajectory in T:
    for coord in trajectory:
        if coord.x == 0 and coord.y == 0: continue
        if coord.x == 65535: continue
        coinset.add(coord)
coins = sorted(coinset)

updates += f'const unsigned NCOINS = {len(coins)}u;\n'
updates += 'const Coords coins[] = {\n'
while coins:
    row = coins[:4]
    coins = coins[4:]
    updates += '  '+(', '.join(str(x) for x in row))
    updates += ',\n' if coins else '\n'
updates += '};\n'

CUT = '// -- %< ---\n'
trajectory_c = open('trajectory.c').read()
split = trajectory_c.split(CUT)
split[1] = updates

try:
    filename = sys.argv[1]
    open(filename,'w').write(CUT.join(split))
except IndexError:
    print(CUT.join(split),end='')


