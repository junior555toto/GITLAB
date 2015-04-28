#include <stdio.h>
 
int main()
{
  int c, n, fact = 1;
<<<<<<< HEAD
  printf(">>>User-B add this line for problem18 [time 1]<<<\n");
  printf("Enter a number to calculate it's factorial\n");
=======
 
  printf("Enter a number to calculate it's factorial: ");
>>>>>>> b20f0d916b94895961f6cbcbc06bb9fdef6da90c
  scanf("%d", &n);
 
  for (c = 1; c <= n; c++)
    fact = fact * c;
<<<<<<< HEAD
  printf(">>>>>User-B add this line for problem18 [time 2]<<<\n");
  printf("Factorial of %d is %d\n", n, fact);
=======
 
  printf("Factorial of %d = %d\n", n, fact);
>>>>>>> b20f0d916b94895961f6cbcbc06bb9fdef6da90c
 
  return 0;
}