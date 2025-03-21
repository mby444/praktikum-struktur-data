#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <ctime>

#define FILE_DATA "database/buku.txt"
#define FILE_PINJAMAN "database/pinjaman.txt"
#define FILE_RIWAYAT "database/riwayat.txt"
#define FILE_PENGEMBALIAN "database/pengembalian.txt"

using namespace std;

struct Buku
{
    string id, judul, penulis, genre;
    int tahun;
    bool tersedia;
    Buku *next, *prev;
};

struct Pinjaman
{
    string id, judul, nama, nim;
    time_t tanggal;
    Pinjaman *next;
};

struct Riwayat
{
    string id, judul, nama, nim;
    time_t tanggal;
    Riwayat *next;
    Riwayat *prev;
};

struct Pengembalian
{
    string id, judul, nama, nim;
    time_t tanggal;
    Pengembalian *next;
};

Buku *head = NULL;
Buku *tail = NULL;

Pinjaman *headPinjaman = NULL;
Pinjaman *tailPinjaman = NULL;

Riwayat *headRiwayat = NULL;
Riwayat *tailRiwayat = NULL;

Pengembalian *headPengembalian = NULL;

void logger(string pesan)
{
    cout << "Log: " << pesan << endl;
}

void outputHeaderPerpustakaan()
{
    cout << "=======================================" << endl;
    cout << "===          Perpustakaan           ===" << endl;
    cout << "=======================================" << endl;
}

void outputSelamatDatang()
{
    cout << "=======================================" << endl;
    cout << "=== Selamat datang di Perpustakaan! ===" << endl;
    cout << "=======================================" << endl;
    cout << "1. Masuk Sebagai Pegawai" << endl;
    cout << "2. Masuk Sebagai Pengunjung" << endl;
    cout << "0. Simpan dan Keluar" << endl;
    cout << endl;
}

void outputDasboardAdmin()
{
    cout << "=======================================" << endl;
    cout << "===          Dashboard Admin        ===" << endl;
    cout << "=======================================" << endl;
    cout << "1. Tambah Buku" << endl;
    cout << "2. Tampilkan Buku" << endl;
    cout << "3. Ubah Buku" << endl;
    cout << "4. Hapus Buku" << endl;
    cout << "5. Proses Permintaan Pinjaman" << endl;
    cout << "6. Proses Pengembalian Buku" << endl;
    cout << "0. Kembali" << endl;
    cout << endl;
}

void outputDashboardPengunjung()
{
    cout << "=======================================" << endl;
    cout << "===       Dashboard Pengunjung      ===" << endl;
    cout << "=======================================" << endl;
    cout << "1. Daftar Buku" << endl;
    cout << "2. Riwayat Pinjaman" << endl;
    cout << "0. Kembali" << endl;
    cout << endl;
}

void outputDaftarBukuAdmin()
{
    cout << "1. Selanjutnya" << endl;
    cout << "2. Sebelumnya" << endl;
    cout << "0. Kembali" << endl;
    cout << endl;
}

void outputDaftarBukuPengunjung()
{
    cout << "1. Selanjutnya" << endl;
    cout << "2. Sebelumnya" << endl;
    cout << "3. Pinjam" << endl;
    cout << "0. Kembali" << endl;
    cout << endl;
}

void outputDaftarRiwayat()
{
    cout << "1. Selanjutnya" << endl;
    cout << "2. Sebelumnya" << endl;
    cout << "3. Kembalikan Buku" << endl;
    cout << "0. Kembali" << endl;
    cout << endl;
}

void outputKonfirmasiPengembalianBuku(string judul)
{
    cout << "Apakah Anda yakin ingin mengembalikan buku \"" << judul << "\"?" << endl;
    cout << "1. Ya" << endl;
    cout << "2. Tidak" << endl;
}

void outputTambahBuku()
{
    cout << "=======================================" << endl;
    cout << "===          Tambah Buku            ===" << endl;
    cout << "=======================================" << endl;
}

void outputUbahBuku()
{
    cout << "=======================================" << endl;
    cout << "===          Ubah Buku              ===" << endl;
    cout << "=======================================" << endl;
    cout << "1. Judul" << endl;
    cout << "2. Penulis" << endl;
    cout << "3. Tahun" << endl;
    cout << "4. Genre" << endl;
    cout << "0. Kembali" << endl;
    cout << endl;
}

void outputHeaderPermintaanPinjaman(string nomorPinjaman)
{
    cout << "=======================================" << endl;
    cout << "         Permintaan Ke-" << nomorPinjaman << "      " << endl;
    cout << "=======================================" << endl;
}

void outputHeaderPengembalian(string nomorPengembalian)
{
    cout << "=======================================" << endl;
    cout << "         Pengembalian Ke-" << nomorPengembalian << "    " << endl;
    cout << "=======================================" << endl;
}

