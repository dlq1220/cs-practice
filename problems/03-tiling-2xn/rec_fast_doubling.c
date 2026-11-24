/*
 * 2×n 骨牌铺法 —— 附加版：快速倍增递归（纯递归、不用数组，O(log n)）
 *
 * 比矩阵快速幂更短，同样是"分治 + 递归"，T(n) = T(n/2) + O(1)。
 * 恒等式（令 F(0)=0, F(1)=1）：
 *      F(2k)   = F(k) * ( 2*F(k+1) - F(k) )
 *      F(2k+1) = F(k)^2 + F(k+1)^2
 * 求 f(n) 即求 F(n+1)，n=50 时递归深度只有 6 层。
 */
#include <stdio.h>

typedef long long ll;

typedef struct { ll fk, fk1; } Pair;   /* fk = F(k), fk1 = F(k+1) */

Pair fib(int n)
{
    Pair r, h;
    if (n == 0) {                      /* F(0)=0, F(1)=1 */
        r.fk = 0; r.fk1 = 1;
        return r;
    }
    h = fib(n / 2);                    /* 只递归一次，规模减半 */
    r.fk  = h.fk * (2 * h.fk1 - h.fk); /* F(2m)   */
    r.fk1 = h.fk * h.fk + h.fk1 * h.fk1; /* F(2m+1) */
    if (n % 2 == 1) {                  /* n 为奇数：再往前走一项 */
        ll even = r.fk;                /* F(2m)，先存下来 */
        ll odd  = r.fk1;               /* F(2m+1) */
        r.fk  = odd;
        r.fk1 = even + odd;            /* F(2m+2) */
    }
    return r;
}

int main(void)
{
    int n;
    while (scanf("%d", &n) == 1) {
        printf("%lld\n", fib(n + 1).fk);   /* f(n) = F(n+1) */
    }
    return 0;
}
