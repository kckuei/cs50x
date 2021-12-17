# Get credit card number
cc_num = -1
while cc_num < 0:
    cc_num = input("Number: ")
    if not cc_num.isnumeric():
        cc_num = -1
    else:
        cc_num = int(cc_num)

# Credit card number as string
cc_str = str(cc_num)

# Double and sum digits of every other number from 2nd to last
# Some those digits with every other digit from last not doubled
sum1 = 0
n = len(cc_str)
for i in range(n-2, -1, -2):
    num = int(cc_str[i])*2
    sum1 += num % 10 + num//10

sum2 = 0
for i in range(n-1, -1, -2):
    sum2 += int(cc_str[i])

checksum = sum1 + sum2

# Check for no remainder ot be valid
if checksum % 10 == 0:
    valid_check_sum = True
else:
    valid_check_sum = False

# Get first two digits
first_two_digits = int(cc_str[0:2])

# Get the CC type
if valid_check_sum and n == 15 and (first_two_digits == 34 or first_two_digits == 37):
    print("AMEX")
elif valid_check_sum and n == 16 and first_two_digits > 50 and first_two_digits < 56:
    print("MASTERCARD")
elif valid_check_sum and (n == 13 or n == 16) and (first_two_digits // 10 == 4):
    print("VISA")
else:
    print("INVALID")

