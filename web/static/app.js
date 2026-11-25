// AlgoLab 前端交互：标签切换 + 调用三个 API + 渲染可视化结果

function parseList(str) {
  const parts = str.split(/[\s,，]+/).filter(x => x !== '');
  const nums = parts.map(Number);
  if (nums.some(x => !Number.isFinite(x))) {
    throw new Error('请输入数字，用逗号或空格分隔');
  }
  return nums;
}

async function post(url, body) {
  const resp = await fetch(url, {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(body),
  });
  const data = await resp.json();
  if (!resp.ok) throw new Error(data.error || '请求失败');
  return data;
}

function esc(s) {
  return String(s).replace(/[&<>]/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;' }[c]));
}

function showError(id, msg) {
  document.getElementById(id).innerHTML = '<div class="error">' + esc(msg) + '</div>';
}

// ---------- 最大子段和 ----------
function setSubseq(v) { document.getElementById('subseq-input').value = v; runSubseq(); }

async function runSubseq() {
  const box = document.getElementById('subseq-result');
  box.innerHTML = '<div class="placeholder">计算中…</div>';
  let nums;
  try {
    nums = parseList(document.getElementById('subseq-input').value);
  } catch (e) { return showError('subseq-result', e.message); }

  try {
    const r = await post('/api/max-subsequence', { nums });
    let html = '<div class="stat-row">'
      + '<div class="stat"><div class="label">最大子段和</div><div class="value">' + r.maxSum + '</div></div>'
      + '<div class="stat"><div class="label">子段位置</div><div class="value">' + r.start + ' ~ ' + r.end + '</div></div>'
      + '<div class="stat"><div class="label">子段长度</div><div class="value">' + (r.end - r.start + 1) + '</div></div>'
      + '</div>';

    html += '<div class="block-title">原数组（绿色为最优子段）</div><div class="cells">';
    r.steps.forEach((s, i) => {
      const inSub = i >= r.start && i <= r.end;
      html += '<div class="cell' + (inSub ? ' in-sub' : '') + '"><span class="idx">' + i + '</span>' + s.value + '</div>';
    });
    html += '</div>';

    html += '<div class="block-title">Kadane 每一步（current：以当前位置结尾的最大和）</div>';
    html += '<div class="table-wrap"><table><thead><tr><th>i</th><th>a[i]</th><th>current</th><th>best</th></tr></thead><tbody>';
    r.steps.forEach(s => {
      html += '<tr><td>' + s.index + '</td><td>' + s.value + '</td><td>' + s.current + '</td>'
        + '<td class="' + (s.current === r.maxSum ? 'best' : '') + '">' + s.best + '</td></tr>';
    });
    html += '</tbody></table></div>';
    box.innerHTML = html;
  } catch (e) { showError('subseq-result', e.message); }
}

// ---------- 矩阵连乘 ----------
function setMatrix(v) { document.getElementById('matrix-input').value = v; runMatrix(); }

async function runMatrix() {
  const box = document.getElementById('matrix-result');
  box.innerHTML = '<div class="placeholder">计算中…</div>';
  let dims;
  try {
    dims = parseList(document.getElementById('matrix-input').value);
  } catch (e) { return showError('matrix-result', e.message); }

  try {
    const r = await post('/api/matrix-chain', { dims });
    let html = '<div class="stat-row">'
      + '<div class="stat"><div class="label">最少乘法次数</div><div class="value">' + r.minCost.toLocaleString() + '</div></div>'
      + '<div class="stat"><div class="label">矩阵个数</div><div class="value">' + dims.length - 1 + '</div></div>'
      + '</div>';

    html += '<div class="block-title">最优加括号方案</div><div class="expr">' + esc(r.order) + '</div>';

    const n = r.m.length;
    html += '<div class="block-title">DP 代价表 m[i][j]（行 i → 列 j，从 0 开始）</div>';
    html += '<div class="table-wrap"><table><thead><tr><th>i \\ j</th>';
    for (let j = 0; j < n; j++) html += '<th>' + j + '</th>';
    html += '</tr></thead><tbody>';
    for (let i = 0; i < n; i++) {
      html += '<tr><th>' + i + '</th>';
      for (let j = 0; j < n; j++) {
        const v = r.m[i][j];
        let cls = '';
        if (j < i) cls = 'diag';
        else if (v === r.minCost) cls = 'best';
        html += '<td class="' + cls + '">' + (j < i ? '—' : v.toLocaleString()) + '</td>';
      }
      html += '</tr>';
    }
    html += '</tbody></table></div>';
    html += '<p class="hint-note">表中每个格子表示第 i..j 个矩阵连乘的最小代价；最终答案在右上角 m[0][' + (n - 1) + ']。</p>';
    box.innerHTML = html;
  } catch (e) { showError('matrix-result', e.message); }
}

// ---------- 2×n 铺砖 ----------
function setTiling(v) { document.getElementById('tiling-input').value = v; runTiling(); }

async function runTiling() {
  const box = document.getElementById('tiling-result');
  box.innerHTML = '<div class="placeholder">计算中…</div>';
  const raw = document.getElementById('tiling-input').value;
  const n = Number(raw);
  if (!Number.isInteger(n) || n < 0 || n > 2000) {
    return showError('tiling-result', 'n 必须是 0 ~ 2000 的整数');
  }

  try {
    const r = await post('/api/tiling', { n });
    let html = '<div class="stat-row">'
      + '<div class="stat"><div class="label">f(' + n + ') 方案数</div><div class="value">' + r.value + '</div></div>'
      + '<div class="stat"><div class="label">递推式</div><div class="value" style="font-size:16px">f(n)=f(n-1)+f(n-2)</div></div>'
      + '</div>';

    const seq = r.sequence;
    const shown = seq.slice(0, Math.min(seq.length, 21));
    html += '<div class="block-title">前 ' + shown.length + ' 项（最后一个为 f(' + n + ')）</div><div class="seq">';
    shown.forEach((v, i) => {
      const isLast = i === n;
      html += '<div class="cell' + (isLast ? ' last' : '') + '"><span class="idx">f(' + i + ')</span>' + v + '</div>';
    });
    html += '</div>';
    if (seq.length > 21) {
      html += '<p class="hint-note">n 较大，仅展示前 21 项；完整答案见上方 f(' + n + ')。Python 大整数精确计算，不会溢出。</p>';
    } else {
      html += '<p class="hint-note">边界：f(0)=1，f(1)=1。后续每项都是前两项之和。</p>';
    }
    box.innerHTML = html;
  } catch (e) { showError('tiling-result', e.message); }
}

// ---------- 标签切换 ----------
document.querySelectorAll('.tab').forEach(tab => {
  tab.addEventListener('click', () => {
    document.querySelectorAll('.tab').forEach(t => t.classList.remove('active'));
    document.querySelectorAll('.panel').forEach(p => p.classList.remove('active'));
    tab.classList.add('active');
    document.getElementById('panel-' + tab.dataset.tab).classList.add('active');
  });
});

// ---------- 初始加载：默认跑一次最大子段和 ----------
runSubseq();
