# Copyright 2025 Patrick Miller
import sys

PENROSE_HEIGHT = 80
PENROSE_WIDTH = 88

class Coords:
    def __init__(self,x=None,y=None):
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
T = [ [] for i in range(52) ]
for i in range(17):
    T[i+1].append(Coords(0,0))
    for j in range(10):
        coords = Coords()
        T[i+1].append(coords)

        X = 32 * i - 32 * j + 25
        coords.x = X

        Y = int(300 - PENROSE_HEIGHT - 32 * 1.73 * j)
        coords.y = Y

        # Out of bounds in some way
        if coords.x < 0 or coords.x >= 640 or coords.y < 0 or coords.y >= 480:
            coords.x = 65535
            coords.y = 65535
            break

for i in range(17):
    T[i+17].append(Coords(0,0))
    for j in range(10):
        coords = Coords()
        T[i+18].append(coords)

        X = i*32 + j*32 + PENROSE_WIDTH
        Y = 300
        coords.x = X
        coords.y = Y

        # Out of bounds in some way
        if coords.x < 0 or coords.x >= 640 or coords.y < 0 or coords.y >= 480:
            coords.x = 65535
            coords.y = 65535
            break

for i in range(17):
    T[i+35].append(Coords(0,0))
    for j in range(10):
        coords = Coords()
        T[i+35].append(coords)

        X = i*32
        Y = int(315 + 32*1.73*j)

        coords.x = X
        coords.y = Y

        # Out of bounds in some way
        if coords.x < 0 or coords.x >= 640 or coords.y < 0 or coords.y >= 480:
            coords.x = 65535
            coords.y = 65535
            break

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


