#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Pengguna {
    string username;
    string password;
    string peran;
};

struct Barang {
    string nama;
    double harga;
    int jumlah;
};

string toLowerCase(const string &str) {
    string hasil = str;
    for (char &c : hasil) {
        c = tolower(c);
    }
    return hasil;
}

string ubahSpasiKeUnderscore(const string &str) {
    string hasil = str;
    for (char &c : hasil) {
        if (c == ' ') {
            c = '_';
        }
    }
    return hasil;
}

string ubahUnderscoreKeSpasi(const string &str) {
    string hasil = str;
    for (char &c : hasil) {
        if (c == '_') {
            c = ' ';
        }
    }
    return hasil;
}

bool muatAkun(Pengguna akun[], int &jumlahAkun, int kapasitasMaksimal) {
    ifstream file("akun.txt");
    if (!file) {
        cout << "File akun tidak ditemukan.\n";
        return false;
    }

    jumlahAkun = 0;
    while (file >> akun[jumlahAkun].username >> akun[jumlahAkun].password >> akun[jumlahAkun].peran) {
        jumlahAkun++;
        if (jumlahAkun >= kapasitasMaksimal) {
            cout << "Database akun penuh. Beberapa data tidak dimuat.\n";
            break;
        }
    }
    file.close();
    return true;
}

void simpanAkun(const Pengguna &akun) {
    ofstream file("akun.txt", ios::app);
    if (!file) {
        cout << "Gagal menyimpan akun.\n";
        return;
    }
    file << akun.username << " " << akun.password << " " << akun.peran << "\n";
    file.close();
}

void registrasiAkun(Pengguna akun[], int &jumlahAkun, int kapasitasMaksimal) {
    if (jumlahAkun >= kapasitasMaksimal) {
        cout << "Database akun penuh. Tidak bisa menambahkan akun baru.\n";
        return;
    }

    Pengguna baru;
    cout << "Masukkan username: ";
    cin >> baru.username;
    cout << "Masukkan password: ";
    cin >> baru.password;
    cout << "Masukkan peran (admin/kasir): ";
    cin >> baru.peran;

    akun[jumlahAkun++] = baru;
    simpanAkun(baru);
    cout << "Akun berhasil didaftarkan.\n";
}

bool login(Pengguna akun[], int jumlahAkun, string &peranAktif) {
    string username, password;
    cout << "Masukkan username: ";
    cin >> username;
    cout << "Masukkan password: ";
    cin >> password;

    for (int i = 0; i < jumlahAkun; i++) {
        if (akun[i].username == username && akun[i].password == password) {
            peranAktif = akun[i].peran;
            cout << "Login berhasil sebagai " << peranAktif << ".\n";
            return true;
        }
    }
    cout << "Username atau password salah.\n";
    return false;
}

void muatDatabase(Barang daftarBarang[], int &jumlahBarang, int kapasitasMaksimal) {
    ifstream fileDatabase("database_barang.txt");
    if (!fileDatabase) {
        cout << "Database barang tidak ditemukan. Mulai dengan data kosong.\n";
        jumlahBarang = 0;
        return;
    }

    jumlahBarang = 0;
    while (fileDatabase >> daftarBarang[jumlahBarang].nama >> daftarBarang[jumlahBarang].harga) {
        daftarBarang[jumlahBarang].jumlah = 0; 
        jumlahBarang++;
        if (jumlahBarang >= kapasitasMaksimal) {
            cout << "Database barang melebihi kapasitas. Beberapa data tidak dimuat.\n";
            break;
        }
    }
    fileDatabase.close();
    cout << "Database barang berhasil dimuat.\n";
}

void tampilkanDatabase(const Barang daftarBarang[], int jumlahBarang) {
    cout << "\nDaftar Barang di Database:\n";
    cout << "---------------------------------\n";
    cout << "No  Nama Barang        Harga\n";
    cout << "---------------------------------\n";
    for (int i = 0; i < jumlahBarang; i++) {
        string namaDenganSpasi = ubahUnderscoreKeSpasi(daftarBarang[i].nama);
        cout << i + 1 << "   " << namaDenganSpasi << "    Rp " << daftarBarang[i].harga << "\n";
    }
    cout << "---------------------------------\n";
}