void outputMenuPermintaanPinjam()
{
    cout << "1. Setujui" << endl;
    cout << "2. Tolak" << endl;
    cout << "0. Kembali" << endl;
    cout << endl;
}

void outputMenuPengembalian()
{
    cout << "1. Proses" << endl;
    cout << "0. Kembali" << endl;
    cout << endl;
}

bool cekNumerik(string input)
{
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] > '9' || input[i] < '0')
        {
            cout << "input[i]: " << input[i] << endl;
            return false;
        }
    }
    return true;
}

int tahunSekarang()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return (1900 + ltm->tm_year);
}

string namaBulan(int bulan)
{
    string namaBulan[12] = {"Januari", "Februari", "Maret", "April", "Mei", "Juni",
                            "Juli", "Agustus", "September", "Oktober", "November", "Desember"};
    return namaBulan[bulan];
}

string fullTanggalString(time_t tanggal)
{
    tm *ltm = localtime(&tanggal);
    string tanggalStr = to_string(ltm->tm_mday);
    string bulanStr = to_string(1 + ltm->tm_mon);
    string tahunStr = to_string(1900 + ltm->tm_year);
    string jamStr = to_string(ltm->tm_hour);
    string menitStr = to_string(ltm->tm_min);

    if (tanggalStr.length() == 1)
    {
        tanggalStr = "0" + tanggalStr;
    }
    if (bulanStr.length() == 1)
    {
        bulanStr = "0" + bulanStr;
    }
    if (jamStr.length() == 1)
    {
        jamStr = "0" + jamStr;
    }
    if (menitStr.length() == 1)
    {
        menitStr = "0" + menitStr;
    }

    return (tanggalStr + " " + namaBulan(stoi(bulanStr) - 1) + " " + tahunStr + " " + jamStr + ":" + menitStr);
    // Contoh output: 8 Maret 2025 08:50
}

int indexHurufTerakhir(string str)
{
    for (int i = str.length() - 1; i >= 0; i--)
    {
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))
        {
            return i;
        }
    }

    return -1;
}

int indexHurufPertama(string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))
        {
            return i;
        }
    }

    return -1;
}

void lihatBuku(Buku *buku)
{
    cout << "-----------------------------------" << endl;
    cout << "Judul: " << buku->judul << endl;
    cout << "ID: " << buku->id << endl;
    cout << "Penulis: " << buku->penulis << endl;
    cout << "Tahun: " << buku->tahun << endl;
    cout << "Genre: " << buku->genre << endl;
    cout << "Tersedia: " << (buku->tersedia ? "Ya" : "Tidak") << endl;
    cout << "-----------------------------------" << endl;
    cout << endl;
}

void lihatRiwayat(Riwayat *riwayat)
{
    cout << "-----------------------------------" << endl;
    cout << "Judul: " << riwayat->judul << endl;
    cout << "ID: " << riwayat->id << endl;
    cout << "Nama: " << riwayat->nama << endl;
    cout << "NIM: " << riwayat->nim << endl;
    cout << "Tanggal: " << fullTanggalString(riwayat->tanggal) << endl;
    cout << "-----------------------------------" << endl;
    cout << endl;
}

void lihatPinjaman(Pinjaman *pinjaman)
{
    cout << "-----------------------------------" << endl;
    cout << "Judul: " << pinjaman->judul << endl;
    cout << "ID: " << pinjaman->id << endl;
    cout << "Nama: " << pinjaman->nama << endl;
    cout << "NIM: " << pinjaman->nim << endl;
    cout << "Tanggal: " << fullTanggalString(pinjaman->tanggal) << endl;
    cout << "-----------------------------------" << endl;
    cout << endl;
}

void lihatPengembalian(Pengembalian *pengembalian)
{
    cout << "-----------------------------------" << endl;
    cout << "Judul: " << pengembalian->judul << endl;
    cout << "ID: " << pengembalian->id << endl;
    cout << "Nama: " << pengembalian->nama << endl;
    cout << "NIM: " << pengembalian->nim << endl;
    cout << "Tanggal: " << fullTanggalString(pengembalian->tanggal) << endl;
    cout << "-----------------------------------" << endl;
    cout << endl;
}

string nomorPinjaman(string idPinjaman)
{
    int iht = indexHurufTerakhir(idPinjaman);
    int len = idPinjaman.length();
    string nomor = idPinjaman.substr(iht + 1, len - (iht + 1));
    return nomor;
}

string nomorPengembalian()
{
    int jumlahNode = 0;
    string nomor;
    Pengembalian *bantu = headPengembalian;

    while (bantu != NULL)
    {
        jumlahNode++;
        bantu = bantu->next;
    }

    nomor = to_string(jumlahNode);

    if (nomor.size() == 1)
    {
        nomor = "0" + nomor;
    }

    return nomor;
}

string toLowerCase(string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        str[i] = tolower(str[i]);
    }

    return str;
}

