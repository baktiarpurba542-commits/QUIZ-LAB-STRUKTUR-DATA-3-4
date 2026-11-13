#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <iomanip>
#include <climits>
#include <algorithm>
#include <cctype>

using namespace std;

const int V = 6; // Jumlah lokasi: A, B, C, D, E, F
const int INF = INT_MAX;

int charToInt(char c) {
    return toupper((unsigned char)c) - 'A';
}

char intToChar(int i) {
    return 'A' + i;
}

// ==================== FUNGSI VALIDASI INPUT ====================
// Input dibaca per baris (getline) supaya tidak error saat user
// mengetik karakter yang tidak sesuai.

string bacaBaris(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    size_t awal = s.find_first_not_of(" \t");
    if (awal == string::npos) return "";
    size_t akhir = s.find_last_not_of(" \t");
    return s.substr(awal, akhir - awal + 1);
}

int bacaPilihanMenu() {
    while (true) {
        string s = bacaBaris("Pilihan Anda [1-5]: ");
        if (s.size() == 1 && s[0] >= '1' && s[0] <= '5') return s[0] - '0';
        cout << "[!] Pilihan tidak valid! Masukkan angka 1 sampai 5.\n";
    }
}

// Mengembalikan huruf lokasi (huruf besar) A-F
char bacaLokasi(const string& prompt) {
    while (true) {
        string s = bacaBaris(prompt);
        if (s.size() == 1 && isalpha((unsigned char)s[0])) {
            int idx = charToInt(s[0]);
            if (idx >= 0 && idx < V) return intToChar(idx);
        }
        cout << "[!] Lokasi tidak valid! Masukkan satu huruf antara A sampai F. Ulangi!\n";
    }
}

// ==================== DATA GRAF ====================
void tambahSisi(int m[V][V], char a, char b, int bobot) {
    m[charToInt(a)][charToInt(b)] = bobot;
    m[charToInt(b)][charToInt(a)] = bobot;
}

void inisialisasiGraf(int adjMatrix[V][V]) {
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            adjMatrix[i][j] = 0;

    tambahSisi(adjMatrix, 'A', 'B', 4);
    tambahSisi(adjMatrix, 'A', 'C', 2);
    tambahSisi(adjMatrix, 'B', 'C', 1);
    tambahSisi(adjMatrix, 'B', 'D', 5);
    tambahSisi(adjMatrix, 'B', 'E', 3);
    tambahSisi(adjMatrix, 'C', 'D', 8);
    tambahSisi(adjMatrix, 'C', 'E', 6);
    tambahSisi(adjMatrix, 'D', 'E', 2);
    tambahSisi(adjMatrix, 'D', 'F', 4);
    tambahSisi(adjMatrix, 'E', 'F', 3);
}

void tampilkanMatriks(int adjMatrix[V][V]) {
    cout << "\n=============================================================\n";
    cout << "                 TABEL JARAK ANTAR LOKASI                    \n";
    cout << "=============================================================\n";
    cout << setw(6) << " ";
    for (int i = 0; i < V; i++) cout << setw(6) << intToChar(i);
    cout << "\n-------------------------------------------------------------\n";

    for (int i = 0; i < V; i++) {
        cout << setw(4) << intToChar(i) << " |";
        for (int j = 0; j < V; j++) {
            cout << setw(6) << adjMatrix[i][j];
        }
        cout << "\n";
    }
    cout << "=============================================================\n";
}

// ==================== PENELUSURAN ====================
void bfs(int adjMatrix[V][V], char startChar) {
    int start = charToInt(startChar);
    vector<bool> visited(V, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    cout << "\n------------ HASIL PENELUSURAN (TERDEKAT DAHULU) ------------\n";
    cout << "Mulai dari Lokasi " << startChar << " : ";

    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        cout << intToChar(curr) << " ";

        for (int i = 0; i < V; i++) {
            if (adjMatrix[curr][i] > 0 && !visited[i]) {
                visited[i] = true;
                q.push(i);
            }
        }
    }
    cout << "\n-------------------------------------------------------------\n";
}