void tambahBarang(Barang daftarBarang[], int &jumlahBarang, int kapasitasMaksimal) {
    if (jumlahBarang >= kapasitasMaksimal) {
        cout << "Database barang penuh! Tidak bisa menambahkan lebih banyak barang.\n";
        return;
    }

    string namaBarang;
    cout << "Masukkan nama barang (gunakan spasi jika perlu): ";
    cin.ignore(); 
    getline(cin, namaBarang);

    string namaBarangUnderscore = ubahSpasiKeUnderscore(namaBarang);

    daftarBarang[jumlahBarang].nama = namaBarangUnderscore;
    cout << "Masukkan harga barang: ";
    cin >> daftarBarang[jumlahBarang].harga;
    daftarBarang[jumlahBarang].jumlah = 0;

    jumlahBarang++;
    cout << "Barang berhasil ditambahkan ke database dengan nama: " << namaBarangUnderscore << "\n";

    ofstream fileDatabase("database_barang.txt", ios::app);
    if (!fileDatabase) {
        cout << "Gagal menyimpan ke file database.\n";
        return;
    }
    fileDatabase << namaBarangUnderscore << " " << daftarBarang[jumlahBarang - 1].harga << "\n";
    fileDatabase.close();
}

void hapusBarangDariDatabase(Barang daftarBarang[], int &jumlahBarang) {
    if (jumlahBarang == 0) {
        cout << "Database kosong. Tidak ada barang untuk dihapus.\n";
        return;
    }

    string namaBarang;
    cout << "Masukkan nama barang yang ingin dihapus (gunakan spasi atau underscore): ";
    cin.ignore();
    getline(cin, namaBarang);

    string namaBarangDenganSpasi = ubahUnderscoreKeSpasi(namaBarang);

    bool ditemukan = false;
    for (int i = 0; i < jumlahBarang; i++) {
        if (toLowerCase(daftarBarang[i].nama) == toLowerCase(namaBarangDenganSpasi)) {
            for (int j = i; j < jumlahBarang - 1; j++) {
                daftarBarang[j] = daftarBarang[j + 1];
            }
            jumlahBarang--;
            ditemukan = true;
            cout << "Barang \"" << namaBarangDenganSpasi << "\" berhasil dihapus dari database.\n";
            break;
        }
    }

    if (!ditemukan) {
        cout << "Barang \"" << namaBarangDenganSpasi << "\" tidak ditemukan di database.\n";
    }

    ofstream fileDatabase("database_barang.txt");
    if (!fileDatabase) {
        cout << "Gagal memperbarui file database.\n";
        return;
    }

    for (int i = 0; i < jumlahBarang; i++) {
        fileDatabase << daftarBarang[i].nama << " " << daftarBarang[i].harga << "\n";
    }
    fileDatabase.close();
    cout << "File database berhasil diperbarui.\n";
}

void tambahkanBarangKeKeranjang(Barang keranjang[], int &jumlahKeranjang, const Barang daftarBarang[], int jumlahBarang, int kapasitasKeranjang) {
    if (jumlahKeranjang >= kapasitasKeranjang) {
        cout << "Keranjang belanja penuh! Tidak bisa menambahkan lebih banyak barang.\n";
        return;
    }

    string namaBarang;
    cout << "Masukkan nama barang yang ingin ditambahkan: ";
    cin.ignore();
    getline(cin, namaBarang);

    string namaBarangUnderscore = ubahSpasiKeUnderscore(namaBarang);

    bool ditemukan = false;
    for (int i = 0; i < jumlahBarang; i++) {
        if (toLowerCase(daftarBarang[i].nama) == toLowerCase(namaBarangUnderscore)) {
            cout << "Masukkan jumlah barang: ";
            int jumlah;
            cin >> jumlah;

            if (jumlah <= 0) {
                cout << "Jumlah tidak valid.\n";
                return;
            }

            keranjang[jumlahKeranjang] = daftarBarang[i];
            keranjang[jumlahKeranjang].jumlah = jumlah;
            jumlahKeranjang++;
            ditemukan = true;
            cout << "Barang berhasil ditambahkan ke keranjang.\n";
            break;
        }
    }

    if (!ditemukan) {
        cout << "Barang tidak ditemukan di database.\n";
    }
}

