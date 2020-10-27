#include "decorator.h"

#include <iostream>


bool Data::read(const std::string &path)
{
  std::cout << "Data read " << std::endl;
  return true;
}

bool Data::write(const std::string &path)
{
  std::cout << "Data write " << std::endl;
  return true;
}


bool DataDecorator::read(const std::string &path)
{
  //std::cout << "DataDecorator:read" << std::endl;
  return m_data->read(path);
}

bool DataDecorator::write(const std::string &path)
{
  return m_data->write(path);
}

//////////////////////////////////////////////////

bool EncryptionDecorator::read(const std::string &path)
{
  //std::cout << "EncryptionDecorator:read" << std::endl;

  bool res = DataDecorator::read(path);

  if (!res)
    return res;

  decrypt("Text from Data");

  return true;
}

bool EncryptionDecorator::write(const std::string &path)
{
   encrypt("Text from Data");

   return DataDecorator::write(path);
}

void EncryptionDecorator::encrypt(const std::string &text)
{
  // todo encrypt
  std::cout << "encrypt" << std::endl;
}

void EncryptionDecorator::decrypt(const std::string &text)
{
  // todo decrypt
  std::cout << "decrypt" << std::endl;
}

//////////////////////////////////////////////////

bool CompressionDecorator::read(const std::string &path)
{
   //std::cout << "CompressionDecorator:read" << std::endl;
   bool res = DataDecorator::read(path);

   if (!res)
     return res;
   
   decompress("Text from Data");

   return true;
}

bool CompressionDecorator::write(const std::string &path)
{
   compress("Text from Data");

   return DataDecorator::write(path);
}

void CompressionDecorator::compress(const std::string &text)
{
  // todo compress
  std::cout << "compress" << std::endl;
}

void CompressionDecorator::decompress(const std::string &text)
{
  // todo decompress
  std::cout << "decompress" << std::endl;
}

