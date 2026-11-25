"""一键自测三个算法：python web/test_algorithms.py"""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from algorithms import max_subsequence, matrix_chain, tiling_2xn  # noqa: E402


def test_max_subsequence():
    r = max_subsequence([-2, 1, -3, 4, -1, 2, 1, -5, 4])
    assert r["maxSum"] == 6, r
    assert r["subarray"] == [4, -1, 2, 1], r
    assert (r["start"], r["end"]) == (3, 6), r

    # 全负数： Kadane 应返回最大的单个元素
    r2 = max_subsequence([-1, -2, -3, -4])
    assert r2["maxSum"] == -1, r2

    # 空数组应报错
    try:
        max_subsequence([])
        assert False, "空数组应抛出 ValueError"
    except ValueError:
        pass
    print("[PASS] max_subsequence")


def test_matrix_chain():
    r = matrix_chain([30, 35, 15, 5, 10, 20, 25])
    assert r["minCost"] == 15125, r
    assert r["order"] == "((A1 x (A2 x A3)) x ((A4 x A5) x A6))", r

    r2 = matrix_chain([10, 20, 30])
    assert r2["minCost"] == 6000, r2
    print("[PASS] matrix_chain")


def test_tiling():
    assert tiling_2xn(0)["value"] == 1
    assert tiling_2xn(1)["value"] == 1
    assert tiling_2xn(10)["value"] == 89
    assert tiling_2xn(30)["value"] == 1346269
    # f(n) = f(n-1) + f(n-2)
    seq = tiling_2xn(20)["sequence"]
    assert all(seq[i] == seq[i - 1] + seq[i - 2] for i in range(2, len(seq)))
    print("[PASS] tiling_2xn")


if __name__ == "__main__":
    test_max_subsequence()
    test_matrix_chain()
    test_tiling()
    print("\nAll tests passed.")
