#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>

using namespace std;

// ==================== FUNGSI VALIDASI INPUT ====================
// Semua input dibaca per baris (getline) supaya tidak error saat user
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

// Format tanggal: DD-MM-YYYY atau DD/MM/YYYY (dicek hari, bulan, kabisat)
bool tanggalValid(const string& s) {
    if (s.size() != 10) return false;
    if ((s[2] != '-' && s[2] != '/') || s[5] != s[2]) return false;
    string d = s.substr(0, 2), m = s.substr(3, 2), y = s.substr(6, 4);
    if (!semuaDigit(d) || !semuaDigit(m) || !semuaDigit(y)) return false;
    int hari = stoi(d), bulan = stoi(m), tahun = stoi(y);
    if (tahun < 1900 || tahun > 2100 || bulan < 1 || bulan > 12) return false;
    int hariPerBulan[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool kabisat = (tahun % 4 == 0 && tahun % 100 != 0) || (tahun % 400 == 0);
    int maks = hariPerBulan[bulan - 1];
    if (bulan == 2 && kabisat) maks = 29;
    return hari >= 1 && hari <= maks;
}

int bacaPilihanMenu() {
    while (true) {
        string s = bacaBaris("Pilihan Anda [1-9]: ");
        if (semuaDigit(s) && s.size() == 1) {
            int p = stoi(s);
            if (p >= 1 && p <= 9) return p;
        }
        cout << "[!] Pilihan tidak valid! Masukkan angka 1 sampai 9.\n";
    }
}

int bacaIDBuku(const string& prompt) {
    while (true) {
        string s = bacaBaris(prompt);
        if (semuaDigit(s) && s.size() <= 9) {
            int id = stoi(s);
            if (id > 0) return id;
        }
        cout << "[!] ID Buku harus berupa angka positif (maksimal 9 digit). Ulangi!\n";
    }
}

string bacaNama() {
    while (true) {
        string nama = bacaBaris("Masukkan Nama Mahasiswa : ");
        if (namaValid(nama)) return nama;
        cout << "[!] Nama hanya boleh huruf dan spasi, tidak boleh kosong. Ulangi!\n";
    }
}

string bacaJudul() {
    while (true) {
        string judul = bacaBaris("Masukkan Judul Buku      : ");
        if (!judul.empty()) return judul;
        cout << "[!] Judul buku tidak boleh kosong. Ulangi!\n";
    }
}

string bacaTanggal() {
    while (true) {
        string t = bacaBaris("Masukkan Tanggal Kembali (DD-MM-YYYY): ");
        if (tanggalValid(t)) return t;
        cout << "[!] Tanggal tidak valid! Gunakan format DD-MM-YYYY (contoh: 14-11-2026). Ulangi!\n";
    }
}

// ==================== ANTREAN PEMINJAMAN ====================
struct QueueNode {
    string namaMahasiswa;
    int idBuku;
    QueueNode* next;
};

struct Queue {
    QueueNode* front = nullptr;
    QueueNode* rear = nullptr;
};

void enqueue(Queue& q, const string& nama, int idBuku) {
    QueueNode* newNode = new QueueNode{nama, idBuku, nullptr};
    if (!q.rear) {
        q.front = q.rear = newNode;
    } else {
        q.rear->next = newNode;
        q.rear = newNode;
    }
    cout << "\n[Sukses] Mahasiswa \"" << nama << "\" berhasil masuk antrean peminjaman buku ID: " << idBuku << "\n";
}

void dequeue(Queue& q) {
    if (!q.front) {
        cout << "\n[!] Antrean peminjaman kosong.\n";
        return;
    }
    QueueNode* temp = q.front;
    cout << "\n[Proses] Memproses peminjaman:\n";
    cout << "         - Nama   : " << temp->namaMahasiswa << "\n";
    cout << "         - ID Buku: " << temp->idBuku << "\n";

    q.front = q.front->next;
    if (!q.front) q.rear = nullptr;
    delete temp;
    cout << "[Sukses] Antrean terdepan telah diproses!\n";
}

void tampilkanQueue(const Queue& q) {
    if (!q.front) {
        cout << "\n[!] Antrean peminjaman saat ini kosong.\n";
        return;
    }
    cout << "\n=============================================================\n";
    cout << "                 ANTREAN PEMINJAMAN BUKU                     \n";
    cout << "=============================================================\n";
    cout << left << setw(5) << "No"
         << setw(25) << "Nama Mahasiswa"
         << right << setw(15) << "ID Buku" << endl;
    cout << "-------------------------------------------------------------\n";

    QueueNode* temp = q.front;
    int no = 1;
    while (temp) {
        cout << left << setw(5) << no++
             << setw(25) << temp->namaMahasiswa
             << right << setw(15) << temp->idBuku << endl;
        temp = temp->next;
    }
    cout << "=============================================================\n";
}

void hapusQueue(Queue& q) {
    while (q.front) {
        QueueNode* hapus = q.front;
        q.front = q.front->next;
        delete hapus;
    }
    q.rear = nullptr;
}

// ==================== RIWAYAT PENGEMBALIAN ====================
struct StackNode {
    string namaMahasiswa;
    int idBuku;
    string tanggalKembali;
    StackNode* next;
};

void pushStack(StackNode*& top, const string& nama, int idBuku, const string& tanggal) {
    top = new StackNode{nama, idBuku, tanggal, top};
    cout << "\n[Sukses] Pengembalian buku ID " << idBuku << " oleh " << nama << " dicatat ke riwayat.\n";
}

void tampilkanStack(StackNode* top) {
    if (!top) {
        cout << "\n[!] Belum ada riwayat pengembalian buku.\n";
        return;
    }
    cout << "\n=============================================================\n";
    cout << "               RIWAYAT PENGEMBALIAN BUKU                     \n";
    cout << "=============================================================\n";
    cout << left << setw(5) << "No"
         << setw(20) << "Nama Mahasiswa"
         << setw(10) << "ID Buku"
         << right << setw(20) << "Tanggal Kembali" << endl;
    cout << "-------------------------------------------------------------\n";

    StackNode* temp = top;
    int no = 1;
    while (temp) {
        cout << left << setw(5) << no++
             << setw(20) << temp->namaMahasiswa
             << setw(10) << temp->idBuku
             << right << setw(20) << temp->tanggalKembali << endl;
        temp = temp->next;
    }
    cout << "=============================================================\n";
}

void hapusStack(StackNode*& top) {
    while (top) {
        StackNode* hapus = top;
        top = top->next;
        delete hapus;
    }
}

// ==================== KATALOG BUKU ====================
struct BSTNode {
    int idBuku;
    string judulBuku;
    BSTNode* left;
    BSTNode* right;
};

BSTNode* searchBST(BSTNode* root, int id) {
    if (!root || root->idBuku == id) return root;
    if (id < root->idBuku) return searchBST(root->left, id);
    return searchBST(root->right, id);
}

// Mengembalikan true jika berhasil ditambahkan, false jika ID sudah ada
bool insertBST(BSTNode*& root, int id, const string& judul) {
    if (!root) {
        root = new BSTNode{id, judul, nullptr, nullptr};
        return true;
    }
    if (id < root->idBuku) return insertBST(root->left, id, judul);
    if (id > root->idBuku) return insertBST(root->right, id, judul);
    return false;
}

void preOrder(BSTNode* root) {
    if (root) {
        cout << "[" << root->idBuku << " : " << root->judulBuku << "] ";
        preOrder(root->left);
        preOrder(root->right);
    }
}

void inOrder(BSTNode* root) {
    if (root) {
        inOrder(root->left);
        cout << "[" << root->idBuku << " : " << root->judulBuku << "] ";
        inOrder(root->right);
    }
}

void postOrder(BSTNode* root) {
    if (root) {
        postOrder(root->left);
        postOrder(root->right);
        cout << "[" << root->idBuku << " : " << root->judulBuku << "] ";
    }
}

void hapusBST(BSTNode*& root) {
    if (root) {
        hapusBST(root->left);
        hapusBST(root->right);
        delete root;
        root = nullptr;
    }
}

// ==================== MENU UTAMA ====================
int main() {
    Queue antreanPeminjaman;
    StackNode* riwayatPengembalian = nullptr;
    BSTNode* katalogBuku = nullptr;
    int pilihan;

    do {
        cout << "\n=============================================================\n";
        cout << "                 SISTEM PERPUSTAKAAN KAMPUS                  \n";
        cout << "=============================================================\n";
        cout << " --- KATALOG BUKU --- \n";
        cout << " 1. Tambah Buku ke Katalog\n";
        cout << " 2. Cari Buku Berdasarkan ID\n";
        cout << " 3. Tampilkan Katalog Buku\n";
        cout << " --- PEMINJAMAN BUKU --- \n";
        cout << " 4. Tambah Antrean Peminjaman Buku\n";
        cout << " 5. Proses Antrean Peminjaman Terdepan\n";
        cout << " 6. Tampilkan Antrean Peminjaman\n";
        cout << " --- PENGEMBALIAN BUKU --- \n";
        cout << " 7. Catat Pengembalian Buku\n";
        cout << " 8. Tampilkan Riwayat Pengembalian\n";
        cout << " 9. Keluar\n";
        cout << "=============================================================\n";
        pilihan = bacaPilihanMenu();

        switch (pilihan) {
            case 1: {
                cout << "\n--- Tambah Buku Baru --- \n";
                int id = bacaIDBuku("Masukkan ID Buku (Angka) : ");
                if (searchBST(katalogBuku, id)) {
                    cout << "[!] ID Buku " << id << " sudah terdaftar di dalam sistem.\n";
                    break;
                }
                string judul = bacaJudul();
                insertBST(katalogBuku, id, judul);
                cout << "[Sukses] Buku berhasil ditambahkan ke katalog!\n";
                break;
            }
            case 2: {
                cout << "\n--- Pencarian Buku --- \n";
                int idCari = bacaIDBuku("Masukkan ID Buku yang dicari: ");
                BSTNode* hasil = searchBST(katalogBuku, idCari);
                if (hasil) {
                    cout << "[v] Buku DITEMUKAN!\n";
                    cout << "    - ID Buku    : " << hasil->idBuku << "\n";
                    cout << "    - Judul Buku : " << hasil->judulBuku << "\n";
                } else {
                    cout << "[x] Buku dengan ID " << idCari << " tidak ditemukan di katalog.\n";
                }
                break;
            }
            case 3:
                if (!katalogBuku) {
                    cout << "\n[!] Katalog buku masih kosong.\n";
                } else {
                    cout << "\n=============================================================\n";
                    cout << "                      KATALOG BUKU                           \n";
                    cout << "=============================================================\n";
                    cout << "Urutan Awal (induk dulu)    : "; preOrder(katalogBuku); cout << "\n";
                    cout << "Urutan Tengah (ID terurut)  : "; inOrder(katalogBuku); cout << "\n";
                    cout << "Urutan Akhir (induk akhir)  : "; postOrder(katalogBuku); cout << "\n";
                    cout << "=============================================================\n";
                }
                break;
            case 4: {
                cout << "\n--- Tambah Antrean Peminjaman --- \n";
                if (!katalogBuku) {
                    cout << "[!] Katalog buku masih kosong. Tambahkan buku terlebih dahulu (menu 1).\n";
                    break;
                }
                string nama = bacaNama();
                int id;
                while (true) {
                    id = bacaIDBuku("Masukkan ID Buku        : ");
                    if (searchBST(katalogBuku, id)) break;
                    cout << "[!] ID Buku " << id << " tidak ada di katalog. Ulangi!\n";
                }
                enqueue(antreanPeminjaman, nama, id);
                break;
            }
            case 5:
                dequeue(antreanPeminjaman);
                break;
            case 6:
                tampilkanQueue(antreanPeminjaman);
                break;
            case 7: {
                cout << "\n--- Catat Pengembalian Buku --- \n";
                string nama = bacaNama();
                int id = bacaIDBuku("Masukkan ID Buku        : ");
                string tanggal = bacaTanggal();
                pushStack(riwayatPengembalian, nama, id, tanggal);
                break;
            }
            case 8:
                tampilkanStack(riwayatPengembalian);
                break;
            case 9:
                cout << "\nTerima kasih telah menggunakan sistem perpustakaan ini.\n";
                break;
        }
    } while (pilihan != 9);

    hapusQueue(antreanPeminjaman);
    hapusStack(riwayatPengembalian);
    hapusBST(katalogBuku);
    return 0;
}