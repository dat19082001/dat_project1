#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;

void saveToFile();

struct Student {
    string id;
    string name;
    string dob;
    string className;
    float math, physics, chemistry;
};

vector<Student> students;

bool isLeapYear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

bool isValidDate(int day, int month, int year) {
    // Kiem tra so ngay hop le trong moi thang
    if (month < 1 || month > 12) return false;

    // Cac thang co 31 ngay
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
        return day >= 1 && day <= 31;
    }
    // Cac thang co 30 ngay
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return day >= 1 && day <= 30;
    }
    // Thang 2, kiem tra nam nhuan
    if (month == 2) {
        if (isLeapYear(year)) {
            return day >= 1 && day <= 29; // Nam nhuan co 29 ngay
        } else {
            return day >= 1 && day <= 28; // Nam khong nhuan co 28 ngay
        }
    }
    return false; // Tra ve false neu khong dung thang
}

// Kiem tra ngay khong vuot qua ngay hien tai
bool isNotFutureDate(int day, int month, int year) {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int curDay = now->tm_mday;
    int curMonth = now->tm_mon + 1; // tm_mon bat dau tu 0
    int curYear = now->tm_year + 1900;

    if (year > curYear) return false;
    if (year == curYear && month > curMonth) return false;
    if (year == curYear && month == curMonth && day > curDay) return false;

    return true;
}

bool validateDate(const string& dob) {
    regex pattern("^(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[012])/\\d{4}$");
    if (!regex_match(dob, pattern)) return false;

    // Tach ngay, thang, nam tu chuoi
    int day, month, year;
    sscanf(dob.c_str(), "%d/%d/%d", &day, &month, &year);

    if (!isValidDate(day, month, year)) return false;
    if (!isNotFutureDate(day, month, year)) return false;

    return true;

}

bool validateClass(const string& className) {
    regex pattern("^\\d{2}-\\d+$");
    return regex_match(className, pattern);
}

bool isUniqueID(const string& id) {
    for (auto& s : students) if (s.id == id) return false;
    return true;
}

bool isValidGrade(float grade) {
    return grade >= 0 && grade <= 10; // Kiểm tra điểm có nằm trong khoảng 0-10 không
}

void addStudent() {
    Student s;
    cout << "Nhap ID: ";
    cin >> s.id;
    if (!isUniqueID(s.id)) {
        cout << "ID bi trung.\n";
        return;
    }
    cin.ignore();
    cout << "Nhap ho ten: ";
    getline(cin, s.name);
    do {
        cout << "Nhap ngay sinh (dd/mm/yyyy): ";
        getline(cin, s.dob);
    } while (!validateDate(s.dob));
    do {
        cout << "Nhap lop (vi du: 10-1): ";
        getline(cin, s.className);
    } while (!validateClass(s.className));
   
    // Nhập điểm và kiểm tra tính hợp lệ
    do {
        cout << "Nhap diem Toan (0-10): ";
        cin >> s.math;
    } while (!isValidGrade(s.math)); // Kiểm tra điểm Toán

    do {
        cout << "Nhap diem Ly (0-10): ";
        cin >> s.physics;
    } while (!isValidGrade(s.physics)); // Kiểm tra điểm Lý

    do {
        cout << "Nhap diem Hoa (0-10): ";
        cin >> s.chemistry;
    } while (!isValidGrade(s.chemistry)); // Kiểm tra điểm Hóa
    
    students.push_back(s);
    saveToFile();  
}

void printAll() {
    for (auto& s : students) {
        cout << "ID: " << s.id << ", Ten: " << s.name
             << ", DOB: " << s.dob << ", Lop: " << s.className
             << ", Toan: " << s.math << ", Ly: " << s.physics << ", Hoa: " << s.chemistry << endl;
    }
}

void editStudentByID() {
    string id;
    cout << "Nhap ID sinh vien can sua: ";
    cin >> id;
    for (auto& s : students) {
        if (s.id == id) {
            cin.ignore();
            cout << "Nhap ten moi: "; getline(cin, s.name);
            do {
                cout << "Nhap ngay sinh moi: "; getline(cin, s.dob);
            } while (!validateDate(s.dob));
            do {
                cout << "Nhap lop moi: "; getline(cin, s.className);
            } while (!validateClass(s.className));
            cout << "Nhap diem moi: "; cin >> s.math >> s.physics >> s.chemistry;
            saveToFile();
            return;
        }
    }
    cout << "Khong tim thay sinh vien.\n";
}

void deleteStudentByID() {
    string id;
    cout << "Nhap ID sinh vien can xoa: ";
    cin >> id;
    auto it = remove_if(students.begin(), students.end(), [&](Student s) { return s.id == id; });
    if (it != students.end()) {
        students.erase(it, students.end());
        saveToFile();
        cout << "Da xoa.\n";
    } else cout << "Khong tim thay.\n";
}

