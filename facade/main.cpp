/* 
 * пример из https://github.com/ehsangazar/design-patterns-cpp.git
 */


#include "file_transfer.h"

int main() {
    FileTransfer* ft = new FileTransfer();
    ft->sendFile("mypicture");
    delete ft;
}