# 算法练习记录

计算机科学与技术本科生的算法练习仓库。目标是持续记录刷题与课程作业的过程，把「学过」变成「能展示」。

目前收录 3 组题目，均为 C 语言实现：

| 目录 | 题目 | 涉及知识点 |
| --- | --- | --- |
| `problems/01-max-subsequence` | 最大子段和 | 暴力 / 枚举优化 / 分治 / 动态规划（Kadane）四种解法对比 |
| `problems/02-matrix-chain` | 矩阵连乘最优加括号 | 区间 DP、最优子结构 |
| `problems/03-tiling-2xn` | 2×n 铺砖 | 递推、矩阵快速幂，附 Python 验证脚本 |

## 编译与运行

需要 C 编译器（gcc / clang）。以最大子段和为例：

```bash
cd problems/01-max-subsequence
gcc -O2 -o maxsubseq maxsubseq.c
./maxsubseq
```

`maxsubseq.c` 顶部的 `METHOD` 宏可切换 1—4 四种解法，重新编译即可对比。
铺砖问题的验证脚本可直接运行：

```bash
python problems/03-tiling-2xn/verify.py
```

## 记录方式

每解决一道题，新增一个 `problems/<编号>-<题目名>/` 目录，至少包含：

- 可编译运行的源码
- 简要的思路说明（时间复杂度、关键递推式）
- 一组能复现的样例输入输出

## 进度

- [x] 最大子段和
- [x] 矩阵连乘
- [x] 2×n 铺砖
- [ ] 持续补充中
