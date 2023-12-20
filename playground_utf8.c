// UTF-8 Endcoding in C
// By Sarale Goldberger

#include <stdio.h>

int main(void)
{
  // Variable declarations
  int input;
  char* ans;
  char again = 'y';

  // Prompt the user for input
  printf("===== Emoji Text Replacement =====\n");
  printf("Options:\n1. Happy\n2. Sad\n3. Excited\n4. Celebrate\n5. Nervous\n");

  while (again == 'y') {
	// Take user input
	printf("Enter number of option you wish to choose: ");
	scanf("%d", &input);
	  
	switch(input) {
	case 1:
		ans = "\U0001F600";
		break;
	case 2:
		ans = "\U0001F62D";
		break;
	case 3:
		ans = "\U0001F606";
		break;
	case 4:
		ans = "\U0001F973";
		break;
	case 5:
		ans = "\U0001F605";
		break;
	default:
		ans = "Number out of range";
		break;
	  }

	printf("%s\n", ans);
	printf("Click y to choose again\n");
	scanf("%s", &again);

  } // end while loop
 
} // end main