int inputOpsi()
{
    try
    {
        string input;
        cout << "> ";
        getline(cin, input);
        return stoi(input);
    }
    catch (...)
    {
        cout << "Mohon masukkan bilangan bulat!" << endl;
        return inputOpsi();
    }
}

string inputData(string label = "")
{
    try
    {
        string input;
        cout << label << "> ";
        getline(cin, input);

        if (input.empty())
        {
            throw "Data tidak boleh kosong!";
        }

        return input;
    }
    catch (...)
    {
        cout << "Mohon masukkan data yang benar!" << endl;
        return inputData(label);
    }
}

int inputDataNumerik(string label = "")
{
    try
    {
        string input = inputData(label);

        if (!cekNumerik(input))
        {
            throw "Data harus berupa angka!";
        }

        return stoi(input);
    }
    catch (exception e)
    {
        cout << e.what() << endl;
        cout << "Mohon masukkan data berupa angka!" << endl;
        return inputDataNumerik(label);
    }
}

Buku *cariBuku(string id)
{
    Buku *bantu = head;

    do
    {
        if (bantu->id == id)
        {
            return bantu;
        }
        bantu = bantu->next;
    } while (bantu != head);

    return NULL;
}

Buku *cariBukuDipinjam(string idPinjaman)
{
    string id = idPinjaman.substr(0, indexHurufPertama(idPinjaman));
    Buku *buku = cariBuku(id);
    return buku;
}

string tetapkanIdBuku(string tempId)
{
    if (head == NULL)
    {
        return tempId;
    }

    Buku *bantu = head;
    char lastChar;

    do
    {
        if (bantu->id == tempId)
        {
            lastChar = tempId[tempId.length() - 1];
            lastChar++;

            tempId[tempId.length() - 1] = lastChar;

            if (lastChar == '9')
            {
                tempId += "0";
            }
        }
        bantu = bantu->next;
    } while (bantu != head);

    return tempId;
}

string buatIdBuku(string &judul, int &tahunInt)
{
    int firstCharInt = (int)judul[0];
    int lastCharInt = (int)judul[judul.length() - 1];
    int yearDigitSum = 0;
    string tahun = to_string(tahunInt);
    string firstTwoDigit = to_string(abs(lastCharInt - firstCharInt));
    string secondAndThirdDigit;
    string lastDigit = "0";
    string tempId;

    for (int i = 0; i < tahun.length(); i++)
    {
        yearDigitSum += stoi(tahun.substr(i, 1));
    }

    secondAndThirdDigit = to_string(yearDigitSum);

    if (firstTwoDigit.length() == 1)
    {
        firstTwoDigit = "0" + firstTwoDigit;
    }

    if (secondAndThirdDigit.length() == 1)
    {
        secondAndThirdDigit = "0" + secondAndThirdDigit;
    }

    tempId = firstTwoDigit + secondAndThirdDigit + lastDigit;

    return tetapkanIdBuku(tempId);
}

int buatUrutanPinjaman()
{
    if (headPinjaman == NULL)
    {
        return 1;
    }

    string urutanTerakhir = tailPinjaman->id.substr(indexHurufTerakhir(tailPinjaman->id) + 1, 2);
    int urutan = stoi(urutanTerakhir) + 1;
    return urutan;
}

string buatIdPinjaman(Buku *buku, string nama)
{
    string idBuku = buku->id;
    string inisialGenre = toLowerCase(buku->genre.substr(0, 1));
    string duaDigitTerakhirTahun = to_string(tahunSekarang()).substr(2, 2);
    string duaKarakterNama = toLowerCase(nama.substr(0, 2));
    string urutanPeminjaman = to_string(buatUrutanPinjaman());

    if (duaDigitTerakhirTahun.length() == 1)
    {
        duaDigitTerakhirTahun = "0" + duaDigitTerakhirTahun;
    }
    if (urutanPeminjaman.length() == 1)
    {
        urutanPeminjaman = "0" + urutanPeminjaman;
    }

    string id = idBuku + inisialGenre + duaDigitTerakhirTahun + duaKarakterNama + urutanPeminjaman;
    return id;
}

void tambahKeLinkedList(Buku *buku)
{
    if (head == NULL)
    {
        head = buku;
        tail = buku;
    }
    else
    {
        buku->next = head;
        buku->prev = tail;
        tail->next = buku;
        head->prev = buku;
        tail = buku;
    }
}

void hapusDariLinkedList(Buku *buku)
{
    if (head == tail && head->id == buku->id)
    {
        delete head;
        head = NULL;
        tail = NULL;
        return;
    }

    if (buku == head)
    {
        head = buku->next;
    }
    else if (buku == tail)
    {
        tail = buku->prev;
    }

    buku->prev->next = buku->next;
    buku->next->prev = buku->prev;
    delete buku;
}