void dfsUtil(int adjMatrix[V][V], int curr, vector<bool>& visited) {
    visited[curr] = true;
    cout << intToChar(curr) << " ";

    for (int i = 0; i < V; i++) {
        if (adjMatrix[curr][i] > 0 && !visited[i]) {
            dfsUtil(adjMatrix, i, visited);
        }
    }
}

void dfs(int adjMatrix[V][V], char startChar) {
    int start = charToInt(startChar);
    vector<bool> visited(V, false);
    cout << "\n------------ HASIL PENELUSURAN (TERDALAM DAHULU) ------------\n";
    cout << "Mulai dari Lokasi " << startChar << " : ";
    dfsUtil(adjMatrix, start, visited);
    cout << "\n-------------------------------------------------------------\n";
}

// ==================== JARAK TERPENDEK ====================
void dijkstra(int adjMatrix[V][V], char startChar, char endChar) {
    int start = charToInt(startChar);
    int end = charToInt(endChar);

    vector<int> dist(V, INF);
    vector<int> parent(V, -1);
    vector<bool> sptSet(V, false);

    dist[start] = 0;

    for (int count = 0; count < V; count++) {
        int minDistance = INF, u = -1;

        for (int i = 0; i < V; i++) {
            if (!sptSet[i] && dist[i] < minDistance) {
                minDistance = dist[i];
                u = i;
            }
        }

        if (u == -1) break;
        sptSet[u] = true;

        for (int v = 0; v < V; v++) {
            if (!sptSet[v] && adjMatrix[u][v] && dist[u] != INF &&
                dist[u] + adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + adjMatrix[u][v];
                parent[v] = u;
            }
        }
    }

    cout << "\n------------------- HASIL RUTE TERPENDEK --------------------\n";
    cout << "Dari Lokasi " << startChar << " ke Lokasi " << endChar << " :\n";

    if (dist[end] == INF) {
        cout << "[x] Tidak ada jalur yang tersedia menuju lokasi " << endChar << ".\n";
    } else {
        cout << "Total Jarak Min : " << dist[end] << "\n";

        vector<char> path;
        int curr = end;
        while (curr != -1) {
            path.push_back(intToChar(curr));
            curr = parent[curr];
        }
        reverse(path.begin(), path.end());

        cout << "Rute Dilalui    : ";
        for (size_t i = 0; i < path.size(); i++) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << "\n";
    }
    cout << "-------------------------------------------------------------\n";
}

// ==================== MENU UTAMA ====================
int main() {
    int adjMatrix[V][V];
    inisialisasiGraf(adjMatrix);

    int pilihan;
    do {
        cout << "\n=============================================================\n";
        cout << "         SISTEM PENCARIAN RUTE DI LINGKUNGAN KAMPUS          \n";
        cout << "=============================================================\n";
        cout << " 1. Tampilkan Tabel Jarak Antar Lokasi\n";
        cout << " 2. Telusuri Lokasi (Terdekat Dahulu)\n";
        cout << " 3. Telusuri Lokasi (Terdalam Dahulu)\n";
        cout << " 4. Cari Jarak Terpendek & Rute\n";
        cout << " 5. Keluar\n";
        cout << "=============================================================\n";
        pilihan = bacaPilihanMenu();

        switch (pilihan) {
            case 1:
                tampilkanMatriks(adjMatrix);
                break;
            case 2: {
                char start = bacaLokasi("Masukkan Lokasi Awal (A-F): ");
                bfs(adjMatrix, start);
                break;
            }
            case 3: {
                char start = bacaLokasi("Masukkan Lokasi Awal (A-F): ");
                dfs(adjMatrix, start);
                break;
            }
            case 4: {
                char start = bacaLokasi("Masukkan Lokasi Awal (A-F)  : ");
                char end = bacaLokasi("Masukkan Lokasi Tujuan (A-F): ");
                dijkstra(adjMatrix, start, end);
                break;
            }
            case 5:
                cout << "\nTerima kasih telah menggunakan sistem pencarian rute kampus.\n";
                break;
        }
    } while (pilihan != 5);

    return 0;
}