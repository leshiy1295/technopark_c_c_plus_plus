#include "decorator.h"

int main()
{
  
  Data *data = new Data();

  data = new EncryptionDecorator(data);
  data = new CompressionDecorator(data);

  data->read("file.exe");

  delete data;
  return 0;
}