#include "web_styles.h"

namespace WebStyles {

const char* getDashboardStyles() {
  return R"(
    body {
      font-family: Inter, Arial, sans-serif;
      background: linear-gradient(135deg, #0f172a, #1e293b);
      color: #e2e8f0;
      margin: 0;
      padding: 24px;
    }
    .shell {
      max-width: 960px;
      margin: 0 auto;
    }
    h1 {
      margin: 0 0 12px;
    }
    .grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(240px, 1fr));
      gap: 12px;
    }
    .card {
      background: #0b1220;
      border: 1px solid #243148;
      border-radius: 12px;
      padding: 16px;
      box-shadow: 0 10px 30px rgba(0, 0, 0, 0.35);
    }
    .badge {
      display: inline-block;
      padding: 4px 8px;
      border-radius: 999px;
      background: #10b981;
      color: #042f2e;
      font-weight: 700;
      font-size: 12px;
    }
    button {
      background: #2563eb;
      color: white;
      border: none;
      border-radius: 10px;
      padding: 12px 16px;
      font-size: 15px;
      cursor: pointer;
    }
    button:hover {
      background: #1d4ed8;
    }
    a {
      color: #60a5fa;
      text-decoration: none;
    }
    a:hover {
      text-decoration: underline;
    }
  )";
}

const char* getOtaStyles() {
  return R"(
    body {
      font-family: Inter, Arial, sans-serif;
      background: #0b1220;
      color: #e2e8f0;
      margin: 0;
      padding: 24px;
    }
    .card {
      max-width: 520px;
      margin: 0 auto;
      background: #111827;
      border: 1px solid #1f2937;
      border-radius: 12px;
      padding: 20px;
      box-shadow: 0 12px 30px rgba(0, 0, 0, 0.35);
    }
    h2 {
      margin-top: 0;
      color: #e2e8f0;
    }
    button, input[type=submit] {
      background: #2563eb;
      color: white;
      border: none;
      border-radius: 10px;
      padding: 12px 16px;
      font-size: 15px;
      cursor: pointer;
      width: 100%;
      box-sizing: border-box;
    }
    button:hover, input[type=submit]:hover {
      background: #1d4ed8;
    }
    input[type=file] {
      display: block;
      margin-bottom: 12px;
      padding: 8px;
      background: #1f2937;
      border: 1px solid #374151;
      border-radius: 6px;
      color: #e2e8f0;
      width: 100%;
      box-sizing: border-box;
    }
    .progress {
      width: 100%;
      background: #1f2937;
      border-radius: 10px;
      overflow: hidden;
      margin: 12px 0;
      height: 14px;
    }
    .bar {
      width: 0;
      height: 100%;
      background: #10b981;
      transition: width 0.15s ease;
    }
    #status {
      font-size: 14px;
      margin-top: 8px;
      color: #cbd5e1;
      text-align: center;
    }
    p {
      text-align: center;
    }
    a {
      text-decoration: none;
    }
  )";
}

const char* getOtaScript() {
  return R"(
    const f = document.getElementById('otaForm');
    const bar = document.getElementById('bar');
    const st = document.getElementById('status');
    
    f.addEventListener('submit', e => {
      e.preventDefault();
      const fd = new FormData(f);
      const xhr = new XMLHttpRequest();
      xhr.open('POST', '/update');
      xhr.upload.onprogress = e => {
        if (e.lengthComputable) {
          const p = Math.round((e.loaded / e.total) * 100);
          bar.style.width = p + '%';
          st.textContent = 'Uploading ' + p + '%';
        }
      };
      xhr.onload = () => {
        if (xhr.status == 200) {
          bar.style.width = '100%';
          st.textContent = 'Upload OK, rebooting...';
          setTimeout(() => location.href = '/', 4000);
        } else {
          st.textContent = 'Error ' + xhr.status;
        }
      };
      xhr.onerror = () => st.textContent = 'Upload failed';
      xhr.send(fd);
    });
  )";
}

}  // namespace WebStyles
