"""纯 Python 算法实现，与 HTTP 层解耦，便于单测和复用。

三个算法对应仓库 problems/ 目录下的 C 语言练习：
  - max_subsequence  -> problems/01-max-subsequence
  - matrix_chain     -> problems/02-matrix-chain
  - tiling_2xn       -> problems/03-tiling-2xn
"""


def max_subsequence(nums):
    """Kadane 算法求最大子段和。

    返回:
        dict: maxSum / start / end / steps
              steps 中每一步记录当前累计值，供前端可视化。
    """
    if not nums:
        raise ValueError("数组不能为空")
    if not all(isinstance(x, int) for x in nums):
        raise ValueError("数组元素必须都是整数")

    # current: 以当前位置结尾的最大子段和
    # best: 全局最大子段和；用 >= 保留最后出现的更优解，这里用 > 保留首次
    current = best = nums[0]
    best_start = best_end = 0
    current_start = 0
    steps = [
        {"index": 0, "value": nums[0], "current": current, "best": best}
    ]

    for i in range(1, len(nums)):
        x = nums[i]
        if current < 0:
            # 前面的累计拖后腿，从当前位置重新开始
            current = x
            current_start = i
        else:
            current += x

        if current > best:
            best = current
            best_start = current_start
            best_end = i

        steps.append(
            {"index": i, "value": x, "current": current, "best": best}
        )

    return {
        "maxSum": best,
        "start": best_start,
        "end": best_end,
        "subarray": nums[best_start:best_end + 1],
        "steps": steps,
    }


def matrix_chain(dims):
    """矩阵连乘最优加括号（区间 DP）。

    dims: 长度为 n+1 的序列，第 i 个矩阵维度为 dims[i] x dims[i+1]
    返回: minCost / m (代价表) / s (分割点表) / order (加括号表达式)
    """
    if len(dims) < 2:
        raise ValueError("至少需要 2 个维度值（即 1 个矩阵）")
    if not all(isinstance(x, int) and x > 0 for x in dims):
        raise ValueError("维度必须是正整数")

    n = len(dims) - 1  # 矩阵个数
    m = [[0] * n for _ in range(n)]
    s = [[0] * n for _ in range(n)]

    # length 为链的长度（矩阵个数）
    for length in range(2, n + 1):
        for i in range(0, n - length + 1):
            j = i + length - 1
            m[i][j] = float("inf")
            for k in range(i, j):
                cost = (
                    m[i][k]
                    + m[k + 1][j]
                    + dims[i] * dims[k + 1] * dims[j + 1]
                )
                if cost < m[i][j]:
                    m[i][j] = cost
                    s[i][j] = k

    def build(i, j):
        if i == j:
            return "A%d" % (i + 1)
        k = s[i][j]
        return "(%s x %s)" % (build(i, k), build(k + 1, j))

    order = build(0, n - 1) if n >= 1 else ""

    # float('inf') 不能 JSON 序列化，单矩阵场景表里都是 0，无需转换
    return {
        "minCost": int(m[0][n - 1]),
        "m": m,
        "s": s,
        "order": order,
        "dims": dims,
    }


def tiling_2xn(n):
    """2xn 铺砖方案数：f(n) = f(n-1) + f(n-2), f(0)=1, f(1)=1。

    返回: value / sequence (f(0)..f(n))
    """
    if not isinstance(n, int):
        raise ValueError("n 必须是整数")
    if n < 0:
        raise ValueError("n 不能为负数")
    if n > 2000:
        raise ValueError("n 过大，请控制在 2000 以内")

    if n == 0:
        return {"value": 1, "sequence": [1]}

    seq = [1, 1]
    for i in range(2, n + 1):
        seq.append(seq[i - 1] + seq[i - 2])

    return {"value": seq[n], "sequence": seq}
