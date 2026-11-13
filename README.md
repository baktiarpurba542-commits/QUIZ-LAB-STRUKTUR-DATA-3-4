# Quiz Lab Struktur Data 3&4

## Identitas
- **Nama:** Baktiar Lammartua Purba
- **NIM:** 251401041
- **Lab:** [3]
- **Program Studi:** S-1 Ilmu Komputer, Universitas Sumatera Utara

## Video Penjelasan
Video penjelasan keempat program dan contoh hasil eksekusinya:
https://youtu.be/aZZK873s-No?si=RauIJFl19Bt_y16s

## Deskripsi Program

### 1. Sistem Pengelolaan Nilai Mahasiswa (`soal1.cpp`)
Program mengelola data mahasiswa (NIM, Nama, Nilai).
- **Single Linked List** menyimpan data mahasiswa (tambah di akhir list, tampilkan, hapus semua node di akhir program).
- **Dynamic Array** menyimpan salinan nilai mahasiswa yang dibuat saat menu pengurutan dipilih.
- **Insertion Sort** mengurutkan nilai dari terkecil hingga terbesar.
- **Linear Search** mencari mahasiswa berdasarkan NIM.
- **Binary Search** mencari nilai tertentu, hanya bisa dipakai setelah nilai diurutkan.
- Validasi input: NIM harus angka dan tidak boleh duplikat, nama hanya berisi huruf, nilai harus 0-100 (koma desimal diterima).

Menu: 1 Tambah data, 2 Tampilkan data, 3 Cari NIM (Linear Search), 4 Urutkan nilai (Insertion Sort), 5 Cari nilai (Binary Search), 6 Keluar.

### 2. Sistem Antrean dan Pengelolaan Buku Perpustakaan (`soal2.cpp`)
Program membantu pengelolaan peminjaman dan pengembalian buku.
- **Queue** (FIFO) mengatur antrean mahasiswa yang ingin meminjam buku, dengan pointer `front` dan `rear`.
- **Stack** (LIFO) menyimpan riwayat pengembalian buku, sehingga yang terbaru tampil paling atas.
- **Binary Search Tree** menyimpan ID buku, dengan pencarian ID serta traversal **Pre-Order, In-Order, dan Post-Order**.
- Validasi input: ID buku harus angka positif dan tidak duplikat, tanggal dicek format dan kebenarannya (termasuk tahun kabisat).

Menu: 1 Tambah buku, 2 Cari buku, 3 Tampilkan katalog (3 traversal), 4 Tambah antrean, 5 Proses antrean, 6 Tampilkan antrean, 7 Catat pengembalian, 8 Tampilkan riwayat, 9 Keluar.

### 3. Sistem Pencarian Rute di Lingkungan Kampus (`soal3.cpp`)
Program memodelkan 6 lokasi kampus (A-F) sebagai **Graph tidak berarah berbobot** dengan **Adjacency Matrix**.
- Sepuluh sisi: A-B=4, A-C=2, B-C=1, B-D=5, B-E=3, C-D=8, C-E=6, D-E=2, D-F=4, E-F=3.
- **BFS** dan **DFS** menelusuri graf dari lokasi awal pilihan pengguna.
- **Dijkstra** mencari jarak dan rute terpendek dari lokasi awal ke tujuan. Contoh A ke F: jarak 9 lewat A -> C -> B -> E -> F.

Menu: 1 Tampilkan matriks, 2 BFS, 3 DFS, 4 Rute terpendek (Dijkstra), 5 Keluar.

### 4. Hash Table Data Mahasiswa (`soal4.cpp`)
Program menyimpan data mahasiswa (NIM sebagai key, Nama sebagai value) di hash table berukuran 20 slot.
- **Open addressing** dengan **Double Hashing**:
  - `hash1(key) = key % 20`
  - `hash2(key) = 19 - (key % 19)`
  - indeks percobaan ke-i = `(hash1 + i * hash2) % 20`
- Operasi: **insert, search, remove, display**.
- Penghapusan memakai penanda *tombstone* (slot "terhapus") supaya jalur pencarian data lain tidak terputus.
- Tersedia menu pengujian otomatis: memasukkan 15 data (termasuk banyak collision), mencari NIM, menghapus data, lalu menampilkan tabel.

Menu: 1 Pengujian otomatis, 2 Tambah data, 3 Cari data, 4 Hapus data, 5 Tampilkan semua, 6 Keluar.

## Struktur Folder
```
quiz-lab-strukdat/
├── soal1.cpp
├── soal2.cpp
├── soal3.cpp
├── soal4.cpp
└── README.md
```

## Cara Menjalankan
Pastikan compiler g++ sudah terpasang. Buka terminal di folder proyek, lalu:

```
g++ -std=c++17 soal1.cpp -o soal1
./soal1
```

Ulangi untuk program lain dengan mengganti nama file:

```
g++ -std=c++17 soal2.cpp -o soal2
./soal2

g++ -std=c++17 soal3.cpp -o soal3
./soal3

g++ -std=c++17 soal4.cpp -o soal4
./soal4
```

Di Windows, jalankan hasil kompilasi dengan `soal1.exe` (bukan `./soal1`).

## Catatan
Seluruh program dibuat dengan bahasa C++ dan dikerjakan secara individu.
