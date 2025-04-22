// thêm, in, sửa , xóa,min/max, thong ke,sắp xếp, lưu và thoát

#include <iostream>
#include <vector>
#include <regex>
#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>

using namespace std;

struct Student
{
    string id;
    string name;
    string dob;
    string className;
    float math, physics, chemistry;
};

vector<Student> students;

bool isUniqueID(const string &id)
{
    for (auto &s : students)
        if (s.id == id)
            return false;
    return true;
}

bool isLeapYear(int year)
{
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

bool isValidDate(int day, int month, int year)
{
    if (month < 1 || month > 12)
        return false;

    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
    {
        return day >= 1 && day <= 31;
    }

    if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        return day >= 1 && day <= 30;
    }

    if (month == 2)
    {
        if (isLeapYear(year))
        {
            return day >= 1 && day <= 29; // Nam nhuan co 29 ngay
        }
        else
        {
            return day >= 1 && day <= 28; // Nam khong nhuan co 28 ngay
        }
    }

    return false;
}

bool isNotFutureDate(int day, int month, int year)
{
    time_t t = time(nullptr);
    tm *now = localtime(&t);
    int currDay = now->tm_mday;
    int currMonth = now->tm_mon + 1;
    int currYear = now->tm_year + 1900;

    if (year > currYear)
        return false;
    if (year == currYear && month > currMonth)
        return false;
    if (year == currYear && month == currMonth && day > currDay)
        return false;

    return true;
}

bool validateDate(const string &dob)
{
    regex pattern("^(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[0-2])/\\d{4}$");
    if (!regex_match(dob, pattern))
        return false;

    // tach ngay thang nam tu chuoi

    int day, month, year;
    sscanf(dob.c_str(), "%d/%d/%d", &day, &month, &year);

    if (!isValidDate(day, month, year))
        return false;
    if (!isNotFutureDate(day, month, year))
        return false;

    return true;
}

bool validateClass(const string &className)
{
    regex pattern("^(\\d{2}-\\d{1})$");
    return regex_match(className, pattern);
}

bool isValidGrade(float grade)
{
    return grade >= 0 && grade <= 10;
}

void saveToFile()
{
    ofstream out("student.txt");

    if (!out.is_open())
    {
        cout << "Khong the mo file de ghi.\n";
        return;
    }

    for (auto &s : students)
    {
        out << s.id << "," << s.name << "," << s.dob << "," << s.className << "," << s.math << "," << s.physics << "," << s.chemistry << "\n";
    }

    out.close();
}

void addStudent()
{
    Student s;
    cout << "Nhap ID: ";
    cin >> s.id;
    if (!isUniqueID(s.id))
    {
        cout << "ID bi trung.\n";
        return;
    }
    cin.ignore();
    cout << "Nhap ho ten: ";
    getline(cin, s.name);
    do
    {
        cout << "Nhap ngay sinh (dd/mm/yyyy): ";
        getline(cin, s.dob);
    } while (!validateDate(s.dob));

    do
    {
        cout << "Nhap lop (vi du: 10-1): ";
        getline(cin, s.className);
    } while (!validateClass(s.className));

    do
    {
        cout << "Nhap diem Toan (0-10): ";
        cin >> s.math;
    } while (!isValidGrade(s.math));

    do
    {
        cout << "Nhap diem Ly (0-10): ";
        cin >> s.physics;
    } while (!isValidGrade(s.physics));

    do
    {
        cout << "Nhap diem Hoa (0-10): ";
        cin >> s.chemistry;
    } while (!isValidGrade(s.chemistry));

    students.push_back(s);
    saveToFile();
}

void printAll()
{
    for (auto &s : students)
    {
        cout << "ID: " << s.id << ", Ten: " << s.name
             << ", DOB: " << s.dob << ", Lop: " << s.className
             << ", Toan: " << s.math << ", Ly: " << s.physics << ", Hoa: " << s.chemistry << endl;
    }
}

