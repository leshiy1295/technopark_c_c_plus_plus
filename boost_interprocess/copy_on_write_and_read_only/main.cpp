#include <boost/interprocess/managed_mapped_file.hpp>
#include <fstream> //std::fstream
#include <iterator> //std::distance

int main() {
    using namespace boost::interprocess;
    const char *ManagedFile = "MyManagedFile";
    const char *ManagedFile2 = "MyManagedFile2";
    file_mapping::remove(ManagedFile);
    file_mapping::remove(ManagedFile2);
    remove_file_on_destroy destroyer1(ManagedFile);
    remove_file_on_destroy destroyer2(ManagedFile2);
    { // copy-on-write
        managed_mapped_file managed_file(create_only, ManagedFile, 65536);
        managed_file.construct<int>("MyInt")(0);
        managed_mapped_file managed_file_cow(open_copy_on_write, ManagedFile); // открываем copy-on-write (cow)
        if (!managed_file_cow.destroy<int>("MyInt")) { throw 42; } // очищаем данные в новом cow-сегменте
        managed_file_cow.construct<int>("MyInt2")(0); // создаём новую сущность в cow-сегменте
        if (managed_file_cow.find<int>("MyInt").first || !managed_file_cow.find<int>("MyInt2").first) { throw 42; }
        if (!managed_file.find<int>("MyInt").first || managed_file.find<int>("MyInt2").first) { throw 42; }
        { // сохраняем данные в файл
            std::fstream file(ManagedFile2, std::ios_base::out | std::ios_base::binary);
            if (!file) { throw 42; }
            file.write(static_cast<const char *>(managed_file_cow.get_address()), (std::streamsize)managed_file_cow.get_size());
        }
        // читаем данные из файла и проверяем соответствие
        managed_mapped_file managed_file_cow2(open_only, ManagedFile2);
        if (managed_file_cow2.find<int>("MyInt").first || !managed_file_cow2.find<int>("MyInt2").first) { throw 42; }
    }
    { // read-only
        managed_mapped_file managed_file_ro(open_read_only, ManagedFile);
        if (!managed_file_ro.find<int>("MyInt").first || managed_file_ro.find<int>("MyInt2").first) { throw 42; }
        if (std::distance(managed_file_ro.named_begin(), managed_file_ro.named_end()) != 1 || std::distance(managed_file_ro.unique_begin(), managed_file_ro.unique_end()) != 0) { throw 42; }
    }
}

