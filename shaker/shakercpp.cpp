#include <iostream>

using namespace std;

int main(int argc, char** argv) {

  cout << "hello world" << endl;
  if (argc > 1)
    cout << "arg1:" << argv[1] << endl ;
  return 0;
}
