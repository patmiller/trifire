class Coords:
    def __init__(self,x=None,y=None):
        self.x = x
        self.y = y
        return

    def __repr__(self):
        return f'{{{self.x}, {self.y}}}'

T = [ [] for i in range(52) ]
for i in range(17):
    for j in range(10):
        coords = Coords()
        T[i+1].append(coords)

    if (32 * i - 32 * j + 25 < 0):
        coords.x = 65536
    else:
        coords.x = 32 * i - 32 * j + 25

    temp = int(315 - 32 * 1.73 * j)
    if( temp < 0 ):
        temp = 65536
    coords.y = temp

    # Out of bounds in some way
    if coords.x == 65536 or coords.y == 65536:
        coords.x = 65536
        coords.y = 65536
        break

        temp = int(235 + 32 * 1.73 * j)
        if( temp > 480 ):
            temp = 65536
        coords.y = temp

    if (32 * i - 32 * j + 30 < 0):
        coords.x = 65536
    else:
        coords.x = 32 * i - 32 * j + 25
    coords.y = 315

    # Out of bounds in some way
    if coords.x == 65536 or coords.y == 65536:
        coords.x = 65536
        coords.y = 65536
        break

for i in range(17):
    for j in range(10):
        coords = Coords()
        T[i+18].append(coords)

    if (32 * i - 32 * j - 15 < 0):
        coords.x = 65536
    else:
        coords.x = 32 * i - 32 * j - 15

    temp = 235 - 32 * 1.73 * j
    if (temp < 0):
        temp = 65536
    coords.y = int(temp)


        temp = int(235 + 32 * 1.73 * j)
        if( temp > 480 ):
            temp = 65536
        coords.y = temp

        # Out of bounds in some way
        if coords.x == 65536 or coords.y == 65536:
            coords.x = 65536
            coords.y = 65536
            break

for i in range(17):
    for j in range(10):
        coords = Coords()
        T[i+35].append(coords)

        if (32 * i - 32 * j - 15 < 0):
            coords.x = 65536
        else:
            coords.x = 32 * i - 32 * j - 15;

        temp = 235 + 32 * 1.73 * j;
        if (temp > 480):
            temp = 65536;
        coords.y = int(temp);

        # Out of bounds in some way
        if coords.x == 65536 or coords.y == 65536:
            coords.x = 65536
            coords.y = 65536
            break

for i,body in enumerate(T):
    if i == 0: continue
    print(f'Coords traj{i}[] = {{\n  ',end='')
    print(',\n  '.join([str(x) for x in body]))
    print("};\n")
