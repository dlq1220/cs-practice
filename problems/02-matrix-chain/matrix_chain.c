#include <stdio.h>
#include <limits.h>

#define MAXN 10
#define INF (LLONG_MAX / 4)

long long m[MAXN + 1][MAXN + 1];  /* m[i][j]: Ai..Aj 的最少乘法次数 */
int s[MAXN + 1][MAXN + 1];        /* s[i][j]: Ai..Aj 最优划分位置 k */

void print_order(int i, int j)
{
    if (i == j) {
        printf("A%d", i);
        return;
    }

    printf("(");
    print_order(i, s[i][j]);
    printf(" x ");
    print_order(s[i][j] + 1, j);
    printf(")");
}

int main(void)
{
    int n;
    int cas = 1;
    long long p[MAXN + 1];  /* 矩阵维度链：Ai 为 p[i-1] x p[i] */

    while (scanf("%d", &n) == 1 && n != 0) {
        int i, L, j, k;

        for (i = 1; i <= n; ++i) {
            int row, col;
            scanf("%d %d", &row, &col);
            if (i == 1) {
                p[0] = row;
            }
            p[i] = col;
        }

        for (i = 1; i <= n; ++i) {
            m[i][i] = 0;
        }

        /* 按链长递增，自底向上填表 */
        for (L = 2; L <= n; ++L) {
            for (i = 1; i <= n - L + 1; ++i) {
                j = i + L - 1;
                m[i][j] = INF;
                for (k = i; k < j; ++k) {
                    long long q = m[i][k] + m[k + 1][j]
                                  + p[i - 1] * p[k] * p[j];
                    if (q < m[i][j]) {
                        m[i][j] = q;
                        s[i][j] = k;
                    }
                }
            }
        }

        printf("Case %d: ", cas++);
        print_order(1, n);
        printf("\n");
    }

    return 0;
}
