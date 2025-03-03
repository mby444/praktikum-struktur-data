#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

#define FILE_DATA "database.txt"

using namespace std;

struct Buku
{
    string id;
    string judul;
    string penulis;
    string tahun;
    string genre;
    bool tersedia;
    Buku *next;
    Buku *prev;
};

Buku *head = NULL;
Buku *tail = NULL;

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
    cout << "1. Masuk sebagai pegawai" << endl;
    cout << "2. Masuk sebagai pengunjung" << endl;
    cout << "0. Keluar" << endl;
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
    cout << "0. Simpan dan Keluar" << endl;
    cout << endl;
}

void outputDaftarBukuAdmin()
{
    cout << "1. Selanjutnya" << endl;
    cout << "2. Sebelumnya" << endl;
    cout << "0. Keluar" << endl;
    cout << endl;
}

void outputDaftarBukuPengunjung()
{
    cout << "1. Selanjutnya" << endl;
    cout << "2. Sebelumnya" << endl;
    cout << "3. Pinjam" << endl;
    cout << "0. Keluar" << endl;
    cout << endl;
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

void cetakLinkedList()
{
    Buku *bantu = head;

    do
    {
        cout << "ID: " << bantu->id << endl;
        cout << "Judul: " << bantu->judul << endl;
        cout << "Penulis: " << bantu->penulis << endl;
        cout << "Tahun: " << bantu->tahun << endl;
        cout << "Genre: " << bantu->genre << endl;
        cout << "Tersedia: " << (bantu->tersedia ? "Ya" : "Tidak") << endl;
        cout << endl;
        bantu = bantu->next;
    } while (bantu != head);

    cout << "===================================" << endl;
    bantu = tail;

    do
    {
        cout << "ID: " << bantu->id << endl;
        cout << "Judul: " << bantu->judul << endl;
        cout << "Penulis: " << bantu->penulis << endl;
        cout << "Tahun: " << bantu->tahun << endl;
        cout << "Genre: " << bantu->genre << endl;
        cout << "Tersedia: " << (bantu->tersedia ? "Ya" : "Tidak") << endl;
        cout << endl;
        bantu = bantu->prev;
    } while (bantu != tail);

    cout << "===================================" << endl;

    cout << "(Head)" << endl;
    cout << "ID: " << head->id << endl;
    cout << "Judul: " << head->judul << endl;
    cout << "Penulis: " << head->penulis << endl;
    cout << "Tahun: " << head->tahun << endl;
    cout << "Genre: " << head->genre << endl;
    cout << "Tersedia: " << (head->tersedia ? "Ya" : "Tidak") << endl;
    cout << endl;

    cout << "===================================" << endl;

    cout << "(Tail)" << endl;
    cout << "ID: " << tail->id << endl;
    cout << "Judul: " << tail->judul << endl;
    cout << "Penulis: " << tail->penulis << endl;
    cout << "Tahun: " << tail->tahun << endl;
    cout << "Genre: " << tail->genre << endl;
    cout << "Tersedia: " << (tail->tersedia ? "Ya" : "Tidak") << endl;
    cout << endl;
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

            if (lastChar == '9')
            {
                tempId[tempId.length() - 1] = lastChar;
                tempId += "0";
            }
            else
            {
                tempId[tempId.length() - 1] = lastChar;
            }
        }
        bantu = bantu->next;
    } while (bantu != head);

    return tempId;
}

string buatIdBuku(string &judul, string &tahun)
{
    int firstCharInt = (int)judul[0];
    int lastCharInt = (int)judul[judul.length() - 1];
    int yearDigitSum = 0;
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

    if (tempId[tempId.length() - 1] == '9')
    {
        tempId += "0";
    }

    return tetapkanIdBuku(tempId);
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

void hapusDariLinkedList(string id)
{
    Buku *bantu = head;

    do
    {
        if (bantu->id == id)
        {
            if (bantu == head)
            {
                head = bantu->next;
            }
            else if (bantu == tail)
            {
                tail = bantu->prev;
            }

            bantu->prev->next = bantu->next;
            bantu->next->prev = bantu->prev;
            delete bantu;
            break;
        }
        bantu = bantu->next;
    } while (bantu != head);
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
            buku->tahun = token;
            break;
        case 4:
            buku->genre = token;
            break;
        }
        line.erase(0, delimiterIndex + 1);
        i++;
    }

    buku->tersedia = line == "1";
    buku->next = buku;
    buku->prev = buku;

    return buku;
}

Buku *buatBukuBaru()
{
    Buku *buku = new Buku;

    buku->judul = inputData("Judul");
    buku->penulis = inputData("Penulis");
    buku->tahun = inputData("Tahun");
    buku->genre = inputData("Genre");
    buku->id = buatIdBuku(buku->judul, buku->tahun);
    buku->tersedia = true;
    buku->next = buku;
    buku->prev = buku;

    return buku;
}

/* Fungsi Utama */

void tampilkanBuku(bool sebagaiPegawai = true)
{
    if (head == NULL)
    {
        cout << "Data buku masih kosong!" << endl;
        return;
    }

    Buku *tampil = head;
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
            }
            else
            {
                cout << "Fitur belum tersedia!" << endl;
            }
            break;
        case 0:
            cout << "Terima kasih telah mengunjungi perpustakaan!" << endl;
            break;

        default:
            cout << "Opsi" << opsi << " tidak ditemukan!" << endl;
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
            buku->tahun = inputData("Tahun baru");
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

    lihatBuku(buku);

    string konfirmasiInput = inputData("Apakah Anda yakin ingin menghapus buku ini? (y/N)");

    if (konfirmasiInput == "y" || konfirmasiInput == "Y")
    {
        string idDihapus = buku->id;
        hapusDariLinkedList(idDicari);
        cout << "Buku dengan ID " << idDihapus << " berhasil dihapus!" << endl;
    }
    else
    {
        cout << "Buku tidak jadi dihapus!" << endl;
    }
}

void muatDataKeLinkedList()
{
    ifstream file(FILE_DATA);
    string line;
    bool tersedia;

    // Tambah semua data buku ke linked list
    while (getline(file, line))
    {
        Buku *buku = lineKeBuku(line);
        tambahKeLinkedList(buku);
    }
}

void simpanDataKeFile()
{
    if (head == NULL)
    {
        return;
    }

    ofstream file(FILE_DATA);
    Buku *bantu = head;

    do
    {
        file << bantu->id << "|" << bantu->judul << "|" << bantu->penulis << "|" << bantu->tahun << "|" << bantu->genre << "|" << (bantu->tersedia ? "1" : "0") << endl;
        bantu = bantu->next;
    } while (bantu != head);
}

void simpanDanKeluar()
{
    simpanDataKeFile();
    cout << "Data berhasil disimpan!" << endl;
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
    case 0:
        simpanDanKeluar();
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
    tampilkanBuku(false);
}

void perpustakaan()
{
    muatDataKeLinkedList();
    outputSelamatDatang();

    int role = inputOpsi();

    switch (role)
    {
    case 1:
        masukSebagaiPegawai();
        break;
    case 2:
        masukSebagaiPengunjung();
        break;
    case 0:
        cout << "Terima kasih telah mengunjungi perpustakaan!" << endl;
        break;
    default:
        cout << "Opsi " << role << " tidak ditemukan!" << endl;
        cout << endl;
        perpustakaan();
        break;
    }
}

int main()
{
    perpustakaan();
    return 0;
}

/* End Fungsi Utama */