void tambahKeRiwayat(Riwayat *riwayat)
{
    if (headRiwayat == NULL)
    {
        headRiwayat = riwayat;
        tailRiwayat = riwayat;
    }
    else
    {
        riwayat->next = headRiwayat;
        riwayat->prev = tailRiwayat;
        tailRiwayat->next = riwayat;
        headRiwayat->prev = riwayat;
        tailRiwayat = riwayat;
    }
}

void hapusDariRiwayat(Riwayat *riwayat)
{
    if (headRiwayat == tailRiwayat && headRiwayat->id == riwayat->id)
    {
        delete headRiwayat;
        headRiwayat = NULL;
        tailRiwayat = NULL;
        return;
    }

    if (riwayat == headRiwayat)
    {
        headRiwayat = riwayat->next;
    }
    else if (riwayat == tailRiwayat)
    {
        tailRiwayat = riwayat->prev;
    }

    riwayat->prev->next = riwayat->next;
    riwayat->next->prev = riwayat->prev;
    delete riwayat;
}

void enqueuePinjaman(Pinjaman *pinjaman)
{
    if (headPinjaman == NULL)
    {
        headPinjaman = pinjaman;
        tailPinjaman = pinjaman;
    }
    else
    {
        tailPinjaman->next = pinjaman;
        tailPinjaman = pinjaman;
        tailPinjaman->next = NULL;
    }
}

void dequeuePinjaman()
{
    if (headPinjaman == NULL)
    {
        return;
    }

    if (headPinjaman == tailPinjaman)
    {
        delete headPinjaman;
        headPinjaman = NULL;
        tailPinjaman = NULL;
        return;
    }

    Pinjaman *hapus = headPinjaman;
    headPinjaman = headPinjaman->next;
    delete hapus;
}

void pushPengembalian(Pengembalian *pengembalian)
{
    if (headPengembalian == NULL)
    {
        headPengembalian = pengembalian;
    }
    else
    {
        pengembalian->next = headPengembalian;
        headPengembalian = pengembalian;
    }
}

void popPengembalian()
{
    if (headPengembalian == NULL)
    {
        return;
    }

    Pengembalian *hapus = headPengembalian;
    headPengembalian = headPengembalian->next;
    delete hapus;
}

Buku *lineKeBuku(string line)
{
    Buku *buku = new Buku;
    string delimiter = "|";
    int delimiterIndex = 0;
    int i = 0;
    string token;

    // Mengubah satu baris data menjadi objek buku
    while (line.find(delimiter) != string::npos)
    {
        delimiterIndex = line.find(delimiter);
        token = line.substr(0, delimiterIndex);

        switch (i)
        {
        case 0:
            buku->id = token;
            break;
        case 1:
            buku->judul = token;
            break;
        case 2:
            buku->penulis = token;
            break;
        case 3:
            buku->tahun = stoi(token);
            break;
        case 4:
            buku->genre = token;
            break;
        }
        line.erase(0, delimiterIndex + 1);
        i++;
    }

    buku->tersedia = line.substr(0, 1) == "1";
    buku->next = buku;
    buku->prev = buku;

    return buku;
}

Pinjaman *lineKePinjaman(string line)
{
    Pinjaman *pinjaman = new Pinjaman;
    string delimiter = "|";
    int delimiterIndex = 0;
    int i = 0;
    string token;

    // Mengubah satu baris data menjadi objek pinjaman
    while (line.find(delimiter) != string::npos)
    {
        delimiterIndex = line.find(delimiter);
        token = line.substr(0, delimiterIndex);

        switch (i)
        {
        case 0:
            pinjaman->id = token;
            break;
        case 1:
            pinjaman->judul = token;
            break;
        case 2:
            pinjaman->nama = token;
            break;
        case 3:
            pinjaman->nim = token;
            break;
        }
        line.erase(0, delimiterIndex + 1);
        i++;
    }

    pinjaman->tanggal = stoi(line);
    pinjaman->next = NULL;

    return pinjaman;
}

Riwayat *lineKeRiwayat(string line)
{
    Riwayat *riwayat = new Riwayat;
    string delimiter = "|";
    int delimiterIndex = 0;
    int i = 0;
    string token;

    // Mengubah satu baris data menjadi objek riwayat
    while (line.find(delimiter) != string::npos)
    {
        delimiterIndex = line.find(delimiter);
        token = line.substr(0, delimiterIndex);

        switch (i)
        {
        case 0:
            riwayat->id = token;
            break;
        case 1:
            riwayat->judul = token;
            break;
        case 2:
            riwayat->nama = token;
            break;
        case 3:
            riwayat->nim = token;
            break;
        }
        line.erase(0, delimiterIndex + 1);
        i++;
    }

    riwayat->tanggal = stoi(line);
    riwayat->next = riwayat;
    riwayat->prev = riwayat;

    return riwayat;
}

