//
// isPalindrome function (KISS principle)
// Test: Calculate the sum of the integers from 1 to 10,000,000 (inclusive) that are a 
// palindrome in both base-2 and base-10, ignoring leading zeroes.
// 
// (2014) Javier Fuchs
//

(function() {
  isPalindrome = function(str) {
    return str === str.split('').reverse().join('');
  };

}).call(this);
var x = 1;
var sum = 0;
while (x <= 10000000)
{
    if (isPalindrome(x.toString(2)) && isPalindrome(x.toString()))
    {
        print ("x is Palindrome in base (2) = ", x.toString(2), " and in base (10) = ", x.toString());
        	
        sum += x;
    }
    x++;
}
print ("Sum = ", sum);


