/*
 * 2×n 骨牌铺法 —— 矩阵快速幂（改进版）
 *
 * 相比原版改了 5 处，逐条对应分析里提的问题：
 *   1. 指数类型统一成 int：n 用 int 就够（n>91 结果必然溢出 long long），
 *      原版 n 是 long long 而 matrix_pow 的指数是 int，会隐式截断
 *   2. n<=0 保护：原版 n=0 会算出 3（错），这里返回 1
 *   3. 用 struct 返回值代替"出参 + 手抄 4 行"，matrix_mul/matrix_pow 都短了一半
 *      （结构体按值传递本身就是副本，别名问题从类型层面消失）
 *   4. 参数加 const，明确"只读"契约
 *   5. 补上溢出边界说明：n<=91 才在 long long 范围内
 *
 * 递推：f(1)=1, f(2)=2, f(n)=f(n-1)+f(n-2)
 * [f(n); f(n-1)] = M^(n-2) · [f(2); f(1)] = M^(n-2) · [2; 1]
 */
#include <stdio.h>

#define MAX_SAFE_N 91          /* f(92) = 12200160415121876738 > 2^63-1，会溢出 */

typedef struct { long long m[2][2]; } Mat;

static Mat mul(Mat x, Mat y)
{
    Mat z;
    z.m[0][0] = x.m[0][0] * y.m[0][0] + x.m[0][1] * y.m[1][0];
    z.m[0][1] = x.m[0][0] * y.m[0][1] + x.m[0][1] * y.m[1][1];
    z.m[1][0] = x.m[1][0] * y.m[0][0] + x.m[1][1] * y.m[1][0];
    z.m[1][1] = x.m[1][0] * y.m[0][1] + x.m[1][1] * y.m[1][1];
    return z;                  /* x、y 是值拷贝，z 是全新对象，无别名风险 */
}

/* 递归分治快速幂：M^e = (M^(e/2))² · (e 为奇数时再乘一个 M) */
static Mat mpow(Mat base, int e)
{
    Mat half, t;
    if (e == 0) {
        Mat id = {{{1, 0}, {0, 1}}};
        return id;
    }
    half = mpow(base, e / 2);
    t = mul(half, half);
    if (e & 1) return mul(t, base);
    return t;
}

int main(void)
{
    int n;
    Mat M = {{{1, 1}, {1, 0}}};

    while (scanf("%d", &n) == 1) {
        Mat p;
        if (n <= 0) { printf("1\n"); continue; }   /* 空方格算 1 种，防 e 为负 */
        if (n <= 2) { printf("%d\n", n); continue; }
        if (n > MAX_SAFE_N) {
            printf("overflow\n");                  /* 题目 n<=50 走不到这里 */
            continue;
        }
        p = mpow(M, n - 2);
        printf("%lld\n", p.m[0][0] * 2 + p.m[0][1]);
    }
    return 0;
}
