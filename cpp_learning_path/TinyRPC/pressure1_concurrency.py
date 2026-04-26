# pressure1_simple.py  不用 Barrier，直接每个 worker 跑完整流程
import socket, threading, sys

N = 100
errors = []

def worker(i):
    try:
        s = socket.socket()
        s.connect(('127.0.0.1', 8080))
        payload = f"msg-{i:03d}\n".encode()
        s.sendall(payload)
        s.shutdown(socket.SHUT_WR)
        buf = b''
        s.settimeout(5)     # 5 秒超时，hang 住立刻暴露
        while True:
            chunk = s.recv(4096)
            if not chunk: break
            buf += chunk
        assert buf == payload, f"worker {i}: want {payload!r} got {buf!r}"
        s.close()
    except Exception as e:
        errors.append((i, type(e).__name__, str(e)))

ts = [threading.Thread(target=worker, args=(i,)) for i in range(N)]
for t in ts: t.start()
for t in ts: t.join()

if errors:
    print(f"FAIL: {len(errors)}/{N}")
    for i, k, e in errors[:10]:
        print(f"  worker {i}: {k}: {e}")
    sys.exit(1)
print(f"PASS: {N} workers all echoed correctly")