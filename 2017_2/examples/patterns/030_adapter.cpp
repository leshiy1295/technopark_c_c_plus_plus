#include <iostream>
#include <stack>

struct XmlReader {
  virtual std::string read() { return "<>\n"; }
};

struct JsonReader {
  std::string fecthJson() { return "{}\n"; }
};

struct Presentaion {
  void Show(XmlReader *reader) {
    std::cout << reader->read(); /*do something with reader*/
  }
};

// composition adapter
struct JsonToXmlAdapter : public XmlReader {
  JsonToXmlAdapter(JsonReader *json_reader) : json_reader_(json_reader) {}

  std::string read() override {
    std::cout << json_reader_->fecthJson();
    return "<>\n";
  }

private:
  JsonReader *json_reader_;
};

// class adapter
struct JsonToXmlAdapterClass : public XmlReader, public JsonReader {
  std::string read() override {
    std::cout << JsonReader::fecthJson();
    return "<>\n";
  }
};

int main() {
  Presentaion p;
  XmlReader *xml_reader = new XmlReader;
  p.Show(xml_reader);


  JsonReader *json_reader = new JsonReader;
  // p.Show(json_reader); // недопустимо

  {
    // composition adaptor
    std::cout << "composition: \n";
    JsonToXmlAdapter* json_to_xml_adater = new JsonToXmlAdapter(json_reader);
    p.Show(json_to_xml_adater);
  }

  {
    // class adaptor
    std::cout << "class: \n";
    JsonToXmlAdapterClass* json_to_xml_adater = new JsonToXmlAdapterClass;
    p.Show(json_to_xml_adater);
  }

  std::stack<int> s;

  return 0;
}