void cariBarang(const Barang daftarBarang[], int jumlahBarang) {
    string namaBarang;
    cout << "Masukkan nama barang yang ingin dicari: ";
    cin.ignore(); 
    getline(cin, namaBarang);

    string namaBarangUnderscore = ubahSpasiKeUnderscore(namaBarang);

    bool ditemukan = false;
    for (int i = 0; i < jumlahBarang; i++) {
        if (toLowerCase(daftarBarang[i].nama) == toLowerCase(namaBarangUnderscore)) {
            cout << "Barang ditemukan: " << daftarBarang[i].nama << ", Harga: Rp " << daftarBarang[i].harga << "\n";
            ditemukan = true;
            break;
        }
    }

    if (!ditemukan) {
        cout << "Barang tidak ditemukan di database.\n";
    }
}


void tampilkanKeranjang(const Barang keranjang[], int jumlahKeranjang) {
    cout << "\nDaftar Belanja:\n";
    cout << "---------------------------------------------\n";
    cout << "No  Nama Barang        Harga   Jumlah   Total\n";
    cout << "---------------------------------------------\n";

    double totalKeseluruhan = 0;
    for (int i = 0; i < jumlahKeranjang; i++) {
        string namaDenganSpasi = ubahUnderscoreKeSpasi(keranjang[i].nama); // Konversi underscore ke spasi
        double total = keranjang[i].harga * keranjang[i].jumlah;
        totalKeseluruhan += total;
        cout << i + 1 << "   " << namaDenganSpasi << "    Rp " << keranjang[i].harga
             << "    " << keranjang[i].jumlah << "       Rp " << total << "\n";
    }
    cout << "---------------------------------------------\n";
    cout << "Total keseluruhan: Rp " << totalKeseluruhan << "\n";
}

void hapusBarangDariKeranjang(Barang keranjang[], int &jumlahKeranjang) {
    if (jumlahKeranjang == 0) {
        cout << "Keranjang kosong. Tidak ada barang untuk dihapus.\n";
        return;
    }

    string namaBarang;
    cout << "Masukkan nama barang yang ingin dihapus dari keranjang (gunakan spasi): ";
    cin.ignore();
    getline(cin, namaBarang);

    string namaBarangUnderscore = toLowerCase(ubahSpasiKeUnderscore(namaBarang));

    bool ditemukan = false;
    for (int i = 0; i < jumlahKeranjang; i++) {
        if (toLowerCase(keranjang[i].nama) == namaBarangUnderscore) {
            for (int j = i; j < jumlahKeranjang - 1; j++) {
                keranjang[j] = keranjang[j + 1];
            }
            jumlahKeranjang--;
            ditemukan = true;
            cout << "Barang \"" << namaBarang << "\" berhasil dihapus dari keranjang.\n";
            break;
        }
    }

    if (!ditemukan) {
        cout << "Barang \"" << namaBarang << "\" tidak ditemukan di keranjang.\n";
    }
}


void prosesPembayaran(const Barang keranjang[], int jumlahKeranjang) {
    double totalKeseluruhan = 0;
    for (int i = 0; i < jumlahKeranjang; i++) {
        totalKeseluruhan += keranjang[i].harga * keranjang[i].jumlah;
    }

    cout << "Total yang harus dibayar: Rp " << totalKeseluruhan << "\n";
    double pembayaran;

    while (true) {
        cout << "Masukkan jumlah uang yang dibayarkan: Rp ";
        cin >> pembayaran;

        if (pembayaran < totalKeseluruhan) {
            cout << "Uang yang dibayarkan tidak cukup. Kekurangan: Rp " << totalKeseluruhan - pembayaran << "\n";
        } else {
            double kembalian = pembayaran - totalKeseluruhan;
            cout << "Pembayaran berhasil! Kembalian Anda: Rp " << kembalian << "\n";
            break;
        }
    }
}

