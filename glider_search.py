# glider_search.py
# Python 3 script — brute force or random search for gliders
import itertools
import copy
from collections import defaultdict

# --- Configurable parameters ---
W, H = 5, 4              # bounding box to search (small values recommended)
Tmax = 6                 # maximum period to test
R = 3                    # search translations in range [-R..R] (excluding 0,0)
mode = "exhaustive"      # "exhaustive" or "random"
random_samples = 200000  # used only if mode == "random"
# -------------------------------

# Map a cell (x,y) in infinite grid to boolean using a finite pattern offset origin at (0,0)
def step(bitmap):
    # bitmap: set of (x,y) live coords
    new = set()
    # Determine candidate cells to check: all cells within +/-2 of any live cell
    candidates = set()
    for (x,y) in bitmap:
        for dx in range(-2,3):
            for dy in range(-2,3):
                candidates.add((x+dx, y+dy))

    def getcell(x,y):
        return (x,y) in bitmap

    for (cx,cy) in candidates:
        # compute leaves
        def leaf_count(cells):
            return sum(1 for (ax,ay) in cells if getcell(ax,ay))
        # top leaf: (cx, cy-1), (cx, cy-2)
        top = leaf_count([(cx, cy-1), (cx, cy-2)])
        right = leaf_count([(cx+1, cy), (cx+2, cy)])
        bottom = leaf_count([(cx, cy+1), (cx, cy+2)])
        left = leaf_count([(cx-1, cy), (cx-2, cy)])
        semi = sum(1 for v in (top,right,bottom,left) if v == 1)
        full = sum(1 for v in (top,right,bottom,left) if v == 2)

        alive = getcell(cx,cy)
        if alive:
            if full > 0:
                pass  # dies
            elif semi == 0:
                pass  # dies
            else:
                new.add((cx,cy))
        else:
            if semi == 2:
                new.add((cx,cy))
    return new

def normalize(pattern):
    if not pattern:
        return frozenset()
    xs = [p[0] for p in pattern]; ys = [p[1] for p in pattern]
    minx, miny = min(xs), min(ys)
    return frozenset(((x-minx, y-miny) for (x,y) in pattern))

def pattern_to_bitmap(bits, w, h):
    s = set()
    i = 0
    for y in range(h):
        for x in range(w):
            if bits & (1<<i):
                s.add((x,y))
            i += 1
    return s

def bitmap_to_str(bm):
    if not bm: return "<empty>"
    norm = normalize(bm)
    xs = [p[0] for p in norm]; ys = [p[1] for p in norm]
    W = max(xs)+1; H = max(ys)+1
    out = []
    for y in range(H):
        row = []
        for x in range(W):
            row.append('#' if (x,y) in norm else '.')
        out.append(''.join(row))
    return "\n".join(out)

# check if pattern P after T steps equals P translated by (dx,dy)
def is_translating_glider(P, T, dx, dy):
    cur = set(P)
    for _ in range(T):
        cur = step(cur)
    # translate original by (dx,dy)
    translated = set((x+dx, y+dy) for (x,y) in P)
    return cur == translated

# find the minimal bounding box for translation checks
def search():
    found = []
    total = 0
    if mode == "exhaustive":
        bits = W*H
        cap = 1<<bits
        print(f"Exhaustive search {W}x{H} => {bits} bits, {cap} patterns")
        rng = range(cap)
    else:
        import random
        rng = (random.randrange(1<<(W*H)) for _ in range(random_samples))
        cap = random_samples
        print(f"Random sampling {cap} patterns in {W}x{H}")

    for idx, b in enumerate(rng):
        total += 1
        if idx and idx % 1000000 == 0:
            print("checked", idx)
        P = pattern_to_bitmap(b, W, H)
        if not P:
            continue
        # optional: skip disconnected patterns (heuristic)
        # quick connectivity check:
        # (skip if you want full result)
        stack = [next(iter(P))]
        vis = {stack[0]}
        while stack:
            u = stack.pop()
            for dxn, dyn in [(1,0),(-1,0),(0,1),(0,-1),(1,1),(1,-1),(-1,1),(-1,-1)]:
                v = (u[0]+dxn, u[1]+dyn)
                if v in P and v not in vis:
                    vis.add(v); stack.append(v)
        if len(vis) != len(P):
            continue

        # test T and translations
        for T in range(1, Tmax+1):
            # try translations dx,dy in [-R..R] excluding (0,0)
            for dx in range(-R, R+1):
                for dy in range(-R, R+1):
                    if dx == 0 and dy == 0: 
                        continue
                    if is_translating_glider(P, T, dx, dy):
                        found.append((P, T, dx, dy))
                        # optionally print the first found and continue searching
                        print("Found glider! T=", T, "dx=",dx,"dy=",dy)
                        print(bitmap_to_str(P))
                        return found
    print("Done. checked", total, "patterns; found", len(found))
    return found

if __name__ == "__main__":
    res = search()
    if res:
        for (P,T,dx,dy) in res:
            print("=== GLIDER ===")
            print("Period:", T, "Vector:", (dx,dy))
            print(bitmap_to_str(P))
