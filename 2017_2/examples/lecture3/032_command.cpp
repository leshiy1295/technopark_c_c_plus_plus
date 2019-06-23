#include <iostream>

#include <string>

using namespace std;

struct Document {
  void insert() {
    std::cout << "insert command\n";
  }
  void remove(){
    std::cout << "remove command\n";
  }
  void copy(){
    std::cout << "copy command\n";
  }
};

struct Command  {
  explicit Command(Document* doc) : doc_(doc) {}
  virtual void execute() = 0;
  virtual void custom_execute() {};

 protected:
  Document* doc_;
};

struct CopyCommand : public Command {
  explicit CopyCommand(Document* doc) : Command(doc) {}
  void execute() {
    doc_->copy();
  }
};

struct RemoveCommand : public Command {
  explicit RemoveCommand(Document* doc) : Command(doc) {}
  void execute() {
    doc_->remove();
  }
};

struct CustomCommand : public Command {
  using ExCommand = std::function<void(Document &)>;
  using AnyCommand = std::function<void()>;

  explicit CustomCommand(Document *doc, ExCommand command)
      : Command(doc), command_(command) {
  }

  explicit CustomCommand(AnyCommand command)
      : Command(nullptr), any_command_(command) {}

  void execute() override {
    command_(*doc_);
  }

  void custom_execute() override {
    any_command_();
  }

  ExCommand command_;
  AnyCommand any_command_;
};

// --------------------------------

struct CopyButton {
  void setCommand(Command *command) {
    command_ = command;
  }

  void onClick() {
    command_->execute();
  }

private:
  Command *command_;
};

int main() {
  Document doc;
  Command* copyCommand = new CopyCommand(&doc);

  CopyButton btn;
  btn.setCommand(copyCommand);
  btn.onClick();

  {
    std::cout << "Custom command \n";

    Document doc;


    std::function<void(Document&)> copy_function = std::bind(&Document::copy,
                                                             std::placeholders::_1);

    std::function<void(void)> copy_function2 = std::bind(&Document::copy, &doc);

    //
    //
    copy_function(doc);
    copy_function2();

    {
      CustomCommand custom_command(&doc, copy_function);
      custom_command.execute();
    }

    {
      CustomCommand custom_command(copy_function2);
      custom_command.custom_execute();
    }
  }


  return 0;
}
