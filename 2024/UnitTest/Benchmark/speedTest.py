import time
start = time.time()
for i in range(0, 0x7FFFFFFF):
    i
end = time.time()
print(end - start)
