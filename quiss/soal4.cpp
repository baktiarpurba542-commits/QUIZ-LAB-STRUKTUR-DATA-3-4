#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>

using namespace std;

const int TABLE_SIZE = 20;

// ==================== FUNGSI VALIDASI INPUT ====================
// Input dibaca per baris (getline) supaya tidak error saat user
// mengetik huruf di tempat angka.

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

int bacaPilihanMenu() {
    while (true) {
        string s = bacaBaris("Pilihan Anda [1-6]: ");
        if (s.size() == 1 && s[0] >= '1' && s[0] <= '6') return s[0] - '0';
        cout << "[!] Pilihan tidak valid! Masukkan angka 1 sampai 6.\n";
    }
}

long long bacaNIM(const string& prompt) {
    while (true) {
        string s = bacaBaris(prompt);
        if (semuaDigit(s) && s.size() <= 15) return stoll(s);
        cout << "[!] NIM harus berupa angka saja (maksimal 15 digit) dan tidak boleh kosong. Ulangi!\n";
    }
}

string bacaNama(const string& prompt) {
    while (true) {
        string nama = bacaBaris(prompt);
        if (namaValid(nama)) return nama;
        cout << "[!] Nama hanya boleh huruf dan spasi, tidak boleh kosong. Ulangi!\n";
    }
}

// ==================== TABEL DATA MAHASISWA ====================
struct Mahasiswa {
    long long nim;
    string nama;
    bool isOccupied; // slot sedang berisi data
    bool isDeleted;  // slot bekas data yang dihapus
};

class HashTable {
private:
    Mahasiswa table[TABLE_SIZE];

    // Hash 1: key % 20
    int hash1(long long key) {
        return key % TABLE_SIZE;
    }

    // Hash 2: 19 - (key % 19)
    int hash2(long long key) {
        return 19 - (key % 19);
    }

