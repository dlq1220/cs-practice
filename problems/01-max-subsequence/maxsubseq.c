/* 最大子段和 —— 算法分析与设计作业
 * 输入：第一行 n（n<1000），随后 n 个整数；多组数据直到 EOF
 * 输出：连续子段和的最大值；若全为负则输出 0
 *
 * 切换解法：修改下面 METHOD 的值（1/2/3/4），重新编译即可。
 */
#include <stdio.h>

#define MAXN 1005
#define METHOD 4   /* 1=暴力O(n^3)  2=枚举O(n^2)  3=分治O(nlogn)  4=动态规划O(n) */

int a[MAXN];       /* 用 1..n 下标，与题面 a1..an 一致 */

/* 解法1：暴力枚举。枚举起点 i、终点 j，再逐项累加求和 O(n^3) */
int maxSubCube(int n)
{
    int best = 0;                    /* 初值为 0：全负时自然输出 0 */
    for (int i = 1; i <= n; i++) {
        for (int j = i; j <= n; j++) {
            int sum = 0;
            for (int k = i; k <= j; k++)
                sum += a[k];
            if (sum > best) best = sum;
        }
    }
    return best;
}

/* 解法2：枚举优化。固定起点 i，右端点 j 右移时累加，去掉最内层循环 O(n^2) */
int maxSubSquare(int n)
{
    int best = 0;
    for (int i = 1; i <= n; i++) {
        int sum = 0;
        for (int j = i; j <= n; j++) {
            sum += a[j];             /* sum 即 a[i..j] 的和 */
            if (sum > best) best = sum;
        }
    }
    return best;
}

/* 解法3：分治。最大子段只可能落在「左半」「右半」「跨越中点」三处之一 */
int maxSubDivide(int left, int right)
{
    if (left == right)               /* 递归出口：单个元素，负数按 0 处理 */
        return a[left] > 0 ? a[left] : 0;

    int mid = (left + right) / 2;
    int leftBest  = maxSubDivide(left, mid);
    int rightBest = maxSubDivide(mid + 1, right);

    /* 跨中点最大和 = 中点左侧最大后缀和 s1 + 中点右侧最大前缀和 s2 */
    int s1 = 0, tmp = 0;
    for (int i = mid; i >= left; i--) {
        tmp += a[i];
        if (tmp > s1) s1 = tmp;
    }
    int s2 = 0;
    tmp = 0;
    for (int i = mid + 1; i <= right; i++) {
        tmp += a[i];
        if (tmp > s2) s2 = tmp;
    }

    int best = leftBest > rightBest ? leftBest : rightBest;
    if (s1 + s2 > best) best = s1 + s2;
    return best;
}

/* 解法4：动态规划(Kadane)。b = 以当前位置结尾的最大子段和 */
int maxSubDP(int n)
{
    int best = 0, b = 0;
    for (int i = 1; i <= n; i++) {
        if (b > 0) b += a[i];        /* 前缀为正才接上，否则从 a[i] 重新开始 */
        else       b = a[i];
        if (b > best) best = b;      /* best 记录全局最优 */
    }
    return best;
}

int main(void)
{
    int n;
    while (scanf("%d", &n) == 1) {   /* 读到 EOF 结束，支持多组数据 */
        if (n <= 0) continue;        /* n 为正整数；防御性跳过异常输入 */
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);

        int ans;
        #if   METHOD == 1
            ans = maxSubCube(n);
        #elif METHOD == 2
            ans = maxSubSquare(n);
        #elif METHOD == 3
            ans = maxSubDivide(1, n);
        #else
            ans = maxSubDP(n);
        #endif
        printf("%d\n", ans);
    }
    return 0;
}
