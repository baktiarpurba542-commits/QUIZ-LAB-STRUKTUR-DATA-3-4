#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>

using namespace std;

// ==================== STRUKTUR DATA ====================
struct Mahasiswa {
    string nim;
    string nama;
    float nilai;
    Mahasiswa* next;
};


string bacaBaris(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    size_t awal = s.find_first_not_of(" \t");
    if (awal == string::npos) return "";
    size_t akhir = s.find_last_not_of(" \t");
    return s.substr(awal, akhir - awal + 1);
}

bool semuaDigit(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit((unsigned char)c)) return false;
    }
    return true;
}

bool namaValid(const string& s) {
    if (s.empty()) return false;
    bool adaHuruf = false;
    for (char c : s) {
        unsigned char u = (unsigned char)c;
        if (isalpha(u)) adaHuruf = true;
        else if (c != ' ' && c != '.' && c != '\'') return false;
    }
    return adaHuruf;
}

bool nimSudahAda(Mahasiswa* head, const string& nim) {
    for (Mahasiswa* t = head; t; t = t->next) {
        if (t->nim == nim) return true;
    }
    return false;
}

int bacaPilihanMenu() {
    while (true) {
        string s = bacaBaris("Pilihan Anda [1-6]: ");
        if (semuaDigit(s) && s.size() <= 2) {
            int p = stoi(s);
            if (p >= 1 && p <= 6) return p;
        }
        cout << "[!] Pilihan tidak valid! Masukkan angka 1 sampai 6.\n";
    }
}

string bacaNIM(Mahasiswa* head) {
    while (true) {
        string nim = bacaBaris("Masukkan NIM   : ");
        if (!semuaDigit(nim)) {
            cout << "[!] NIM harus berupa angka saja dan tidak boleh kosong. Ulangi!\n";
        } else if (nimSudahAda(head, nim)) {
            cout << "[!] NIM sudah terdaftar. Masukkan NIM lain!\n";
        } else {
            return nim;
        }
    }
}

string bacaNIMCari() {
    while (true) {
        string nim = bacaBaris("\nMasukkan NIM yang dicari: ");
        if (semuaDigit(nim)) return nim;
        cout << "[!] NIM harus berupa angka saja. Ulangi!\n";
    }
}

string bacaNama() {
    while (true) {
        string nama = bacaBaris("Masukkan Nama  : ");
        if (namaValid(nama)) return nama;
        cout << "[!] Nama hanya boleh huruf dan spasi, tidak boleh kosong. Ulangi!\n";
    }
}

float bacaNilai(const string& prompt) {
    while (true) {
        string s = bacaBaris(prompt);
        for (char& c : s) if (c == ',') c = '.';
        try {
            size_t pos = 0;
            float v = stof(s, &pos);
            if (pos == s.size() && v >= 0.0f && v <= 100.0f) return v;
        } catch (...) {
        }
        cout << "[!] Nilai harus berupa angka antara 0 sampai 100. Ulangi!\n";
    }
}

// ==================== LINKED LIST ====================
void tambahMahasiswa(Mahasiswa*& head, const string& nim, const string& nama, float nilai) {
    Mahasiswa* newNode = new Mahasiswa{nim, nama, nilai, nullptr};
    if (!head) {
        head = newNode;
    } else {
        Mahasiswa* temp = head;
        while (temp->next) temp = temp->next;
        temp->next = newNode;
    }
    cout << "\n[Sukses] Data mahasiswa berhasil ditambahkan!\n";
}

void tampilkanMahasiswa(Mahasiswa* head) {
    if (!head) {
        cout << "\n[!] Belum ada data mahasiswa yang tersimpan.\n";
        return;
    }

    cout << "\n=============================================================\n";
    cout << "                   DAFTAR DATA MAHASISWA                     \n";
    cout << "=============================================================\n";
    cout << left << setw(5) << "No"
         << setw(15) << "NIM"
         << setw(25) << "Nama"
         << right << setw(10) << "Nilai" << endl;
    cout << "-------------------------------------------------------------\n";

    Mahasiswa* temp = head;
    int no = 1;
    while (temp) {
        cout << left << setw(5) << no++
             << setw(15) << temp->nim
             << setw(25) << temp->nama
             << right << setw(10) << fixed << setprecision(2) << temp->nilai << endl;
        temp = temp->next;
    }
    cout << "=============================================================\n";
}

void linearSearchNIM(Mahasiswa* head, const string& targetNIM) {
    Mahasiswa* temp = head;
    int posisi = 1;
    bool ditemukan = false;

    cout << "\n----------------- HASIL LINEAR SEARCH (NIM) -----------------\n";
    while (temp) {
        if (temp->nim == targetNIM) {
            cout << "[v] Mahasiswa DITEMUKAN pada posisi ke-" << posisi << "\n";
            cout << "    - NIM   : " << temp->nim << "\n";
            cout << "    - Nama  : " << temp->nama << "\n";
            cout << "    - Nilai : " << fixed << setprecision(2) << temp->nilai << "\n";
            ditemukan = true;
            break;
        }
        temp = temp->next;
        posisi++;
    }

    if (!ditemukan) {
        cout << "[x] Mahasiswa dengan NIM \"" << targetNIM << "\" tidak ditemukan.\n";
    }
    cout << "-------------------------------------------------------------\n";
}

