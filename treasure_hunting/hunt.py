n, m = map(int, input().split())
print("scan", 1, 1)
r1 = int(input())
print("scan", n, 1)
r2 = int(input())
x = (r1 - r2 + 2 * n + 2) / 4
y = (r1 + r2 + 6 - 2 * n) / 4
print("center:", x, y)
print("scan", round(x), round(y))
rc = int(input())
print("scan", round(x), 1)
rd = int(input())
y1 = (rd - rc + 2) / 2
y2 = 2 * y - y1
x2 = (2 * x + rc - y2 + y1) / 2
x1 = 2 * x - x2
print("dig", x1, y1)
if input() == "1":
    print("dig", x2, y2)
else:
    print("dig", x1, y2)
    print("dig", x2, y1)
