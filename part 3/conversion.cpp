#include <bits/stdc++.h>
using namespace std;

int main() {
  float s = 33.233539458239;
  char* n = (char *) &s;
  float t = *(float *) n;
  cout << t;
  
    /*
  float number = 33.233539458239;
  bool t;
  char* numberstring = new char[(((sizeof number) * CHAR_BIT) + 2)/3 + 2];
  // char numberstring[(((sizeof number) * CHAR_BIT) + 2)/3 + 2];
  sprintf(numberstring, "%f", number);
  cout << numberstring << '\n';
  std::cout << "OK" << '\n';

  int i = 0;
  // int num = 0
  while(true) {
    t = (numberstring[i] == '\0');
    if (t) {
      break;
    }
    if(numberstring[i] == '.'){

    }
    std::cout << numberstring[i] << '\n';
    // std::cout << t << '\n';
    i++;
  }
  */

  return 0;
}
