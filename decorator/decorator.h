#pragma once

#include <string>

struct Data
{
public:
   virtual bool read(const std::string &path);
   virtual bool write(const std::string &path);
 
 private:
   std::string m_text;
};

struct DataDecorator : public Data
{
public:
    DataDecorator(Data *data) : m_data(data) {}

    virtual bool read(const std::string &path);
    virtual bool write(const std::string &path);

  private:
    Data *m_data; // никому не доступно
};



struct EncryptionDecorator : public DataDecorator
{
public: 
    EncryptionDecorator(Data *data) : DataDecorator(data) {}

    virtual bool read(const std::string &path);
    virtual bool write(const std::string &path);

private:
    void encrypt(const std::string &text);
    void decrypt(const std::string &text);
};


struct CompressionDecorator : public DataDecorator
{
public: 
    CompressionDecorator(Data *data) : DataDecorator(data) {}

    virtual bool read(const std::string &path);
    virtual bool write(const std::string &path);

private:
    void compress(const std::string &text);
    void decompress(const std::string &text);
};