int hitungJumlahData(Mahasiswa* head) {
    int count = 0;
    for (Mahasiswa* t = head; t; t = t->next) count++;
    return count;
}

void hapusSemua(Mahasiswa*& head) {
    while (head) {
        Mahasiswa* hapus = head;
        head = head->next;
        delete hapus;
    }
}

// ==================== DYNAMIC ARRAY, SORT, SEARCH ====================
float* salinKeDynamicArray(Mahasiswa* head, int size) {
    if (size <= 0) return nullptr;
    float* arr = new float[size];
    Mahasiswa* temp = head;
    for (int i = 0; temp && i < size; i++) {
        arr[i] = temp->nilai;
        temp = temp->next;
    }
    return arr;
}

void insertionSort(float arr[], int size) {
    for (int i = 1; i < size; i++) {
        float key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void tampilkanDynamicArray(float arr[], int size) {
    if (size <= 0) {
        cout << "\n[!] Array nilai kosong.\n";
        return;
    }
    cout << "\n----------------- HASIL INSERTION SORT --------------------\n";
    cout << "Nilai terurut (Kecil ke Besar): { ";
    for (int i = 0; i < size; i++) {
        cout << fixed << setprecision(2) << arr[i];
        if (i < size - 1) cout << ", ";
    }
    cout << " }\n";
    cout << "-------------------------------------------------------------\n";
}

void binarySearchNilai(float arr[], int size, float target) {
    int low = 0, high = size - 1;
    bool ditemukan = false;

    cout << "\n----------------- HASIL BINARY SEARCH (NILAI) ---------------\n";
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == target) {
            cout << "[v] Nilai " << fixed << setprecision(2) << target
                 << " DITEMUKAN pada indeks ke-" << mid << " (Array terurut).\n";
            ditemukan = true;
            break;
        }
        if (arr[mid] < target) low = mid + 1;
        else high = mid - 1;
    }

    if (!ditemukan) {
        cout << "[x] Nilai " << fixed << setprecision(2) << target
             << " tidak ditemukan dalam daftar nilai mahasiswa.\n";
    }
    cout << "-------------------------------------------------------------\n";
}

// ==================== MAIN ====================
int main() {
    Mahasiswa* head = nullptr;
    float* dynamicNilai = nullptr;
    int ukuranData = 0;
    bool sudahDiurutkan = false;
    int pilihan;

    do {
        cout << "\n=============================================================\n";
        cout << "            PROGRAM PENGELOLAAN NILAI MAHASISWA              \n";
        cout << "=============================================================\n";
        cout << " 1. Tambah Data Mahasiswa\n";
        cout << " 2. Tampilkan Daftar Mahasiswa\n";
        cout << " 3. Cari Mahasiswa Berdasarkan NIM \n";
        cout << " 4. Urutkan Nilai Mahasiswa \n";
        cout << " 5. Cari Nilai Tertentu \n";
        cout << " 6. Keluar\n";
        cout << "=============================================================\n";
        pilihan = bacaPilihanMenu();

        switch (pilihan) {
            case 1: {
                cout << "\n--- Input Data Mahasiswa Baru ---\n";
                string nim = bacaNIM(head);
                string nama = bacaNama();
                float nilai = bacaNilai("Masukkan Nilai : ");
                tambahMahasiswa(head, nim, nama, nilai);
                sudahDiurutkan = false;
                break;
            }
            case 2:
                tampilkanMahasiswa(head);
                break;
            case 3: {
                if (!head) {
                    cout << "\n[!] Data mahasiswa masih kosong.\n";
                    break;
                }
                string targetNIM = bacaNIMCari();
                linearSearchNIM(head, targetNIM);
                break;
            }
            case 4: {
                ukuranData = hitungJumlahData(head);
                if (ukuranData == 0) {
                    cout << "\n[!] Belum ada data untuk diurutkan.\n";
                    break;
                }
                delete[] dynamicNilai;
                dynamicNilai = salinKeDynamicArray(head, ukuranData);
                insertionSort(dynamicNilai, ukuranData);
                sudahDiurutkan = true;
                tampilkanDynamicArray(dynamicNilai, ukuranData);
                break;
            }
            case 5: {
                if (!sudahDiurutkan) {
                    cout << "\n[!] Harap urutkan nilai terlebih dahulu (Pilih menu 4) sebelum Binary Search!\n";
                    break;
                }
                float targetNilai = bacaNilai("\nMasukkan Nilai yang ingin dicari: ");
                binarySearchNilai(dynamicNilai, ukuranData, targetNilai);
                break;
            }
            case 6:
                cout << "\nTerima kasih telah menggunakan program ini. Program dihentikan.\n";
                break;
        }
    } while (pilihan != 6);

    delete[] dynamicNilai;
    hapusSemua(head);
    return 0;
}