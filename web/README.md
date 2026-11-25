# AlgoLab · 算法可视化实验室

把课堂上的经典算法做成能交互、看得见过程的 Web 应用。这是 `cs-practice` 仓库的第二个 commit——从「会写」到「能用」。

## 为什么做这个

刚学完数据结构与算法时，最大子段和、矩阵连乘、铺砖这些题停留在「能 AC」。但面试官更愿意看到你能把一个算法讲清楚、能可视化它的执行过程。这个项目把仓库 `problems/` 下的三组 C 语言练习，用 Python 重新实现并包装成带图形界面的 Web 服务。

**零第三方依赖**是刻意的设计：后端只用 Python 标准库 `http.server`，前端是原生 HTML/CSS/JS。任何人只要有 Python 就能跑起来，不用 `pip install`，也不会因为环境问题跑不通。

## 快速开始

需要 Python 3.6+（无需安装任何包）。

```bash
cd web
python server.py
```

浏览器打开 <http://localhost:8000> 即可。就这一步。

跑自测：

```bash
python test_algorithms.py
```

## 功能与对应算法

| 标签页 | 算法 | 复杂度 | 对应练习 |
| --- | --- | --- | --- |
| 最大子段和 | Kadane 动态规划 | O(n) | `problems/01-max-subsequence` |
| 矩阵连乘 | 区间 DP | O(n³) | `problems/02-matrix-chain` |
| 2×n 铺砖 | 递推 / 斐波那契 | O(n) | `problems/03-tiling-2xn` |

每个标签页都可以自由输入数据（带示例按钮），运行后展示：
- 最大子段和：最优子段高亮 + Kadane 每一步的 `current` / `best` 表格
- 矩阵连乘：最优加括号方案 + 完整 DP 代价表
- 2×n 铺砖：方案数 + 递推序列（Python 大整数，n=2000 也不溢出）

## 目录结构

```
web/
├── server.py            # HTTP 服务：静态文件 + REST API
├── algorithms.py        # 三个算法的实现（纯函数，可单测）
├── test_algorithms.py   # 一键自测
├── README.md
└── static/
    ├── index.html       # 页面结构
    ├── style.css        # 样式
    └── app.js           # 交互与可视化渲染
```

## API 接口

| 方法 | 路径 | 请求体 | 说明 |
| --- | --- | --- | --- |
| GET | `/api/health` | — | 健康检查 |
| POST | `/api/max-subsequence` | `{"nums": [-2,1,-3,4]}` | 返回 maxSum / start / end / steps |
| POST | `/api/matrix-chain` | `{"dims": [30,35,15,5]}` | 返回 minCost / m / s / order |
| POST | `/api/tiling` | `{"n": 10}` | 返回 value / sequence |

非法输入返回 400 和 `{"error": "..."}`。

## 部署

标准 HTTP 服务，监听 `0.0.0.0:PORT`（默认 8000，可用环境变量 `PORT` 覆盖）。可直接部署到任何支持 Python 的平台（云服务器、Railway、Render、腾讯云等），无需构建步骤。

## 技术栈

- 后端：Python 3 标准库（`http.server`、`json`、`pathlib`）
- 前端：HTML5 + CSS3 + 原生 JavaScript（Fetch API）
- 无框架、无依赖、无构建工具