Pengembalian *lineKePengembalian(string line)
{
    Pengembalian *pengembalian = new Pengembalian;
    string delimiter = "|";
    int delimiterIndex = 0;
    int i = 0;
    string token;

    // Mengubah satu baris data menjadi objek pengembalian
    while (line.find(delimiter) != string::npos)
    {
        delimiterIndex = line.find(delimiter);
        token = line.substr(0, delimiterIndex);

        switch (i)
        {
        case 0:
            pengembalian->id = token;
            break;
        case 1:
            pengembalian->judul = token;
            break;
        case 2:
            pengembalian->nama = token;
            break;
        case 3:
            pengembalian->nim = token;
            break;
        }
        line.erase(0, delimiterIndex + 1);
        i++;
    }

    pengembalian->tanggal = stoi(line);
    pengembalian->next = NULL;

    return pengembalian;
}

Pengembalian *riwayatKePengembalian(Riwayat *riwayat)
{
    Pengembalian *pengembalianBaru = new Pengembalian;
    pengembalianBaru->id = riwayat->id;
    pengembalianBaru->judul = riwayat->judul;
    pengembalianBaru->nama = riwayat->nama;
    pengembalianBaru->nim = riwayat->nim;
    pengembalianBaru->tanggal = riwayat->tanggal;
    pengembalianBaru->next = NULL;

    return pengembalianBaru;
}

Buku *buatBukuBaru()
{
    Buku *buku = new Buku;

    buku->judul = inputData("Judul");
    buku->penulis = inputData("Penulis");
    buku->tahun = inputDataNumerik("Tahun");
    buku->genre = inputData("Genre");
    buku->id = buatIdBuku(buku->judul, buku->tahun);
    buku->tersedia = true;
    buku->next = buku;
    buku->prev = buku;

    return buku;
}

Pinjaman *buatPinjamanBaru(Buku *buku)
{
    Pinjaman *pinjaman = new Pinjaman;

    pinjaman->judul = buku->judul;
    pinjaman->nama = inputData("Nama Peminjam");
    pinjaman->nim = inputData("NIM Peminjam");
    pinjaman->id = buatIdPinjaman(buku, pinjaman->nama);
    pinjaman->tanggal = time(0);
    pinjaman->next = NULL;

    return pinjaman;
}

/* Fungsi Utama */

void pinjamBuku(Buku *tampil, Pinjaman *pinjaman)
{
    if (!tampil->tersedia)
    {
        cout << "Buku tidak tersedia!" << endl;
        return;
    }
    pinjaman = buatPinjamanBaru(tampil);
    enqueuePinjaman(pinjaman);
    cout << "Permintaan pinjaman telah dibuat!" << endl;
    lihatPinjaman(pinjaman);
    cout << "Menunggu persetujuan pegawai" << endl;
}

void kembalikanBuku(Riwayat **riwayatPtr)
{
    Riwayat *riwayat = *riwayatPtr;
    string judul = riwayat->judul;

    outputKonfirmasiPengembalianBuku(judul);

    string konfirmasi = inputData();
    Pengembalian *pengembalian;

    if (konfirmasi == "2")
    {
        cout << "Buku \"" + judul + "\" tidak jadi dikembalikan" << endl;
        return;
    }

    if (konfirmasi != "1")
    {
        cout << "Opsi tidak ditemukan!" << endl;
        return;
    }

    pengembalian = riwayatKePengembalian(riwayat);
    pushPengembalian(pengembalian);

    *riwayatPtr = riwayat->next;
    hapusDariRiwayat(riwayat);

    cout << "Buku \"" + judul + "\" telah dikembalikan!" << endl;
}

void tampilkanBuku(bool sebagaiPegawai = true)
{
    if (head == NULL)
    {
        cout << "Data buku masih kosong!" << endl;
        return;
    }

    Buku *tampil = head;
    Pinjaman *pinjaman;
    int opsi;

    do
    {
        outputHeaderPerpustakaan();
        lihatBuku(tampil);
        sebagaiPegawai ? outputDaftarBukuAdmin() : outputDaftarBukuPengunjung();
        opsi = inputOpsi();

        switch (opsi)
        {
        case 1:
            tampil = tampil->next;
            break;
        case 2:
            tampil = tampil->prev;
            break;
        case 3:
            if (sebagaiPegawai)
            {
                cout << "Opsi" << opsi << " tidak ditemukan!" << endl;
                break;
            }
            pinjamBuku(tampil, pinjaman);
            break;
        case 0:
            break;

        default:
            cout << "Opsi" << opsi << " tidak ditemukan!" << endl;
            break;
        }
    } while (opsi != 0);
}

