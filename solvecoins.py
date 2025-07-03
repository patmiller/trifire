import re
import trifire

def solve(x,y):
    # Build a json with each x position and each rotation
    for i in range(18):
        for j in range(3):
            state = {
                'turn': 0,
                'points': 0,
                'tri_x': i*32,
                'rotation': j,
                "cannon_t": 0,
                "cannon_offset": 0,
                "coin_x": x,
                "coin_y": y}
            # Fire the cannon in this configuration and spin for a while to see if a
            # point is scored
            try:
                state = trifire.play(state,'f')
            except:
                state = None
            if state is not None:
                for k in range(20):
                    state = trifire.play(state,'s')
                    if state['points'] != 0:
                        #print('solved',(x,y),i,j,k,state)
                        print((x,y),i*'r'+'s'*j + 'f')
                        return
                
                

m = re.search(r'coins\[\]\s+=\s+\{(.*?)\};',open('trajectory.c').read(),re.DOTALL)
definition = m.group(1)
coins = [(int(m.group(1)),int(m.group(2))) for m in re.finditer(r'(\d+),\s+(\d+)',definition)]
for x,y in coins:
    solve(x,y)
         
