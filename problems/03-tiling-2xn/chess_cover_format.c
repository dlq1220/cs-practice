/*
 * 棋盘覆盖 —— PE 排查版（数字已确认全部正确，只差空白字符）
 *
 * 三个开关，按下面顺序试，每次只改一处后重新提交：
 *
 *   第 1 步：SPACE=0 CRLF=0 BLANK=0   标准格式（当前默认）
 *   第 2 步：SPACE=1 CRLF=0 BLANK=0   每个数字后都有空格（老师用 printf("%d ") 生成的答案）
 *   第 3 步：SPACE=0 CRLF=0 BLANK=1   多组数据之间空一行（题面"注意格式"常指这个）
 *   第 4 步：SPACE=1 CRLF=0 BLANK=1
 *   第 5 步：CRLF=1 任意组合          答案文件是 Windows 换行符 \r\n（校内 Windows 服务器常见）
 *
 * 全部失败的话再把 EOL_MODE 改成 0（文件末尾不带换行）。
 */
#include <stdio.h>
#include <string.h>

#define SPACE_MODE 0    /* 1 = 每个数字后都输出空格（含行尾），0 = 仅行内分隔 */
#define CRLF_MODE  0    /* 1 = 换行用 \r\n，0 = 用 \n */
#define BLANK_MODE 0    /* 1 = 多组数据之间空一行，0 = 不空 */
#define EOL_MODE   1    /* 1 = 每行后换行，0 = 行前换行（文件末尾无换行） */

#define MAXN 64

int board[MAXN][MAXN];
int tile;

void chessBoard(int tr, int tc, int dr, int dc, int size)
{
    int s, t;

    if (size == 1) return;

    s = size / 2;
    t = ++tile;

    if (dr < tr + s && dc < tc + s) {          /* 左上 */
        chessBoard(tr, tc, dr, dc, s);
    } else {
        board[tr + s - 1][tc + s - 1] = t;
        chessBoard(tr, tc, tr + s - 1, tc + s - 1, s);
    }

    if (dr < tr + s && dc >= tc + s) {         /* 右上 */
        chessBoard(tr, tc + s, dr, dc, s);
    } else {
        board[tr + s - 1][tc + s] = t;
        chessBoard(tr, tc + s, tr + s - 1, tc + s, s);
    }

    if (dr >= tr + s && dc < tc + s) {         /* 左下 */
        chessBoard(tr + s, tc, dr, dc, s);
    } else {
        board[tr + s][tc + s - 1] = t;
        chessBoard(tr + s, tc, tr + s, tc + s - 1, s);
    }

    if (dr >= tr + s && dc >= tc + s) {        /* 右下 */
        chessBoard(tr + s, tc + s, dr, dc, s);
    } else {
        board[tr + s][tc + s] = t;
        chessBoard(tr + s, tc + s, tr + s, tc + s, s);
    }
}

void newline(void)
{
#if CRLF_MODE == 1
    putchar('\r');
#endif
    putchar('\n');
}

int main(void)
{
    int k, dr, dc, n, i, j, first = 1;

    while (scanf("%d %d %d", &k, &dr, &dc) == 3) {
        if (k < 1 || k > 6) continue;
        n = 1 << k;
        if (dr < 0 || dr >= n || dc < 0 || dc >= n) continue;

#if BLANK_MODE == 1
        if (!first) newline();                 /* 组与组之间空一行 */
#endif
        first = 0;

        memset(board, 0, sizeof(board));
        tile = 0;
        board[dr][dc] = 0;
        chessBoard(0, 0, dr, dc, n);

        for (i = 0; i < n; i++) {
#if EOL_MODE == 0
            if (i > 0) newline();              /* 行前换行，文件末尾无换行 */
#endif
            for (j = 0; j < n; j++) {
                printf("%d", board[i][j]);
#if SPACE_MODE == 1
                putchar(' ');
#else
                if (j < n - 1) putchar(' ');
#endif
            }
#if EOL_MODE == 1
            newline();
#endif
        }
    }
    return 0;
}