void tampilkanRiwayat()
{
    if (headRiwayat == NULL)
    {
        cout << "Riwayat pinjaman masih kosong!" << endl;
        return;
    }

    Riwayat *tampil = headRiwayat;
    int opsi;

    do
    {
        outputHeaderPerpustakaan();
        lihatRiwayat(tampil);
        outputDaftarRiwayat();

        opsi = inputOpsi();

        switch (opsi)
        {
        case 1:
            tampil = tampil->next;
            break;
        case 2:
            tampil = tampil->prev;
            break;
        case 3:
            kembalikanBuku(&tampil);
            break;
        case 0:
            break;
        default:
            cout << "Opsi " << opsi << " tidak ditemukan!" << endl;
            break;
        }

        if (headRiwayat == NULL)
        {
            cout << "Riwayat pinjaman sudah kosong!" << endl;
            break;
        }

    } while (opsi != 0);
}

void tambahBuku()
{
    outputTambahBuku();

    Buku *bukuBaru = buatBukuBaru();
    tambahKeLinkedList(bukuBaru);

    string input = inputData("Tambah lainnya? (y/N)");

    if (input == "y" || input == "Y")
    {
        tambahBuku();
    }
    else
    {
        cout << "Data berhasil ditambahkan!" << endl;
    }
}

void ubahBuku()
{
    if (head == NULL)
    {
        cout << "Data buku masih kosong!" << endl;
        return;
    }

    string idDicari = inputData("ID Buku yang ingin diubah");
    Buku *buku = cariBuku(idDicari);

    if (buku == NULL)
    {
        cout << "Buku dengan ID " << idDicari << " tidak ditemukan!" << endl;
        return;
    }

    if (buku->tersedia == false)
    {
        cout << "Buku tidak bisa diubah karena sedang dipinjam!" << endl;
        return;
    }

    int opsi;

    do
    {
        outputUbahBuku();

        opsi = inputOpsi();

        switch (opsi)
        {
        case 1:
            buku->judul = inputData("Judul baru");
            buku->id = buatIdBuku(buku->judul, buku->tahun);
            break;
        case 2:
            buku->penulis = inputData("Penulis baru");
            break;
        case 3:
            buku->tahun = inputDataNumerik("Tahun baru");
            buku->id = buatIdBuku(buku->judul, buku->tahun);
            break;
        case 4:
            buku->genre = inputData("Genre baru");
            break;
        case 0:
            break;
        default:
            cout << "Opsi " << opsi << " tidak ditemukan!" << endl;
            break;
        }

        lihatBuku(buku);
        cout << endl;
    } while (opsi != 0);
}

void hapusBuku()
{
    if (head == NULL)
    {
        cout << "Data buku masih kosong!" << endl;
        return;
    }

    string idDicari = inputData("ID Buku yang ingin dihapus");
    Buku *buku = cariBuku(idDicari);

    if (buku == NULL)
    {
        cout << "Buku dengan ID " << idDicari << " tidak ditemukan!" << endl;
        return;
    }

    if (buku->tersedia == false)
    {
        cout << "Buku tidak bisa dihapus karena sedang dipinjam!" << endl;
        return;
    }

    lihatBuku(buku);

    string konfirmasiInput = inputData("Apakah Anda yakin ingin menghapus buku ini? (y/N)");

    if (konfirmasiInput == "y" || konfirmasiInput == "Y")
    {
        string idDihapus = buku->id;
        hapusDariLinkedList(buku);
        cout << "Buku dengan ID " << idDihapus << " berhasil dihapus!" << endl;
    }
    else
    {
        cout << "Buku tidak jadi dihapus!" << endl;
    }
}

void salinPinjamanKeRiwayat()
{
    Riwayat *riwayatBaru = new Riwayat;
    riwayatBaru->id = headPinjaman->id;
    riwayatBaru->judul = headPinjaman->judul;
    riwayatBaru->nama = headPinjaman->nama;
    riwayatBaru->nim = headPinjaman->nim;
    riwayatBaru->tanggal = headPinjaman->tanggal;
    riwayatBaru->next = riwayatBaru;
    riwayatBaru->prev = riwayatBaru;

    tambahKeRiwayat(riwayatBaru);
}

bool tandaiBukuDipinjam(string idPinjam)
{
    Buku *bantu = head;
    string id = idPinjam.substr(0, indexHurufPertama(idPinjam));
    bool found = false;

    do
    {
        if (bantu->id == id)
        {
            bantu->tersedia = false;
            found = true;
            break;
        }
        bantu = bantu->next;
    } while (bantu != head);

    return found;
}

bool tandaiBukuDikembalikan(string idPengembalian)
{
    Buku *bantu = head;
    string id = idPengembalian.substr(0, indexHurufPertama(idPengembalian));
    bool found = false;

    do
    {
        if (bantu->id == id)
        {
            bantu->tersedia = true;
            found = true;
            break;
        }
        bantu = bantu->next;
    } while (bantu != head);

    return found;
}