void loadFromFile()
{
    ifstream in("student.txt");

    if (!in.is_open())
    {
        cout << "Khong the mo file de doc.\n";
    }

    students.clear();
    Student s;
    string line;
    while (getline(in, line))
    {
        size_t pos = 0;
        vector<string> parts;
        while ((pos = line.find(",")) != string::npos)
        {
            parts.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }

        parts.push_back(line);
        if (parts.size() == 7)
        {
            s.id = parts[0];
            s.name = parts[1];
            s.dob = parts[2];
            s.className = parts[3];
            s.math = stof(parts[4]);
            s.physics = stof(parts[5]);
            s.chemistry = stof(parts[6]);
            students.push_back(s);
        }
        else
        {
            cout << "Doc bi loi: " << line << "\n";
        }
    }
    in.close();
}

void editByID()
{
    string id;
    cout << "Nhap ID sinh vien can sua: ";
    cin >> id;
    cin.ignore();
    for (auto &s : students)
    {
        if (s.id == id)
        {
            cout << "Tim thay ID: " << id << "\nBan muon sua toan bo thong tin (1) hay mot phan thong tin (2): ";
            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 1)
            {
                cout << "Nhap ten moi: ";
                getline(cin, s.name);
                do
                {
                    cout << "Nhap ngay sinh moi (dd/mm/yyyy): ";
                    getline(cin, s.dob);
                } while (!validateDate(s.dob));

                do
                {
                    cout << "Nhap lop moi (vi du: 10-1): ";
                    getline(cin, s.className);
                } while (!validateClass(s.className));

                do
                {
                    cout << "Nhap diem Toan moi (0-10): ";
                    cin >> s.math;
                } while (!isValidGrade(s.math));

                do
                {
                    cout << "Nhap diem Ly moi (0-10): ";
                    cin >> s.physics;
                } while (!isValidGrade(s.physics));

                do
                {
                    cout << "Nhap diem Hoa moi (0-10): ";
                    cin >> s.chemistry;
                } while (!isValidGrade(s.chemistry));

                cout<<"Da sua thanh cong!\n";
                saveToFile();
            }
            else if (choice == 2)
            {
                int editChoice;
                cout << "Ban muon sua thong tin gi? (1: name, 2: dob, 3: className, 4: grade): ";
                cin >> editChoice;
                cin.ignore();

                
                switch (editChoice)
                {
                case 1:
                    cout << "Nhap ten moi: ";
                    getline(cin, s.name);
                    break;
                case 2:
                    do
                    {
                        cout << "Nhap ngay sinh moi (dd/mm/yyyy): ";
                        getline(cin, s.dob);
                    } while (!validateDate(s.dob));
                    break;
                case 3:
                    do
                    {
                        cout << "Nhap lop moi (vi du: 10-1): ";
                        getline(cin, s.className);
                    } while (!validateClass(s.className));
                    break;
                case 4:
                    
                    int subjectChoice;
                    cout << "Ban muon sua diem mon nao? (1: math, 2: physics, 3: chemistry): ";
                    cin >> subjectChoice;

                   
                    switch (subjectChoice)
                    {
                    case 1:
                        do
                        {
                            cout << "Nhap diem Toan moi (0-10): ";
                            cin >> s.math;
                        } while (!isValidGrade(s.math)); // Kiểm tra điểm Toán
                        break;
                    case 2:
                        do
                        {
                            cout << "Nhap diem Ly moi (0-10): ";
                            cin >> s.physics;
                        } while (!isValidGrade(s.physics)); // Kiểm tra điểm Ly
                        break;
                    case 3:
                        do
                        {
                            cout << "Nhap diem Hoa moi (0-10): ";
                            cin >> s.chemistry;
                        } while (!isValidGrade(s.chemistry)); // Kiểm tra điểm Hóa  
                        break;
                    default:
                        cout << "Lua chon mon hoc khong hop le.\n";
                        return;
                    }
                    break;
                default:
                    cout << "Lua chon khong hop le.\n";
                    return;
                }
                cout<<"Da sua thanh cong!\n";
                saveToFile();
            }
            else cout<< "Lua chon khong hop le!\n"; 
        
            return;
        }
    }
    cout << "Khong tim thay ID: " << id << "\n";
}

