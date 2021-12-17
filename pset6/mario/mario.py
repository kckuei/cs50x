
# get positive integer beteween 1 and 8 inclusive
height = 0
while height < 1 or height > 8:
    height = input("Height: ")
    if not height.isnumeric():
        height = 0
    else:
        height = int(height)

# generate half pyramids
for i in range(1, height+1):
    string = "#"*i
    print(f"{string:>{height}}  {string:<{height}}".rstrip())