float getSubjectScore(const Student& s, const string& subject) {
    if (subject == "math") return s.math;
    if (subject == "physics") return s.physics;
    return s.chemistry;  // Nếu không phải math hoặc physics thì lấy chemistry
}


void findExtremum(const string& subject, bool isMax) {
    if (students.empty()) return;

    auto comp = [&](Student& a, Student& b) {
        float va = getSubjectScore(a, subject);  // Lấy điểm môn học từ sinh viên a
        float vb = getSubjectScore(b, subject);  // Lấy điểm môn học từ sinh viên b
        return va < vb;  // So sánh theo max hoặc min
    };

    // Tìm sinh viên có điểm cao nhất (max) hoặc thấp nhất (min)
    auto result = isMax ? *max_element(students.begin(), students.end(), comp) : 
                          *min_element(students.begin(), students.end(), comp);

    // In kết quả
    cout << (isMax ? "Max" : "Min") << " diem mon " << subject << ": " 
         << result.name << " (" << getSubjectScore(result, subject) << ")\n";
}


void statistics(const string& subject) {
    float sum = 0, mean, stddev = 0;
    for (auto& s : students) {
        float val = getSubjectScore(s, subject);  // Sử dụng hàm getSubjectScore
        sum += val;
    }
    mean = sum / students.size();
    for (auto& s : students) {
        float val = getSubjectScore(s, subject);  // Sử dụng hàm getSubjectScore
        stddev += pow(val - mean, 2);
    }
    stddev = sqrt(stddev / students.size());
    cout << "Diem trung binh mon " << subject << ": " << mean << ", Do lech chuan: " << stddev << endl;
}



void saveToFile() {
    ofstream out("students.txt");
    // Kiểm tra mở file thành công không
    if (!out.is_open()) {
        cout << "Khong the mo file de ghi.\n";
        return;
    }
    for (auto& s : students) {
        out << s.id << "," << s.name << "," << s.dob << "," << s.className << "," << s.math << "," << s.physics << "," << s.chemistry << "\n";
    }
    out.close();
}

void sortBySubject(const string& subject, bool ascending) {
    sort(students.begin(), students.end(), [&](Student& a, Student& b) {

        float va = getSubjectScore(a, subject);  // Lấy điểm môn học từ sinh viên a
        float vb = getSubjectScore(b, subject);  // Lấy điểm môn học từ sinh viên b
      
        return ascending ? va < vb : va > vb;
    });
   
    printAll();
    saveToFile();
}

void loadFromFile() {
    ifstream in("students.txt");
    // Kiểm tra nếu file không mở được
    if (!in.is_open()) {
        cout << "Khong the mo file de doc.\n";
        return;
    }
    students.clear();
    Student s;
    string line;
    while (getline(in, line)) {
        size_t pos = 0;
        vector<string> parts;
        while ((pos = line.find(",")) != string::npos) {
            parts.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }
        parts.push_back(line);
        if (parts.size() == 7) {
            s.id = parts[0]; s.name = parts[1]; s.dob = parts[2]; s.className = parts[3];
            s.math = stof(parts[4]); s.physics = stof(parts[5]); s.chemistry = stof(parts[6]);
            students.push_back(s);
        }else {
            cout << "Doc bi loi: " << line << "\n";
        }
    }
    in.close();
}

int main() {
    loadFromFile();
    int choice;
    do {
        cout << "\n1. Them sinh vien\n2. In danh sach\n3. Sua theo ID\n4. Xoa theo ID\n5. Tim diem cao/thap\n6. Thong ke\n7. Sap xep\n8. Luu va thoat\nChon: ";
        cin >> choice;
        switch (choice) {
            case 1: addStudent(); break;
            case 2: printAll(); break;
            case 3: editStudentByID(); break;
            case 4: deleteStudentByID(); break;
            case 5: {
                string mon; bool max;
                cout << "Nhap mon (math/physics/chemistry): "; cin >> mon;
                cout << "Tim diem cao nhat (1) hay thap nhat (0): "; cin >> max;
                findExtremum(mon, max);
                break;
            }
            case 6: {
                string mon;
                cout << "Nhap mon (math/physics/chemistry): "; cin >> mon;
                statistics(mon);
                break;
            }
            case 7: {
                string mon; bool asc;
                cout << "Nhap mon (math/physics/chemistry): "; cin >> mon;
                cout << "Tang dan (1) hay giam dan (0): "; cin >> asc;
                sortBySubject(mon, asc);
                break;
            }
            case 8: saveToFile(); cout << "Da luu va thoat.\n"; break;
        }
    } while (choice != 8);
    return 0;
}
