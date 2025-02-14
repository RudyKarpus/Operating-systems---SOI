#include <iostream>
#include <vector>
#include <cstring>
#include "fs.cpp";
int main() {
    FileSystem fs;
    try {
        int descriptor_index1 = fs.createFile("test");
        vector<char> data1(70, 'A');
        fs.write(descriptor_index1, data1, false, false, false, false);
        fs.print();
        vector<char> data2(49, 'A');
        fs.write(descriptor_index1, data2, true, false, false, false);
        fs.repair();
        fs.print();
        vector<char> data3(200, 'A');
        fs.write(descriptor_index1, data3, false, true, false, false);
        fs.repair();
        fs.print();
        vector<char> data4(151, 'A');
        fs.write(descriptor_index1, data4, false, false, true, false);
        fs.repair();
        fs.print();
        vector<char> data5(30, 'A');
        fs.write(descriptor_index1, data5, false, false, false, true);
        fs.repair();
        fs.print();



    }

    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }


    return 0;
}
