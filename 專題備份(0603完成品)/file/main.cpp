#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <iomanip>

#define gradFileName grad_2

#define debug true
#define GradFileName "grade.csv"
#define schoolFileName "school.csv"
#define aspirationFileName "aspiration.csv"
#define finalFile "school_aspiration_result.csv"
#define stuFinalFile "student_aspiration_result.csv"

using namespace std;

struct _grad
{

    int ID;
    int A;
    int B;
    int C;
    int D;
    int E;

    int total;

    vector<string> aspiration;

    string final;

} typedef Grad;

struct _school
{
    string name;
    int size;
    vector<int> site;
    // int position;  // 驗證用，和map的取值做對照

} typedef School;

fstream file;

vector<Grad> grad_data; // 儲存strudent List

vector<School> school_data; //儲存school List
map<string, int> school_position;

vector<int> splitTOint(char *a, string target)
{

    vector<int> result;

    const char *d = target.c_str(); //分割依據
    char *p;                        //儲存每次分割結果
    p = strtok(a, d);

    while (p)
    {
        // cout<<p<<endl;
        result.push_back(atoi(p));
        p = strtok(NULL, d);
    }

    return result;
}

vector<string> split(char *a, string target)
{

    vector<string> result;

    const char *d = target.c_str(); //分割依據
    char *p;                        //儲存每次分割結果
    p = strtok(a, d);

    while (p)
    {
        // cout<<p<<endl;
        result.push_back(p);
        p = strtok(NULL, d);
    }

    return result;
}

int forGradSum(vector<int> a)
{

    // 第一項學號不取

    int result = 0;
    int temp = 0;

    for (int i = 1; i < a.size(); i++)
    {
        temp = a[i];
        result += temp;
    }

    return result;
}

bool openFile(string name, string inORout)
{
    if (file.is_open())
        file.close();

    if (inORout == "in")
    {
        file.open(name, ios::in);
    }
    else if (inORout == "out")
    {
        file.open(name, ios::out);
    }
    else
    {
        cout << "\nwrong argument" << endl;
        return false;
    }

    if (!file.is_open())
    {
        cout << "\nfile oepn failed : " << name << endl;
        file.close();
        return false;
    }

    return true;
}

void getStudentGrad()
{
    /*
    取得學生資料，並把數量存到全域變數 vector<Grad> grad_data
    */

    file.seekg(0, ios::beg); // 移動讀指標到檔案開頭往後移 0 個位置

    char temp[30];
    while (file.getline(temp, 60))
    {

        vector<int> temp_vec = splitTOint(temp, ",");
        if (temp_vec.size() != 6)
        {
            cout << "\ngrad data wrong !!!\n ID : " << temp_vec[0];
            continue;
        }
        Grad g{temp_vec[0], temp_vec[1], temp_vec[2], temp_vec[3], temp_vec[4], temp_vec[5], forGradSum(temp_vec)};

        grad_data.push_back(g);
    }

    cout << "student size : " << grad_data.size() << endl;
}

void getSchool()
{
    char line[30];
    file.getline(line, 30);

    for (int i = 0; file.getline(line, 30); i++)
    {
        vector<string> a = split(line, ",");

        School temp = {a[0], atoi(a[1].c_str())};
        // temp.position = i;

        school_position[a[0]] = i;

        school_data.push_back(temp);

        // cout << line << endl;
    }

    cout << "school size : " << school_data.size() << endl;
}

void getAspiration()
{
    /*
        取得學生志願，目前一人只能取100個文字，若是要更多可以把學校改成代號，去做學校的編碼
    */

    char line[500];
    while (file.getline(line, 500))
    {
        vector<string> a = split(line, ",");

        int ID = atoi(a[0].c_str());

        a.erase(a.begin());

        grad_data[ID].aspiration = a;
        // cout << ID << endl;
    }

    cout << "aspiration get" << endl;
}

bool sort_grad(Grad a, Grad b)
{
    // cout << a.ID << ", " << b.ID << endl;

    if (a.total != b.total)
        return a.total > b.total;
    else if (a.A != b.A)
        return a.A > b.A;
    else if (a.B != b.B)
        return a.B > b.B;
    else if (a.C != b.C)
        return a.C > b.C;
    else if (a.D != b.D)
        return a.D > b.C;
    else
        return a.E > b.E;
}

void assignAspiration_assign()
{

    // cout << grad_data.size() << endl;

    for (int i = 0; i < grad_data.size(); i++)
    {
        // 迭帶所有的學生名單

        // cout << "test 1" << endl;

        vector<string>
            aspiration;
        // cout << grad_data[i].ID << endl;
        aspiration.swap(grad_data[i].aspiration);

        // cout << aspiration.size();

        for (int j = 0; j < aspiration.size(); j++)
        {
            // 迭帶學生的所有志願

            int position = school_position[aspiration[j]];

            if (school_data[position].size > school_data[position].site.size())
            {
                // 若是缺額比名單還多時，填上志願並跳離圓圈
                // cout << school_data[position].name << " , " << grad_data[i].ID << endl;
                school_data[position].site.push_back(grad_data[i].ID);
                grad_data[i].final = school_data[position].name; // 把志願結果寫入學生資料
                break;
            }
            else
            {
                // 若是缺額等於或大於名單時，往下一個志願填
                continue;
            }
        }
    }

    cout << "assign done" << endl;
}

void assignAspiration_makeFile()
{

    /*
        把學校的入取名單寫入檔案
    */

    for (int i = 0; i < school_data.size(); i++)
    {
        // 迭代所有學校
        file << school_data[i].name;
        for (int j = 0; j < school_data[i].site.size(); j++)
        {
            // 迭代學校內的名單
            file << "," << setfill('0') << setw(6) << school_data[i].site[j];
        }
        file << "\n";
    }
    cout << "school final file output done" << endl;
}

void stu_assignAspiration_makeFile()
{

    //寫入學生最終上哪間的檔案

    for (int i = 0; i < grad_data.size(); i++)
    {
        file << setfill('0') << setw(6) << grad_data[i].ID << "," << grad_data[i].final << "\n";
    }
    cout << "student final file output done" << endl;
}

int main()
{
    srand(time(NULL));

    // construct_gradFile("grad-2");

    // 取得成績檔案資料
    if (!openFile(GradFileName, "in"))
    {
        goto badendd;
    }
    getStudentGrad();
    file.close();

    // searchMy(); // 查詢自己的成績

    // 取得學校的名額
    if (!openFile(schoolFileName, "in"))
    {
        goto badendd;
    }
    getSchool();
    file.close();

    // 取得學生的志願
    if (!openFile(aspirationFileName, "in"))
    {
        goto badendd;
    }
    getAspiration();
    file.close();

    //  // 測試排序資料
    // for (vector<Grad>::iterator p = grad_data.begin(); p != grad_data.end();p++){
      
    //     cout << p->ID << "\n";
    // }

    // 排序
    sort(grad_data.begin(), grad_data.end() + 1, sort_grad); // 排序資料
    if (debug)
    {
        cout << "sort done" << endl;
    }


//  // 測試排序資料
//     for (vector<Grad>::iterator p = grad_data.begin(); p != grad_data.end();p++){
      
//         cout << p->ID << "\n";
//     }


    //分發志願

    assignAspiration_assign();

    if (!openFile(finalFile, "out"))
    {
        goto badendd;
    }
    assignAspiration_makeFile();
    file.close();

    if (!openFile(stuFinalFile, "out"))
    {
        goto badendd;
    }
    stu_assignAspiration_makeFile();
    file.close();

    system("pause");
    return 0;

badendd:

    return -1;
}
