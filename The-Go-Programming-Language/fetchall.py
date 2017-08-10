import requests
import sys
import time
import multiprocessing

def fetch(url):
    ts = time.time()
    try:
        resp = requests.get(url, timeout=10)
    except:
        print("Timeout {}".format(url))
        return
    te = time.time()
    print("{:.2f}s {:7} {}".format(te-ts, len(resp.content), url))

def fetchall(urls):
    ts = time.time()
    jobs = []
    for url in urls:
        if not url.startswith("http"):
            url = "http://" + url
        p = multiprocessing.Process(target=fetch, args=(url,))
        jobs.append(p)
        p.start()
    for job in jobs:
        job.join()
    te = time.time()
    print("{:.2f}s elapsed".format(te-ts))

if __name__ == "__main__":
    fetchall(sys.argv[1:])