void deleteByID()
{
    string id;
    cout << "Nhap ID sinh vien can xoa: ";
    cin >> id;
    auto it = remove_if(students.begin(), students.end(),
                        [&](Student &s)
                        { return s.id == id; });
    if (it != students.end())
    {
        students.erase(it, students.end());
        cout << "Da xoa ID: " << id << "\n";
        saveToFile();
    }
    else
        cout << "Khong ton tai ID: " << id << "\n";
}

float getSubjectScore(const Student& s, const string& subject){
    if(subject == "math") return s.math;
    if(subject == "physics") return s.physics;
    return s.chemistry;
}

void findExtremum(const string& subject,bool isMax ){
    if(students.empty()) {
        cout<<"Danh sach rong\n";
        return;
    }

    auto comp = [&] (Student&a, Student&b){
        float va = getSubjectScore(a,subject);
        float vb = getSubjectScore(b,subject);
        return va<vb;
    };

    auto result = isMax ?  *max_element(students.begin(),students.end(), comp):
                           *min_element(students.begin(),students.end(), comp);  
    cout << (isMax ? "Max" : "Min")<<" diem mon "<<subject<<": "
         <<result.name<< "(" << getSubjectScore(result, subject)<<")\n";
}

void statistics (const string& subject){
    float sum=0, mean,stddve=0;
    for(auto& s : students){
        float val = getSubjectScore(s, subject);
        sum+=val;
    }
    mean=sum/students.size();

    for(auto& s : students){
        float val = getSubjectScore(s, subject);
        stddve+=pow(val-mean,2);
    }

    stddve=sqrt(stddve/students.size());
    cout<<"Diem trung binh mon "<<subject<<": "<<mean<<", Do lech chuan: "<<stddve<<"\n";
}

void sortBySubject(const string& subject, bool ascending){
    sort(students.begin(),students.end(),[&](Student& a, Student& b){
        float va= getSubjectScore(a,subject);
        float vb= getSubjectScore(b,subject);
        return ascending ? va<vb : va>vb;

        
    });
    cout<<"Danh sach sau khi sap xep la: \n";
    printAll();
    saveToFile();
}


int main()
{
    loadFromFile();
    int choice;

    while (choice != 8)
    {
        cout << "1. Them sinh vien\n2. In danh sach\n3. Sua theo ID\n4. Xoa theo ID\n5. Tim diem cao/thap\n6. Thong ke\n7. Sap xep\n8. Luu va thoat\nChon: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            addStudent();
            break;
        case 2:
            printAll();
            break;
        case 3:
            editByID();
            break;
        case 4:
            deleteByID();
            break;
        case 5:{
            string mon; 
            bool max;
            cout<< "Nhap mon (math/physics/chemistry): "; cin>>mon;
            cout<< "Tim diem cao nhat (1) hay thap nhat (0): ";cin>>max;
            findExtremum(mon, max);
            break;
        }
        case 6:{
            string mon;
            cout<< "Nhap mon (math/physics/chemistry): "; cin>>mon;
            statistics(mon);
            break;
        }
        case 7:{
            string mon; bool asc;
            cout<< "Nhap mon (math/physics/chemistry): "; cin >> mon;
            cout<< "Tang dan (1) hay giam dan (0): ";cin >> asc; 
            sortBySubject(mon, asc);
            break;          
        }
            
        case 8:{
            saveToFile();
            cout<<"Da luu va thoat";
            break;
        }
        }
    }
}