import sys
sys.setrecursionlimit(10000)

def rec(n):
    if n == 1: return 1
    if n == 2: return 2
    return rec(n - 1) + rec(n - 2)

def lin(n, a=1, b=2):
    if n == 1: return a
    if n == 2: return b
    return lin(n - 1, b, a + b)

def mul(a, b):
    return [[sum(a[i][k] * b[k][j] for k in range(2)) for j in range(2)] for i in range(2)]

def mp(n):
    res = [[1, 0], [0, 1]]
    base = [[1, 1], [1, 0]]
    while n > 0:
        if n & 1: res = mul(res, base)
        base = mul(base, base)
        n >>= 1
    return res[0][0]

print("sample (1,3,2):", rec(1), rec(3), rec(2), "|", lin(1), lin(3), lin(2), "|", mp(1), mp(3), mp(2))
bad = [n for n in range(1, 31) if not (lin(n) == mp(n) == rec(n))]
print("n=1..30 all three agree (naive recursion only feasible to ~30):", not bad, bad)
bad2 = [n for n in range(1, 51) if lin(n) != mp(n)]
print("n=1..50 linear recursion == matrix power:", not bad2)
print("n=50 ->", mp(50), "| fits in long long:", mp(50) < 2**63 - 1)
print("f(1..10) =", [mp(n) for n in range(1, 11)])
