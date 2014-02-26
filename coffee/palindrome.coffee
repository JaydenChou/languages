# 
# Javier Fuchs (Year 14)
#
x = 1
sum = 0
isPalindrome = (str) -> str is str.split('').reverse().join('');
while x <= 10000000
    sum += x  if isPalindrome(x.toString(2)) and isPalindrome(x.toString())
    x++
print "The great pleasure in my life is doing what people say you cannot do. Sum = ", sum
#
# EOF
#
