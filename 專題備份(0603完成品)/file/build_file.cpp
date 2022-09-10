#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <iomanip>

using namespace std;

void construct_gradFile(string fileName, int size)
{

    // char name[fileName.length()];

    // strcpy(name, fileName.c_str());

    // sprintf(name, "%s.csv", fileName.c_str());

    ofstream grad(fileName, ios::out);
    if (!grad.is_open())
    {
        cout << " ---- file open failed : " << fileName;
    }
    else
    {

        for (int i = 0; i < size; i++)
        {
            grad << setfill('0') << setw(6) << i << "," << rand() % 100 << "," << rand() % 100 << "," << rand() % 100 << "," << rand() % 100 << "," << rand() % 100 << "\n";
            
        }
        cout << "gradFileInit done\n";

        grad.close();
    }
}

void construct_aspiration(string fileName, int studentSize){

    /*
    新增學生志願序檔案
    */

    ofstream wFile(fileName.c_str(), ios::out);
    ifstream rFile("school.csv", ios::in);

    vector<string> schoolList2;

    char line[60];

    if(!rFile.is_open() || !wFile.is_open()){
        cout << "construct_aspiration file cannot open" << endl;
    }else{

        rFile.getline(line, 60);

        // 取得學校資料
        while(rFile.getline(line, 60)){

            string b = line;
            // cout << b.substr(0, b.find(',')) << endl;

            schoolList2.push_back(b.substr(0, b.find(',')));
        }

        // 建立學生志願資料
        for (int i = 0; i < studentSize;i++){
            wFile << setfill('0') << setw(6) << i;  // 先放學號
            int size = schoolList2.size();

            // 隨機把schoolList2(學校)寫進FILE(志願檔案)裡面
            while (size)
            {
                int position = rand() % size; 

                // cout << " --- " << position << " --- "; // 隨機數
                // cout << "," << *(schoolList2.begin() + position); // 被抽到的

                // if(size != 1)

                //把被抽到的那一項 跟 還沒被抽的最後一項做交換 => 被抽到的項目都會放在最後面
                swap(*(schoolList2.begin() + position), *(schoolList2.begin() + size - 1));  

                wFile << "," << *(schoolList2.begin() + size - 1);  // 把最後一項寫進FILE裡面

                size -= 1;
            }
            // cout << '\n';
            wFile << '\n';
        }
        cout << "write done" << endl;
        rFile.close();
        wFile.close();
    }

}


int main(){

    srand(time(NULL));

    int studentSize;

    cout << "how many student you want build (input integer) ? : ";
    cin >> studentSize;

    construct_gradFile("grade.csv", studentSize);
    construct_aspiration("aspiration.csv", studentSize);

    system("pause");
}