void prosesPermintaanPinjaman()
{
    if (headPinjaman == NULL)
    {
        cout << "Data pinjaman masih kosong!" << endl;
        return;
    }

    int opsi = 0;
    bool bukuDitandai;
    string idPinjaman;
    Buku *buku;

    do
    {
        idPinjaman = headPinjaman->id;
        outputHeaderPermintaanPinjaman(nomorPinjaman(idPinjaman));
        lihatPinjaman(headPinjaman);
        outputMenuPermintaanPinjam();
        opsi = inputOpsi();

        switch (opsi)
        {
        case 1:
            buku = cariBukuDipinjam(idPinjaman);

            if (buku == NULL || !buku->tersedia)
            {
                cout << "Permintaan pinjaman dengan ID " << idPinjaman << " ditolak karena buku sudah tidak ada atau dipinjam!" << endl;
                dequeuePinjaman();
                break;
            }

            salinPinjamanKeRiwayat();
            tandaiBukuDipinjam(idPinjaman);
            dequeuePinjaman();
            cout << "Permintaan pinjaman dengan ID " << idPinjaman << " berhasil disetujui!" << endl;
            break;
        case 2:
            dequeuePinjaman();
            cout << "Permintaan pinjaman dengan ID " << idPinjaman << " ditolak!" << endl;
            break;
        case 0:
            break;
        default:
            cout << "Opsi " << opsi << " tidak ditemukan!" << endl;
            break;
        }

        if (headPinjaman == NULL)
        {
            cout << "Data pinjaman sudah kosong!" << endl;
            break;
        }
    } while (opsi != 0);
}

void prosesPengembalian()
{
    if (headPengembalian == NULL)
    {
        cout << "Data pengembalian buku masih kosong!" << endl;
        return;
    }

    int opsi = 0;
    bool bukuDitandai;
    string idPengembalian, judulPengembalian;
    Pengembalian *pengembalian;

    do
    {
        idPengembalian = headPengembalian->id;
        judulPengembalian = headPengembalian->judul;
        outputHeaderPengembalian(nomorPengembalian());
        lihatPengembalian(headPengembalian);
        outputMenuPengembalian();
        opsi = inputOpsi();

        switch (opsi)
        {
        case 1:
            popPengembalian();
            tandaiBukuDikembalikan(idPengembalian);
            cout << "Buku \"" + judulPengembalian + "\" telah dikembalikan!" << endl;
            break;
        case 0:
            break;
        default:
            cout << "Opsi " << opsi << " tidak ditemukan!" << endl;
            break;
        }

        if (headPengembalian == NULL)
        {
            cout << "Data pengembalian buku sudah kosong!" << endl;
            break;
        }
    } while (opsi != 0);
}

void muatDataBukuKeLinkedList()
{
    ifstream file(FILE_DATA);

    if (!file.is_open())
    {
        return;
    }

    string line;

    // Tambah semua data buku ke linked list
    while (getline(file, line))
    {
        Buku *buku = lineKeBuku(line);
        tambahKeLinkedList(buku);
    }
}

void muatDataPinjamanKeQueue()
{
    ifstream file(FILE_PINJAMAN);

    if (!file.is_open())
    {
        return;
    }

    string line;
    while (getline(file, line))
    {
        Pinjaman *pinjaman = lineKePinjaman(line);
        enqueuePinjaman(pinjaman);
    }
}

void muatDataRiwayatKeLinkedList()
{
    ifstream file(FILE_RIWAYAT);

    if (!file.is_open())
    {
        return;
    }

    string line;

    // Tambah semua data riwayat ke linked list
    while (getline(file, line))
    {
        Riwayat *riwayat = lineKeRiwayat(line);
        tambahKeRiwayat(riwayat);
    }
}

void muatDataPengembalianKeStack()
{
    ifstream file(FILE_PENGEMBALIAN);

    if (!file.is_open())
    {
        return;
    }

    string line;
    while (getline(file, line))
    {
        Pengembalian *pengembalian = lineKePengembalian(line);
        pushPengembalian(pengembalian);
    }
}

void kosongkanLinkedList()
{
    if (head == NULL)
    {
        return;
    }
    Buku *bantu = head;

    do
    {
        Buku *hapus = bantu;
        bantu = bantu->next;
        delete hapus;
    } while (bantu != head);

    head = NULL;
    tail = NULL;
}

void kosongkanQueue()
{
    if (headPinjaman == NULL)
    {
        return;
    }

    Pinjaman *bantu = headPinjaman;

    do
    {
        Pinjaman *hapus = bantu;
        bantu = bantu->next;
        delete hapus;
    } while (bantu != NULL);

    headPinjaman = NULL;
    tailPinjaman = NULL;
}

void kosongkanRiwayat()
{
    if (headRiwayat == NULL)
    {
        return;
    }

    Riwayat *bantu = headRiwayat;

    do
    {
        Riwayat *hapus = bantu;
        bantu = bantu->next;
        delete hapus;
    } while (bantu != headRiwayat);

    headRiwayat = NULL;
    tailRiwayat = NULL;
}

