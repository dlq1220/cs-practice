"""零依赖 HTTP 服务：静态文件 + 三个算法 API。

只用 Python 标准库，pip install 都不需要。启动:
    python web/server.py
然后浏览器访问 http://localhost:8000
"""

import json
import os
import sys
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import urlparse

# 保证从任意目录启动都能 import 到同目录的 algorithms
sys.path.insert(0, str(Path(__file__).resolve().parent))
from algorithms import max_subsequence, matrix_chain, tiling_2xn  # noqa: E402

BASE_DIR = Path(__file__).resolve().parent
STATIC_DIR = BASE_DIR / "static"
PORT = int(os.environ.get("PORT", "8000"))

CONTENT_TYPES = {
    ".html": "text/html; charset=utf-8",
    ".css": "text/css; charset=utf-8",
    ".js": "application/javascript; charset=utf-8",
    ".json": "application/json; charset=utf-8",
    ".svg": "image/svg+xml",
    ".ico": "image/x-icon",
}


class Handler(BaseHTTPRequestHandler):
    def _send(self, status, body, content_type):
        if isinstance(body, str):
            body = body.encode("utf-8")
        self.send_response(status)
        self.send_header("Content-Type", content_type)
        self.send_header("Content-Length", str(len(body)))
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.end_headers()
        self.wfile.write(body)

    def _json(self, status, payload):
        self._send(
            status, json.dumps(payload, ensure_ascii=False), "application/json; charset=utf-8"
        )

    def _static(self, path):
        # 去掉查询串，解析相对路径，并阻止目录穿越
        rel = urlparse(path).path.lstrip("/")
        if rel in ("", "index.html"):
            target = STATIC_DIR / "index.html"
        else:
            target = (STATIC_DIR / rel).resolve()
            if STATIC_DIR.resolve() not in target.parents and target != STATIC_DIR.resolve():
                self._json(403, {"error": "Forbidden"})
                return
        if not target.is_file():
            self._json(404, {"error": "Not Found"})
            return
        ctype = CONTENT_TYPES.get(target.suffix.lower(), "application/octet-stream")
        self._send(200, target.read_bytes(), ctype)

    def do_OPTIONS(self):
        self._send(204, b"", "text/plain")

    def do_GET(self):
        path = urlparse(self.path).path
        if path == "/api/health":
            self._json(200, {"status": "ok", "service": "algo-lab"})
        elif path.startswith("/api/"):
            self._json(404, {"error": "Unknown API"})
        else:
            self._static(path)

    def do_POST(self):
        path = urlparse(self.path).path
        length = int(self.headers.get("Content-Length", "0"))
        try:
            payload = json.loads(self.rfile.read(length) or b"{}")
        except json.JSONDecodeError:
            self._json(400, {"error": "Invalid JSON"})
            return

        try:
            if path == "/api/max-subsequence":
                result = max_subsequence(payload.get("nums", []))
            elif path == "/api/matrix-chain":
                result = matrix_chain(payload.get("dims", []))
            elif path == "/api/tiling":
                result = tiling_2xn(payload.get("n", 0))
            else:
                self._json(404, {"error": "Unknown API"})
                return
            self._json(200, result)
        except (ValueError, TypeError) as exc:
            self._json(400, {"error": str(exc)})

    def log_message(self, fmt, *args):
        # 精简日志，避免刷屏
        sys.stderr.write("%s - %s\n" % (self.address_string(), fmt % args))


def main():
    server = ThreadingHTTPServer(("0.0.0.0", PORT), Handler)
    print("AlgoLab running at http://localhost:%d" % PORT, flush=True)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\nstopped", flush=True)
        server.server_close()


if __name__ == "__main__":
    main()