int main() {
    const int KAPASITAS_AKUN = 100;
    const int KAPASITAS_BARANG = 100;
    const int KAPASITAS_KERANJANG = 50;

    Pengguna akun[KAPASITAS_AKUN];
    Barang daftarBarang[KAPASITAS_BARANG];
    Barang keranjang[KAPASITAS_KERANJANG];

    int jumlahAkun = 0;
    int jumlahBarang = 0;
    int jumlahKeranjang = 0;

    muatAkun(akun, jumlahAkun, KAPASITAS_AKUN);
    muatDatabase(daftarBarang, jumlahBarang, KAPASITAS_BARANG);

    string peranAktif;

    while (true) {
        cout << "\n=== Sistem Supermarket ===\n";
        cout << "1. Login\n";
        cout << "2. Registrasi Akun\n";
        cout << "3. Keluar\n";
        cout << "Pilih opsi: ";
        int opsi;
        cin >> opsi;

        if (opsi == 1) {
            if (login(akun, jumlahAkun, peranAktif)) {
                break;
            }
        } else if (opsi == 2) {
            registrasiAkun(akun, jumlahAkun, KAPASITAS_AKUN);
        } else if (opsi == 3) {
            cout << "Keluar dari sistem.\n";
            return 0;
        } else {
            cout << "Opsi tidak valid. Silakan coba lagi.\n";
        }
    }

    if (peranAktif == "admin") {
        while (true) {
            cout << "\n=== Menu Admin ===\n";
            cout << "1. Tampilkan Database Barang\n";
            cout << "2. Tambah Barang ke Database\n";
            cout << "3. Hapus Barang dari Database\n";
            cout << "4. Logout\n";
            cout << "Pilih opsi: ";
            int opsi;
            cin >> opsi;

            if (opsi == 1) {
                tampilkanDatabase(daftarBarang, jumlahBarang);
            } else if (opsi == 2) {
                tambahBarang(daftarBarang, jumlahBarang, KAPASITAS_BARANG);
            } else if (opsi == 3) {
                hapusBarangDariDatabase(daftarBarang, jumlahBarang);
            } else if (opsi == 4) {
                cout << "Logout berhasil.\n";
                return main();
            } else {
                cout << "Opsi tidak valid. Silakan coba lagi.\n";
            }
        }
    } else if (peranAktif == "kasir") {
        while (true) {
            cout << "\n=== Menu Kasir ===\n";
            cout << "1. Tampilkan Database Barang\n";
            cout << "2. Cari Barang di Database\n";
            cout << "3. Tambah Barang ke Keranjang\n";
            cout << "4. Tampilkan Keranjang\n";
            cout << "5. Hapus Barang dari Keranjang\n";
            cout << "6. Lakukan Pembayaran\n";
            cout << "7. Logout\n";
            cout << "Pilih opsi: ";
            int opsi;
            cin >> opsi;

            if (opsi == 1) {
                tampilkanDatabase(daftarBarang, jumlahBarang);
            } else if (opsi == 2) {
                cariBarang(daftarBarang, jumlahBarang);
            } else if (opsi == 3) {
                tambahkanBarangKeKeranjang(keranjang, jumlahKeranjang, daftarBarang, jumlahBarang, KAPASITAS_KERANJANG);
            } else if (opsi == 4) {
                tampilkanKeranjang(keranjang, jumlahKeranjang);
            } else if (opsi == 5) {
                hapusBarangDariKeranjang(keranjang, jumlahKeranjang);
            } else if (opsi == 6) {
                prosesPembayaran(keranjang, jumlahKeranjang);
            } else if (opsi == 7) {
                cout << "Logout berhasil.\n";
                return main();
            } else {
                cout << "Opsi tidak valid. Silakan coba lagi.\n";
            }
        }
    }

    return 0;
}