void kosongkanPengembalian()
{
    if (headPengembalian == NULL)
    {
        return;
    }

    Pengembalian *bantu = headPengembalian;

    do
    {
        Pengembalian *hapus = bantu;
        bantu = bantu->next;
        delete hapus;
    } while (bantu != NULL);

    headPengembalian = NULL;
}

void simpanDataBukuKeFile()
{
    ofstream file(FILE_DATA);

    if (head == NULL)
    {
        return;
    }

    Buku *bantu = head;

    do
    {
        file << bantu->id << "|" << bantu->judul << "|" << bantu->penulis << "|" << bantu->tahun << "|" << bantu->genre << "|" << (bantu->tersedia ? "1" : "0") << endl;
        bantu = bantu->next;
    } while (bantu != head);

    file.close();
}

void simpanDataPinjamanKeFile()
{
    ofstream file(FILE_PINJAMAN);

    if (headPinjaman == NULL)
    {
        return;
    }

    Pinjaman *bantu = headPinjaman;

    do
    {
        file << bantu->id << "|" << bantu->judul << "|" << bantu->nama << "|" << bantu->nim << "|" << bantu->tanggal << endl;
        bantu = bantu->next;
    } while (bantu != NULL);

    file.close();
}

void simpanDataRiwayatKeFile()
{
    ofstream file(FILE_RIWAYAT);

    if (headRiwayat == NULL)
    {
        return;
    }

    Riwayat *bantu = headRiwayat;

    do
    {
        file << bantu->id << "|" << bantu->judul << "|" << bantu->nama << "|" << bantu->nim << "|" << bantu->tanggal << endl;
        bantu = bantu->next;
    } while (bantu != headRiwayat);

    file.close();
}

void simpanDataPengembalianKeFile()
{
    ofstream file(FILE_PENGEMBALIAN);
    string line, teks;

    if (headPengembalian == NULL)
    {
        return;
    }

    Pengembalian *bantu = headPengembalian;

    do
    {
        line = bantu->id + "|" + bantu->judul + "|" + bantu->nama + "|" + bantu->nim + "|" + to_string(bantu->tanggal) + "\n";
        teks = line + teks;
        bantu = bantu->next;
    } while (bantu != NULL);

    file << teks;
    file.close();
}

void simpanDanKeluar()
{
    cout << "Menyimpan data..." << endl;
    simpanDataBukuKeFile();
    cout << "Berhasil menyimpan data buku!" << endl;
    simpanDataPinjamanKeFile();
    cout << "Berhasil menyimpan data pinjaman!" << endl;
    simpanDataRiwayatKeFile();
    cout << "Berhasil menyimpan data riwayat!" << endl;
    simpanDataPengembalianKeFile();
    cout << "Berhasil menyimpan data pengembalian!" << endl;
    cout << "Data berhasil disimpan!" << endl;
    kosongkanLinkedList();
    kosongkanQueue();
    kosongkanRiwayat();
    kosongkanPengembalian();
    cout << "Terima kasih telah mengunjungi perpustakaan!" << endl;
}

void masukSebagaiPegawai()
{
    outputDasboardAdmin();

    int opsi = inputOpsi();

    switch (opsi)
    {
    case 1:
        tambahBuku();
        break;
    case 2:
        tampilkanBuku();
        break;
    case 3:
        ubahBuku();
        break;
    case 4:
        hapusBuku();
        break;
    case 5:
        prosesPermintaanPinjaman();
        break;
    case 6:
        prosesPengembalian();
        break;
    case 0:
        break;
    default:
        cout << "Opsi " << opsi << " tidak ditemukan!" << endl;
        cout << endl;
        break;
    }

    if (opsi != 0)
    {
        masukSebagaiPegawai();
    }
}

void masukSebagaiPengunjung()
{
    outputDashboardPengunjung();

    int opsi = inputOpsi();

    switch (opsi)
    {
    case 1:
        tampilkanBuku(false);
        break;
    case 2:
        tampilkanRiwayat();
        break;
    case 0:
        break;
    default:
        cout << "Opsi " << opsi << " tidak ditemukan!" << endl;
        cout << endl;
        break;
    }

    if (opsi != 0)
    {
        masukSebagaiPengunjung();
    }
}

void perpustakaan()
{
    muatDataBukuKeLinkedList();
    muatDataPinjamanKeQueue();
    muatDataRiwayatKeLinkedList();
    muatDataPengembalianKeStack();

    int role = 0;

    do
    {
        outputSelamatDatang();
        role = inputOpsi();

        switch (role)
        {
        case 1:
            masukSebagaiPegawai();
            break;
        case 2:
            masukSebagaiPengunjung();
            break;
        case 0:
            simpanDanKeluar();
            break;
        default:
            cout << "Opsi " << role << " tidak ditemukan!" << endl;
            cout << endl;
            break;
        }
    } while (role != 0);
}

int main()
{
    perpustakaan();
    return 0;
}

/* End Fungsi Utama */