    // Mengembalikan indeks data dengan NIM tersebut, atau -1 jika tidak ada
    int cariIndeks(long long nim) {
        int h1 = hash1(nim);
        int h2 = hash2(nim);

        for (int i = 0; i < TABLE_SIZE; i++) {
            int index = (h1 + i * h2) % TABLE_SIZE;

            // Slot yang belum pernah terisi: data pasti tidak ada
            if (!table[index].isOccupied && !table[index].isDeleted) return -1;

            if (table[index].isOccupied && table[index].nim == nim) return index;
        }
        return -1;
    }

public:
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i].nim = -1;
            table[i].nama = "";
            table[i].isOccupied = false;
            table[i].isDeleted = false;
        }
    }

    // 1. Tambah data
    void insert(long long nim, const string& nama) {
        // Cek NIM sudah ada atau belum (termasuk di belakang slot bekas hapus)
        if (cariIndeks(nim) != -1) {
            cout << "[!] NIM " << nim << " sudah terdaftar di dalam tabel.\n";
            return;
        }

        int h1 = hash1(nim);
        int h2 = hash2(nim);

        for (int i = 0; i < TABLE_SIZE; i++) {
            int index = (h1 + i * h2) % TABLE_SIZE;

            if (i == 0 && table[index].isOccupied) {
                cout << "[Info] Indeks " << index << " (NIM " << nim
                     << ") sudah terisi. Mencari indeks lain...\n";
            }

            // Slot kosong atau bekas terhapus bisa dipakai
            if (!table[index].isOccupied) {
                table[index].nim = nim;
                table[index].nama = nama;
                table[index].isOccupied = true;
                table[index].isDeleted = false;
                cout << "[Sukses] NIM " << nim << " (" << nama << ") dimasukkan pada indeks ke-" << index << "\n";
                return;
            }
        }
        cout << "[Gagal] Tabel penuh! Tidak dapat memasukkan NIM " << nim << ".\n";
    }

    // 2. Cari data berdasarkan NIM
    void search(long long nim) {
        int index = cariIndeks(nim);
        if (index != -1) {
            cout << "\n[v] Data DITEMUKAN!\n";
            cout << "    - Indeks Tabel : " << index << "\n";
            cout << "    - NIM          : " << table[index].nim << "\n";
            cout << "    - Nama         : " << table[index].nama << "\n";
        } else {
            cout << "\n[x] Mahasiswa dengan NIM " << nim << " tidak ditemukan.\n";
        }
    }

    // 3. Hapus data berdasarkan NIM
    void remove(long long nim) {
        int index = cariIndeks(nim);
        if (index != -1) {
            table[index].isDeleted = true;
            table[index].isOccupied = false;
            cout << "\n[Sukses] Data dengan NIM " << nim << " berhasil dihapus dari indeks ke-" << index << ".\n";
        } else {
            cout << "\n[x] Gagal menghapus: NIM " << nim << " tidak ditemukan.\n";
        }
    }

    // 4. Tampilkan seluruh isi tabel
    void display() {
        cout << "\n=============================================================\n";
        cout << "                   DATA MAHASISWA TERSIMPAN                  \n";
        cout << "=============================================================\n";
        cout << left << setw(8) << "Indeks"
             << setw(15) << "NIM"
             << setw(25) << "Nama"
             << right << setw(15) << "Status" << endl;
        cout << "-------------------------------------------------------------\n";

        for (int i = 0; i < TABLE_SIZE; i++) {
            cout << left << setw(8) << i;
            if (table[i].isOccupied) {
                cout << left << setw(15) << table[i].nim
                     << setw(25) << table[i].nama
                     << right << setw(15) << "Terisi";
            } else if (table[i].isDeleted) {
                cout << left << setw(15) << "-"
                     << setw(25) << "-"
                     << right << setw(15) << "Telah Dihapus";
            } else {
                cout << left << setw(15) << "-"
                     << setw(25) << "-"
                     << right << setw(15) << "Kosong";
            }
            cout << "\n";
        }
        cout << "=============================================================\n";
    }

    // Pengujian otomatis 15 data (termasuk NIM yang indeksnya bentrok & penghapusan)
    void jalankanPengujianOtomatis() {
        cout << "\n=============================================================\n";
        cout << "         MENJALANKAN PENGUJIAN OTOMATIS (15+ DATA)           \n";
        cout << "=============================================================\n";

        insert(241501, "Budi Santoso");
        insert(241521, "Ani Lestari");     // indeks awal sama dengan 241501 (241521 % 20 == 241501 % 20 == 1)
        insert(241541, "Cokro Wijaya");    // indeks awal sama lagi
        insert(241502, "Dita Permata");
        insert(241503, "Eko Prasetyo");
        insert(241523, "Fira Azahra");     // indeks awal sama dengan 241503
        insert(241504, "Gilang Ramadhan");
        insert(241505, "Hana Salsabila");
        insert(241506, "Irfan Hakim");
        insert(241507, "Joko Widodo");
        insert(241527, "Kirana Larasati"); // indeks awal sama dengan 241507
        insert(241508, "Lukman Hakim");
        insert(241509, "Maya Sari");
        insert(241510, "Nova Arifin");
        insert(241530, "Oki Setiawan");    // indeks awal sama dengan 241510

        cout << "\n--- Tampilan Tabel Setelah 15 Data Masuk ---";
        display();

        cout << "\n--- Pengujian Pencarian (NIM 241521 & 241599) ---";
        search(241521);
        search(241599);

        cout << "\n--- Pengujian Penghapusan (NIM 241521) ---";
        remove(241521);

        cout << "\n--- Tampilan Tabel Setelah Penghapusan ---";
        display();
    }
};

// ==================== MENU UTAMA ====================
int main() {
    HashTable ht;
    int pilihan;

    do {
        cout << "\n=============================================================\n";
        cout << "           SISTEM PENGELOLAAN DATA MAHASISWA                 \n";
        cout << "=============================================================\n";
        cout << " 1. Jalankan Pengujian Otomatis (15+ Data & Penghapusan)\n";
        cout << " 2. Tambah Data Mahasiswa Baru\n";
        cout << " 3. Cari Mahasiswa Berdasarkan NIM\n";
        cout << " 4. Hapus Data Mahasiswa\n";
        cout << " 5. Tampilkan Seluruh Data\n";
        cout << " 6. Keluar\n";
        cout << "=============================================================\n";
        pilihan = bacaPilihanMenu();

        switch (pilihan) {
            case 1:
                ht.jalankanPengujianOtomatis();
                break;
            case 2: {
                cout << "\n--- Tambah Data Baru ---\n";
                long long nim = bacaNIM("Masukkan NIM  : ");
                string nama = bacaNama("Masukkan Nama : ");
                ht.insert(nim, nama);
                break;
            }
            case 3: {
                cout << "\n--- Cari Mahasiswa ---\n";
                long long nim = bacaNIM("Masukkan NIM yang dicari: ");
                ht.search(nim);
                break;
            }
            case 4: {
                cout << "\n--- Hapus Data Mahasiswa ---\n";
                long long nim = bacaNIM("Masukkan NIM yang akan dihapus: ");
                ht.remove(nim);
                break;
            }
            case 5:
                ht.display();
                break;
            case 6:
                cout << "\nTerima kasih telah menggunakan program ini.\n";
                break;
        }
    } while (pilihan != 6);

    return 0;
}