#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Input.H>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <ctime>

using namespace std;

#define FILE_DATA "database/buku.txt"
#define FILE_PINJAMAN "database/pinjaman.txt"

// Struktur Data Buku (Circular Doubly Linked List)
struct Buku
{
    string id, judul, penulis, tahun, genre;
    bool tersedia;
    Buku *next;
    Buku *prev;
};

struct Pinjaman
{
    string id, judul, nama, nim;
    time_t tanggal;
    Pinjaman *next;
};

// Head dari Linked List
Buku *head = NULL;
Buku *tail = NULL;
Buku *tampilAdmin = NULL;
Buku *tampilPengunjung = NULL;

// Elemen tampilan buku
Buku *bukuDiubah;
Buku *bukuDihapus;
Fl_Input *idDicariInput;

// Elemen tampilan buku untuk admin
Fl_Box *judulBoxAdmin, *idBoxAdmin, *penulisBoxAdmin, *tahunBoxAdmin, *genreBoxAdmin, *tersediaBoxAdmin;
Fl_Button *btnSelanjutnyaAdmin, *btnSebelumnyaAdmin, *btnKembaliAdmin;
Fl_Input *judulInputAdmin, *penulisInputAdmin, *tahunInputAdmin, *genreInputAdmin;

// Elemen tampilan buku untuk pengunjung
Fl_Box *judulBoxPengunjung, *idBoxPengunjung, *penulisBoxPengunjung, *tahunBoxPengunjung, *genreBoxPengunjung, *tersediaBoxPengunjung;
Fl_Button *btnPinjamPengunjung, *btnSelanjutnyaPengunjung, *btnSebelumnyaPengunjung, *btnKembaliPengunjung;
Fl_Input *judulInputPengunjung, *penulisInputPengunjung, *tahunInputPengunjung, *genreInputPengunjung, *namaInputPengunjung, *nimInputPengunjung;

// Head dari Queue
Pinjaman *headPinjaman = NULL;
Pinjaman *tailPinjaman = NULL;

void cetakLinkedList()
{
    if (head == NULL)
    {
        cout << "Data buku masih kosong!" << endl;
        return;
    }

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

void cetakQueue()
{
    if (headPinjaman == NULL)
    {
        cout << "Data pinjaman masih kosong!" << endl;
        return;
    }

    Pinjaman *bantu = headPinjaman;

    do
    {
        cout << "ID: " << bantu->id << endl;
        cout << "Judul: " << bantu->judul << endl;
        cout << "Nama: " << bantu->nama << endl;
        cout << "NIM: " << bantu->nim << endl;
        cout << "Tanggal: " << bantu->tanggal << endl;
        cout << endl;
        bantu = bantu->next;
    } while (bantu != NULL);

    cout << "===================================" << endl;

    cout << "(Head)" << endl;
    cout << "ID: " << headPinjaman->id << endl;
    cout << "Judul: " << headPinjaman->judul << endl;
    cout << "Nama: " << headPinjaman->nama << endl;
    cout << "NIM: " << headPinjaman->nim << endl;
    cout << "Tanggal: " << headPinjaman->tanggal << endl;
    cout << endl;

    cout << "===================================" << endl;

    cout << "(Tail)" << endl;
    cout << "ID: " << tailPinjaman->id << endl;
    cout << "Judul: " << tailPinjaman->judul << endl;
    cout << "Nama: " << tailPinjaman->nama << endl;
    cout << "NIM: " << tailPinjaman->nim << endl;
    cout << "Tanggal: " << tailPinjaman->tanggal << endl;
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

string hapusSpasi(string input)
{
    string output = "";
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] != ' ')
        {
            output += input[i];
        }
    }
    return output;
}

bool cekStringKosong(string input)
{
    return hapusSpasi(input).empty();
}

string nomorPinjaman(string idPinjaman)
{
    int iht = indexHurufTerakhir(idPinjaman);
    int len = idPinjaman.length();
    string nomor = idPinjaman.substr(iht + 1, len - (iht + 1));
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

// **Fungsi Menampilkan Satu Buku**
void lihatBukuAdmin(Buku *tampil = NULL)
{
    cout << "lihatBukuAdmin 1" << endl;
    if (!tampil)
    {
        cout << "lihatBukuAdmin 2" << endl;
        judulBoxAdmin->label("Judul: -");
        idBoxAdmin->label("ID: -");
        penulisBoxAdmin->label("Penulis: -");
        tahunBoxAdmin->label("Tahun: -");
        genreBoxAdmin->label("Genre: -");
        tersediaBoxAdmin->label("Ketersediaan: -");
        return;
    }

    cout << "lihatBukuAdmin 3" << endl;

    string judulLabel = ("Judul: " + tampil->judul);
    string idLabel = ("ID: " + tampil->id);
    string penulisLabel = ("Penulis: " + tampil->penulis);
    string tahunLabel = ("Tahun: " + tampil->tahun);
    string genreLabel = ("Genre: " + tampil->genre);
    string tersediaLabel = (tampil->tersedia) ? "Ketersediaan: Tersedia" : "Ketersediaan: Tidak Tersedia";

    cout << "lihatBukuAdmin 4" << endl;

    judulBoxAdmin->copy_label(judulLabel.c_str());
    idBoxAdmin->copy_label(idLabel.c_str());
    penulisBoxAdmin->copy_label(penulisLabel.c_str());
    tahunBoxAdmin->copy_label(tahunLabel.c_str());
    genreBoxAdmin->copy_label(genreLabel.c_str());
    tersediaBoxAdmin->copy_label(tersediaLabel.c_str());

    cout << "lihatBukuAdmin 5" << endl;
}

// **Fungsi Menampilkan Satu Buku**
void lihatBukuPengunjung(Buku *tampil = NULL)
{
    if (!tampil)
    {
        judulBoxPengunjung->label("Judul: -");
        idBoxPengunjung->label("ID: -");
        penulisBoxPengunjung->label("Penulis: -");
        tahunBoxPengunjung->label("Tahun: -");
        genreBoxPengunjung->label("Genre: -");
        tersediaBoxPengunjung->label("Ketersediaan: -");
        return;
    }

    string judulLabel = ("Judul: " + tampil->judul);
    string idLabel = ("ID: " + tampil->id);
    string penulisLabel = ("Penulis: " + tampil->penulis);
    string tahunLabel = ("Tahun: " + tampil->tahun);
    string genreLabel = ("Genre: " + tampil->genre);
    string tersediaLabel = (tampil->tersedia) ? "Ketersediaan: Tersedia" : "Ketersediaan: Tidak Tersedia";

    judulBoxPengunjung->copy_label(judulLabel.c_str());
    idBoxPengunjung->copy_label(idLabel.c_str());
    penulisBoxPengunjung->copy_label(penulisLabel.c_str());
    tahunBoxPengunjung->copy_label(tahunLabel.c_str());
    genreBoxPengunjung->copy_label(genreLabel.c_str());
    tersediaBoxPengunjung->copy_label(tersediaLabel.c_str());
}

void lihatPinjamanPengunjung(Pinjaman *tampil = NULL)
{
    if (!tampil)
    {
        judulBoxPengunjung->label("Judul: -");
        idBoxPengunjung->label("ID: -");
        penulisBoxPengunjung->label("Penulis: -");
        tahunBoxPengunjung->label("Tahun: -");
        genreBoxPengunjung->label("Genre: -");
        tersediaBoxPengunjung->label("Ketersediaan: -");
        return;
    }

    string judulLabel = ("Judul: " + tampil->judul);
    string idLabel = ("ID: " + tampil->id);
    string penulisLabel = ("Nama: " + tampil->nama);
    string tahunLabel = ("NIM: " + tampil->nim);
    string genreLabel = ("Tanggal: " + fullTanggalString(tampil->tanggal));

    judulBoxPengunjung->copy_label(judulLabel.c_str());
    idBoxPengunjung->copy_label(idLabel.c_str());
    penulisBoxPengunjung->copy_label(penulisLabel.c_str());
    tahunBoxPengunjung->copy_label(tahunLabel.c_str());
    genreBoxPengunjung->copy_label(genreLabel.c_str());
}

// **Navigasi ke Buku Berikutnya**
void selanjutnyaAdmin(Fl_Widget *)
{
    cout << "Selanjutnya 1" << endl;
    if (tampilAdmin)
    {
        cout << tampilAdmin->judul << endl;
        tampilAdmin = tampilAdmin->next;
        cout << "Selanjutnya 2" << endl;
    }
    cout << "Selanjutnya 3" << endl;
    lihatBukuAdmin(tampilAdmin);
}

// **Navigasi ke Buku Sebelumnya**
void sebelumnyaAdmin(Fl_Widget *)
{
    if (tampilAdmin)
    {
        tampilAdmin = tampilAdmin->prev;
    }
    lihatBukuAdmin(tampilAdmin);
}

// **Navigasi ke Buku Berikutnya**
void selanjutnyaPengunjung(Fl_Widget *)
{
    if (tampilPengunjung)
    {
        tampilPengunjung = tampilPengunjung->next;
    }
    lihatBukuPengunjung(tampilPengunjung);
}

// **Navigasi ke Buku Sebelumnya**
void sebelumnyaPengunjung(Fl_Widget *)
{
    if (tampilPengunjung)
    {
        tampilPengunjung = tampilPengunjung->prev;
    }
    lihatBukuPengunjung(tampilPengunjung);
}

void pinjam(Fl_Widget *w, void *win)
{
    // if (cekStringKosong(namaInputPengunjung->value()) || cekStringKosong(nimInputPengunjung->value()))
    // {
    //     showPopup("Nama dan NIM tidak boleh kosong!");
    //     return;
    // }
}

// **Peminjaman Buku (Khusus Pengunjung)**
void pinjamBuku(Fl_Widget *)
{
    Fl_Window *winPinjam = new Fl_Window(400, 200, "Pinjam Buku");
    namaInputPengunjung = new Fl_Input(150, 50, 200, 30, "Nama");
    nimInputPengunjung = new Fl_Input(150, 100, 200, 30, "NIM");
    Fl_Button *btnPinjam = new Fl_Button(150, 150, 100, 30, "Pinjam");

    btnPinjam->callback(pinjam, winPinjam);
    winPinjam->end();
    winPinjam->show();
}

// **Menutup Tampilan Buku**
void kembali(Fl_Widget *w, void *win)
{
    ((Fl_Window *)win)->hide();
}

void keluar(Fl_Widget *)
{
    exit(0);
}

void showPopup(string message)
{
    Fl_Window *win = new Fl_Window(300, 200, "Perpustakaan");
    Fl_Box *box = new Fl_Box(50, 50, 200, 30, message.c_str());
    Fl_Button *btn = new Fl_Button(100, 100, 100, 30, "OK");

    btn->callback(kembali, win);
    win->end();
    win->show();
}

// **Tampilan Pengunjung**
void showBukuPengunjung(Fl_Widget *)
{
    Fl_Window *winPengunjung = new Fl_Window(400, 300, "Lihat Buku");

    judulBoxPengunjung = new Fl_Box(50, 20, 300, 30);
    idBoxPengunjung = new Fl_Box(50, 50, 300, 30);
    penulisBoxPengunjung = new Fl_Box(50, 80, 300, 30);
    tahunBoxPengunjung = new Fl_Box(50, 110, 300, 30);
    genreBoxPengunjung = new Fl_Box(50, 140, 300, 30);
    tersediaBoxPengunjung = new Fl_Box(50, 170, 300, 30);

    btnSebelumnyaPengunjung = new Fl_Button(50, 210, 100, 30, "Sebelumnya");
    btnSelanjutnyaPengunjung = new Fl_Button(160, 210, 100, 30, "Selanjutnya");
    btnPinjamPengunjung = new Fl_Button(50, 250, 100, 30, "Pinjam");
    btnKembaliPengunjung = new Fl_Button(160, 250, 100, 30, "Kembali");

    btnSebelumnyaPengunjung->callback(sebelumnyaPengunjung);
    btnSelanjutnyaPengunjung->callback(selanjutnyaPengunjung);
    btnPinjamPengunjung->callback(pinjamBuku);
    btnKembaliPengunjung->callback(kembali, winPengunjung);

    winPengunjung->end();
    winPengunjung->show();

    tampilPengunjung = head;
    lihatBukuPengunjung(tampilPengunjung);
}

// **Tampilan Pegawai**
void showBukuAdmin(Fl_Widget *)
{
    Fl_Window *winPegawai = new Fl_Window(400, 300, "Lihat Buku");

    if (head == NULL)
    {
        showPopup("Data buku masih kosong!");
        return;
    }

    judulBoxAdmin = new Fl_Box(50, 20, 300, 30);
    idBoxAdmin = new Fl_Box(50, 50, 300, 30);
    penulisBoxAdmin = new Fl_Box(50, 80, 300, 30);
    tahunBoxAdmin = new Fl_Box(50, 110, 300, 30);
    genreBoxAdmin = new Fl_Box(50, 140, 300, 30);
    tersediaBoxAdmin = new Fl_Box(50, 170, 300, 30);

    btnSebelumnyaAdmin = new Fl_Button(50, 210, 100, 30, "Sebelumnya");
    btnSelanjutnyaAdmin = new Fl_Button(160, 210, 100, 30, "Selanjutnya");
    btnKembaliAdmin = new Fl_Button(160, 250, 100, 30, "Kembali");

    btnSebelumnyaAdmin->callback(sebelumnyaAdmin);
    btnSelanjutnyaAdmin->callback(selanjutnyaAdmin);
    btnKembaliAdmin->callback(kembali, winPegawai);

    winPegawai->end();
    winPegawai->show();

    tampilAdmin = head;
    lihatBukuAdmin(tampilAdmin);
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
        Pinjaman *pinjaman = lineKeQueue(line);
        lihatPinjaman(pinjaman);
        enqueuePinjaman(pinjaman);
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

void simpanDataKeFile()
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

void simpanDanKeluar(Fl_Widget *)
{
    simpanDataKeFile();
    kosongkanLinkedList();
    exit(0);
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

string buatIdBuku(string judul, string tahun)
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

Buku *cariBukuDipinjam(string idPinjaman)
{
    string id = idPinjaman.substr(0, indexHurufPertama(idPinjaman));
    Buku *buku = cariBuku(id);
    return buku;
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

void hapusDariLinkedList2(Buku *buku)
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

Pinjaman *lineKeQueue(string line)
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

Buku *buatBukuBaru()
{
    string judul = judulInputAdmin->value();
    string penulis = penulisInputAdmin->value();
    string tahun = tahunInputAdmin->value();
    string genre = genreInputAdmin->value();

    if (cekStringKosong(judul) || cekStringKosong(penulis) || cekStringKosong(tahun) || cekStringKosong(genre))
    {
        showPopup("Semua data harus diisi!");
        return NULL;
    }

    if (!cekNumerik(tahun))
    {
        showPopup("Tahun harus berupa angka!");
        return NULL;
    }

    Buku *buku = new Buku;
    buku->judul = judul;
    buku->penulis = penulis;
    buku->tahun = tahun;
    buku->genre = genre;
    buku->id = buatIdBuku(judul, tahun);
    buku->tersedia = true;
    buku->next = buku;
    buku->prev = buku;

    return buku;
}

Pinjaman *buatPinjamanBaru(Buku *buku)
{
}

void tambahkan(Fl_Widget *, void *win)
{
    Buku *bukuBaru = buatBukuBaru();
    if (bukuBaru == NULL)
    {
        return;
    }
    tambahKeLinkedList(bukuBaru);
    ((Fl_Window *)win)->hide();
}

void ubah(Fl_Widget *, void *win)
{
    if (bukuDiubah == NULL)
    {
        showPopup("Buku tidak ditemukan!");
        return;
    }

    if (cekStringKosong(judulInputAdmin->value()) || cekStringKosong(penulisInputAdmin->value()) || cekStringKosong(tahunInputAdmin->value()) || cekStringKosong(genreInputAdmin->value()))
    {
        showPopup("Data tidak boleh kosong!");
        return;
    }

    if (!cekNumerik(tahunInputAdmin->value()))
    {
        showPopup("Tahun harus berupa angka!");
        return;
    }

    if (bukuDiubah->judul != judulInputAdmin->value() || bukuDiubah->tahun != tahunInputAdmin->value())
    {
        bukuDiubah->id = buatIdBuku(judulInputAdmin->value(), tahunInputAdmin->value());
    }

    bukuDiubah->judul = judulInputAdmin->value();
    bukuDiubah->penulis = penulisInputAdmin->value();
    bukuDiubah->tahun = tahunInputAdmin->value();
    bukuDiubah->genre = genreInputAdmin->value();
    ((Fl_Window *)win)->hide();
}

void hapus(Fl_Widget *, void *win)
{
    if (bukuDihapus == NULL)
    {
        showPopup("Buku tidak ditemukan!");
        return;
    }

    if (tampilAdmin == bukuDihapus)
    {
        tampilAdmin = tampilAdmin->next;
    }

    if (tampilPengunjung == bukuDihapus)
    {
        tampilPengunjung = tampilPengunjung->next;
    }

    hapusDariLinkedList2(bukuDihapus);

    if (head == NULL)
    {
        tampilAdmin = NULL;
        tampilPengunjung = NULL;
    }

    ((Fl_Window *)win)->hide();
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

void showTambahBuku(Fl_Widget *)
{
    Fl_Window *winTambah = new Fl_Window(300, 300, "Tambah Buku");

    Fl_Box *judulLabel = new Fl_Box(50, 20, 200, 30, "Judul");
    Fl_Box *penulisLabel = new Fl_Box(50, 70, 200, 30, "Penulis");
    Fl_Box *tahunLabel = new Fl_Box(50, 120, 200, 30, "Tahun");
    Fl_Box *genreLabel = new Fl_Box(50, 170, 200, 30, "Genre");

    judulInputAdmin = new Fl_Input(50, 50, 200, 30);
    penulisInputAdmin = new Fl_Input(50, 100, 200, 30);
    tahunInputAdmin = new Fl_Input(50, 150, 200, 30);
    genreInputAdmin = new Fl_Input(50, 200, 200, 30);

    Fl_Button *btnTambahkan = new Fl_Button(50, 250, 100, 30, "Simpan");
    Fl_Button *btnBatal = new Fl_Button(160, 250, 100, 30, "Kembali");

    btnTambahkan->callback(tambahkan, winTambah);
    btnBatal->callback(kembali, winTambah);

    winTambah->end();
    winTambah->show();
}

void showUbahBuku(Fl_Widget *, void *win)
{
    Fl_Window *winUbah = new Fl_Window(300, 300, "Ubah Buku");
    bukuDiubah = cariBuku(idDicariInput->value());

    if (bukuDiubah == NULL)
    {
        showPopup("Buku tidak ditemukan!");
        return;
    }

    Fl_Box *judulLabel = new Fl_Box(50, 20, 200, 30, "Judul");
    Fl_Box *penulisLabel = new Fl_Box(50, 70, 200, 30, "Penulis");
    Fl_Box *tahunLabel = new Fl_Box(50, 120, 200, 30, "Tahun");
    Fl_Box *genreLabel = new Fl_Box(50, 170, 200, 30, "Genre");

    judulInputAdmin = new Fl_Input(50, 50, 200, 30);
    penulisInputAdmin = new Fl_Input(50, 100, 200, 30);
    tahunInputAdmin = new Fl_Input(50, 150, 200, 30);
    genreInputAdmin = new Fl_Input(50, 200, 200, 30);

    judulInputAdmin->value(bukuDiubah->judul.c_str());
    penulisInputAdmin->value(bukuDiubah->penulis.c_str());
    tahunInputAdmin->value(bukuDiubah->tahun.c_str());
    genreInputAdmin->value(bukuDiubah->genre.c_str());

    Fl_Button *btnUbah = new Fl_Button(50, 250, 100, 30, "Simpan");
    Fl_Button *btnBatal = new Fl_Button(160, 250, 100, 30, "Batal");

    btnUbah->callback(ubah, winUbah);
    btnBatal->callback(kembali, winUbah);

    winUbah->end();
    winUbah->show();
    ((Fl_Window *)win)->hide();
}

void showHapusBuku(Fl_Widget *, void *win)
{
    Fl_Window *winUbah = new Fl_Window(300, 300, "Hapus Buku");
    bukuDihapus = cariBuku(idDicariInput->value());

    if (bukuDihapus == NULL)
    {
        showPopup("Buku tidak ditemukan!");
        return;
    }

    judulBoxAdmin = new Fl_Box(50, 20, 300, 30);
    idBoxAdmin = new Fl_Box(50, 50, 300, 30);
    penulisBoxAdmin = new Fl_Box(50, 80, 300, 30);
    tahunBoxAdmin = new Fl_Box(50, 110, 300, 30);
    genreBoxAdmin = new Fl_Box(50, 140, 300, 30);
    tersediaBoxAdmin = new Fl_Box(50, 170, 300, 30);

    Fl_Button *btnUbah = new Fl_Button(50, 250, 100, 30, "Hapus");
    Fl_Button *btnBatal = new Fl_Button(160, 250, 100, 30, "Batal");

    btnUbah->callback(hapus, winUbah);
    btnBatal->callback(kembali, winUbah);

    winUbah->end();
    winUbah->show();
    ((Fl_Window *)win)->hide();

    lihatBukuAdmin(bukuDihapus);
}

void showInputIdUbahBuku(Fl_Widget *)
{
    Fl_Window *winInputId = new Fl_Window(300, 200, "Ubah Buku");

    if (head == NULL)
    {
        showPopup("Data buku masih kosong!");
        return;
    }

    Fl_Box *box = new Fl_Box(50, 50, 200, 30, "Masukkan ID Buku");
    idDicariInput = new Fl_Input(50, 100, 200, 30);
    Fl_Button *btnCari = new Fl_Button(50, 150, 100, 30, "Cari");

    btnCari->callback(showUbahBuku, winInputId);

    winInputId->end();
    winInputId->show();
}

void showInputIdHapusBuku(Fl_Widget *)
{
    Fl_Window *winInputId = new Fl_Window(300, 200, "Hapus Buku");

    if (head == NULL)
    {
        showPopup("Data buku masih kosong!");
        return;
    }

    Fl_Box *box = new Fl_Box(50, 50, 200, 30, "Masukkan ID Buku");
    idDicariInput = new Fl_Input(50, 100, 200, 30);
    Fl_Button *btnCari = new Fl_Button(50, 150, 100, 30, "Cari");

    btnCari->callback(showHapusBuku, winInputId);

    winInputId->end();
    winInputId->show();
}

void showProsesPermintaanPinjaman(Fl_Widget *, void *win)
{
}

// **Tampilan Pegawai**
void showAdmin(Fl_Widget *)
{
    Fl_Window *winAdmin = new Fl_Window(300, 400, "Dashboard Admin");
    Fl_Button *btnTambah = new Fl_Button(50, 50, 200, 40, "Tambah Buku");
    Fl_Button *btnLihat = new Fl_Button(50, 100, 200, 40, "Lihat Buku");
    Fl_Button *btnUbah = new Fl_Button(50, 150, 200, 40, "Ubah Buku");
    Fl_Button *btnHapus = new Fl_Button(50, 200, 200, 40, "Hapus Buku");
    Fl_Button *btnKembali = new Fl_Button(50, 250, 200, 40, "Kembali");

    btnTambah->callback(showTambahBuku);
    btnLihat->callback(showBukuAdmin);
    btnUbah->callback(showInputIdUbahBuku);
    btnHapus->callback(showInputIdHapusBuku);
    btnKembali->callback(kembali, winAdmin);
    winAdmin->end();
    winAdmin->show();
}

// **Tampilan Utama**
int main()
{
    muatDataBukuKeLinkedList();
    muatDataPinjamanKeQueue();

    Fl_Window *win = new Fl_Window(300, 400, "Perpustakaan");
    Fl_Button *btnAdmin = new Fl_Button(50, 50, 200, 40, "Masuk Sebagai Pegawai");
    Fl_Button *btnPengunjung = new Fl_Button(50, 110, 200, 40, "Masuk Sebagai Pengunjung");
    Fl_Button *btnSimpanDanKeluar = new Fl_Button(50, 170, 200, 40, "Simpan dan Keluar");

    btnAdmin->callback(showAdmin);
    btnPengunjung->callback(showBukuPengunjung);
    btnSimpanDanKeluar->callback(simpanDanKeluar);

    win->end();
    win->show();
    return Fl::run();
}

// Referensi yang versi CLI

// #include <iostream>
// #include <string>
// #include <fstream>
// #include <cmath>
// #include <ctime>

// #define FILE_DATA "database/buku.txt"
// #define FILE_PINJAMAN "database/pinjaman.txt"

// using namespace std;

// struct Buku
// {
//     string id, judul, penulis, genre;
//     int tahun;
//     bool tersedia;
//     Buku *next, *prev;
// };

// struct Pinjaman
// {
//     string id, judul, nama, nim;
//     time_t tanggal;
//     Pinjaman *next;
// };

// Buku *head = NULL;
// Buku *tail = NULL;

// Pinjaman *headPinjaman = NULL;
// Pinjaman *tailPinjaman = NULL;

// void outputHeaderPerpustakaan()
// {
//     cout << "=======================================" << endl;
//     cout << "===          Perpustakaan           ===" << endl;
//     cout << "=======================================" << endl;
// }

// void outputSelamatDatang()
// {
//     cout << "=======================================" << endl;
//     cout << "=== Selamat datang di Perpustakaan! ===" << endl;
//     cout << "=======================================" << endl;
//     cout << "1. Masuk Sebagai Pegawai" << endl;
//     cout << "2. Masuk Sebagai Pengunjung" << endl;
//     cout << "0. Simpan dan Keluar" << endl;
//     cout << endl;
// }

// void outputDasboardAdmin()
// {
//     cout << "=======================================" << endl;
//     cout << "===          Dashboard Admin        ===" << endl;
//     cout << "=======================================" << endl;
//     cout << "1. Tambah Buku" << endl;
//     cout << "2. Tampilkan Buku" << endl;
//     cout << "3. Ubah Buku" << endl;
//     cout << "4. Hapus Buku" << endl;
//     cout << "5. Permintaan Pinjaman" << endl;
//     cout << "0. Kembali" << endl;
//     cout << endl;
// }

// void outputDaftarBukuAdmin()
// {
//     cout << "1. Selanjutnya" << endl;
//     cout << "2. Sebelumnya" << endl;
//     cout << "0. Kembali" << endl;
//     cout << endl;
// }

// void outputDaftarBukuPengunjung()
// {
//     cout << "1. Selanjutnya" << endl;
//     cout << "2. Sebelumnya" << endl;
//     cout << "3. Pinjam" << endl;
//     cout << "0. Kembali" << endl;
//     cout << endl;
// }

// void outputTambahBuku()
// {
//     cout << "=======================================" << endl;
//     cout << "===          Tambah Buku            ===" << endl;
//     cout << "=======================================" << endl;
// }

// void outputUbahBuku()
// {
//     cout << "=======================================" << endl;
//     cout << "===          Ubah Buku              ===" << endl;
//     cout << "=======================================" << endl;
//     cout << "1. Judul" << endl;
//     cout << "2. Penulis" << endl;
//     cout << "3. Tahun" << endl;
//     cout << "4. Genre" << endl;
//     cout << "0. Kembali" << endl;
//     cout << endl;
// }

// void outputHeaderPermintaanPinjaman(string nomorPinjaman)
// {
//     cout << "=======================================" << endl;
//     cout << "         Permintaan Ke-" << nomorPinjaman << "      " << endl;
//     cout << "=======================================" << endl;
// }

// void outputMenuPermintaanPinjam()
// {
//     cout << "1. Setujui" << endl;
//     cout << "2. Tolak" << endl;
//     cout << "0. Kembali" << endl;
//     cout << endl;
// }

// bool cekNumerik(string input)
// {
//     for (int i = 0; i < input.length(); i++)
//     {
//         if (input[i] > '9' || input[i] < '0')
//         {
//             cout << "input[i]: " << input[i] << endl;
//             return false;
//         }
//     }
//     return true;
// }

// int tahunSekarang()
// {
//     time_t now = time(0);
//     tm *ltm = localtime(&now);
//     return (1900 + ltm->tm_year);
// }

// string namaBulan(int bulan)
// {
//     string namaBulan[12] = {"Januari", "Februari", "Maret", "April", "Mei", "Juni",
//                             "Juli", "Agustus", "September", "Oktober", "November", "Desember"};
//     return namaBulan[bulan];
// }

// string fullTanggalString(time_t tanggal)
// {
//     tm *ltm = localtime(&tanggal);
//     string tanggalStr = to_string(ltm->tm_mday);
//     string bulanStr = to_string(1 + ltm->tm_mon);
//     string tahunStr = to_string(1900 + ltm->tm_year);
//     string jamStr = to_string(ltm->tm_hour);
//     string menitStr = to_string(ltm->tm_min);

//     if (tanggalStr.length() == 1)
//     {
//         tanggalStr = "0" + tanggalStr;
//     }
//     if (bulanStr.length() == 1)
//     {
//         bulanStr = "0" + bulanStr;
//     }
//     if (jamStr.length() == 1)
//     {
//         jamStr = "0" + jamStr;
//     }
//     if (menitStr.length() == 1)
//     {
//         menitStr = "0" + menitStr;
//     }

//     return (tanggalStr + " " + namaBulan(stoi(bulanStr) - 1) + " " + tahunStr + " " + jamStr + ":" + menitStr);
//     // Contoh output: 8 Maret 2025 08:50
// }

// void cetakLinkedList()
// {
//     if (head == NULL)
//     {
//         cout << "Data buku masih kosong!" << endl;
//         return;
//     }

//     Buku *bantu = head;

//     do
//     {
//         cout << "ID: " << bantu->id << endl;
//         cout << "Judul: " << bantu->judul << endl;
//         cout << "Penulis: " << bantu->penulis << endl;
//         cout << "Tahun: " << bantu->tahun << endl;
//         cout << "Genre: " << bantu->genre << endl;
//         cout << "Tersedia: " << (bantu->tersedia ? "Ya" : "Tidak") << endl;
//         cout << endl;
//         bantu = bantu->next;
//     } while (bantu != head);

//     cout << "===================================" << endl;
//     bantu = tail;

//     do
//     {
//         cout << "ID: " << bantu->id << endl;
//         cout << "Judul: " << bantu->judul << endl;
//         cout << "Penulis: " << bantu->penulis << endl;
//         cout << "Tahun: " << bantu->tahun << endl;
//         cout << "Genre: " << bantu->genre << endl;
//         cout << "Tersedia: " << (bantu->tersedia ? "Ya" : "Tidak") << endl;
//         cout << endl;
//         bantu = bantu->prev;
//     } while (bantu != tail);

//     cout << "===================================" << endl;

//     cout << "(Head)" << endl;
//     cout << "ID: " << head->id << endl;
//     cout << "Judul: " << head->judul << endl;
//     cout << "Penulis: " << head->penulis << endl;
//     cout << "Tahun: " << head->tahun << endl;
//     cout << "Genre: " << head->genre << endl;
//     cout << "Tersedia: " << (head->tersedia ? "Ya" : "Tidak") << endl;
//     cout << endl;

//     cout << "===================================" << endl;

//     cout << "(Tail)" << endl;
//     cout << "ID: " << tail->id << endl;
//     cout << "Judul: " << tail->judul << endl;
//     cout << "Penulis: " << tail->penulis << endl;
//     cout << "Tahun: " << tail->tahun << endl;
//     cout << "Genre: " << tail->genre << endl;
//     cout << "Tersedia: " << (tail->tersedia ? "Ya" : "Tidak") << endl;
//     cout << endl;
// }

// void cetakQueue()
// {
//     if (headPinjaman == NULL)
//     {
//         cout << "Data pinjaman masih kosong!" << endl;
//         return;
//     }

//     Pinjaman *bantu = headPinjaman;

//     do
//     {
//         cout << "ID: " << bantu->id << endl;
//         cout << "Judul: " << bantu->judul << endl;
//         cout << "Nama: " << bantu->nama << endl;
//         cout << "NIM: " << bantu->nim << endl;
//         cout << "Tanggal: " << bantu->tanggal << endl;
//         cout << endl;
//         bantu = bantu->next;
//     } while (bantu != NULL);

//     cout << "===================================" << endl;

//     cout << "(Head)" << endl;
//     cout << "ID: " << headPinjaman->id << endl;
//     cout << "Judul: " << headPinjaman->judul << endl;
//     cout << "Nama: " << headPinjaman->nama << endl;
//     cout << "NIM: " << headPinjaman->nim << endl;
//     cout << "Tanggal: " << headPinjaman->tanggal << endl;
//     cout << endl;

//     cout << "===================================" << endl;

//     cout << "(Tail)" << endl;
//     cout << "ID: " << tailPinjaman->id << endl;
//     cout << "Judul: " << tailPinjaman->judul << endl;
//     cout << "Nama: " << tailPinjaman->nama << endl;
//     cout << "NIM: " << tailPinjaman->nim << endl;
//     cout << "Tanggal: " << tailPinjaman->tanggal << endl;
//     cout << endl;
// }

// int indexHurufTerakhir(string str)
// {
//     for (int i = str.length() - 1; i >= 0; i--)
//     {
//         if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))
//         {
//             return i;
//         }
//     }

//     return -1;
// }

// int indexHurufPertama(string str)
// {
//     for (int i = 0; i < str.length(); i++)
//     {
//         if ((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z'))
//         {
//             return i;
//         }
//     }

//     return -1;
// }

// void lihatBuku(Buku *buku)
// {
//     cout << "-----------------------------------" << endl;
//     cout << "Judul: " << buku->judul << endl;
//     cout << "ID: " << buku->id << endl;
//     cout << "Penulis: " << buku->penulis << endl;
//     cout << "Tahun: " << buku->tahun << endl;
//     cout << "Genre: " << buku->genre << endl;
//     cout << "Tersedia: " << (buku->tersedia ? "Ya" : "Tidak") << endl;
//     cout << "-----------------------------------" << endl;
//     cout << endl;
// }

// void lihatPinjaman(Pinjaman *pinjaman)
// {
//     cout << "-----------------------------------" << endl;
//     cout << "Judul: " << pinjaman->judul << endl;
//     cout << "ID: " << pinjaman->id << endl;
//     cout << "Nama: " << pinjaman->nama << endl;
//     cout << "NIM: " << pinjaman->nim << endl;
//     cout << "Tanggal: " << fullTanggalString(pinjaman->tanggal) << endl;
//     cout << "-----------------------------------" << endl;
//     cout << endl;
// }

// string nomorPinjaman(string idPinjaman)
// {
//     int iht = indexHurufTerakhir(idPinjaman);
//     int len = idPinjaman.length();
//     string nomor = idPinjaman.substr(iht + 1, len - (iht + 1));
//     return nomor;
// }

// string toLowerCase(string str)
// {
//     for (int i = 0; i < str.length(); i++)
//     {
//         str[i] = tolower(str[i]);
//     }

//     return str;
// }

// int inputOpsi()
// {
//     try
//     {
//         string input;
//         cout << "> ";
//         getline(cin, input);
//         return stoi(input);
//     }
//     catch (...)
//     {
//         cout << "Mohon masukkan bilangan bulat!" << endl;
//         return inputOpsi();
//     }
// }

// string inputData(string label = "")
// {
//     try
//     {
//         string input;
//         cout << label << "> ";
//         getline(cin, input);

//         if (input.empty())
//         {
//             throw "Data tidak boleh kosong!";
//         }

//         return input;
//     }
//     catch (...)
//     {
//         cout << "Mohon masukkan data yang benar!" << endl;
//         return inputData(label);
//     }
// }

// int inputDataNumerik(string label = "")
// {
//     try
//     {
//         string input = inputData(label);

//         if (!cekNumerik(input))
//         {
//             throw "Data harus berupa angka!";
//         }

//         return stoi(input);
//     }
//     catch (exception e)
//     {
//         cout << e.what() << endl;
//         cout << "Mohon masukkan data berupa angka!" << endl;
//         return inputDataNumerik(label);
//     }
// }

// Buku *cariBuku(string id)
// {
//     Buku *bantu = head;

//     do
//     {
//         if (bantu->id == id)
//         {
//             return bantu;
//         }
//         bantu = bantu->next;
//     } while (bantu != head);

//     return NULL;
// }

// Buku *cariBukuDipinjam(string idPinjaman)
// {
//     string id = idPinjaman.substr(0, indexHurufPertama(idPinjaman));
//     Buku *buku = cariBuku(id);
//     return buku;
// }

// string tetapkanIdBuku(string tempId)
// {
//     if (head == NULL)
//     {
//         return tempId;
//     }

//     Buku *bantu = head;
//     char lastChar;

//     do
//     {
//         if (bantu->id == tempId)
//         {
//             lastChar = tempId[tempId.length() - 1];
//             lastChar++;

//             tempId[tempId.length() - 1] = lastChar;

//             if (lastChar == '9')
//             {
//                 tempId += "0";
//             }
//         }
//         bantu = bantu->next;
//     } while (bantu != head);

//     return tempId;
// }

// string buatIdBuku(string &judul, int &tahunInt)
// {
//     int firstCharInt = (int)judul[0];
//     int lastCharInt = (int)judul[judul.length() - 1];
//     int yearDigitSum = 0;
//     string tahun = to_string(tahunInt);
//     string firstTwoDigit = to_string(abs(lastCharInt - firstCharInt));
//     string secondAndThirdDigit;
//     string lastDigit = "0";
//     string tempId;

//     for (int i = 0; i < tahun.length(); i++)
//     {
//         yearDigitSum += stoi(tahun.substr(i, 1));
//     }

//     secondAndThirdDigit = to_string(yearDigitSum);

//     if (firstTwoDigit.length() == 1)
//     {
//         firstTwoDigit = "0" + firstTwoDigit;
//     }

//     if (secondAndThirdDigit.length() == 1)
//     {
//         secondAndThirdDigit = "0" + secondAndThirdDigit;
//     }

//     tempId = firstTwoDigit + secondAndThirdDigit + lastDigit;

//     return tetapkanIdBuku(tempId);
// }

// int buatUrutanPinjaman()
// {
//     if (headPinjaman == NULL)
//     {
//         return 1;
//     }

//     string urutanTerakhir = tailPinjaman->id.substr(indexHurufTerakhir(tailPinjaman->id) + 1, 2);
//     int urutan = stoi(urutanTerakhir) + 1;
//     return urutan;
// }

// string buatIdPinjaman(Buku *buku, string nama)
// {
//     string idBuku = buku->id;
//     string inisialGenre = toLowerCase(buku->genre.substr(0, 1));
//     string duaDigitTerakhirTahun = to_string(tahunSekarang()).substr(2, 2);
//     string duaKarakterNama = toLowerCase(nama.substr(0, 2));
//     string urutanPeminjaman = to_string(buatUrutanPinjaman());

//     if (duaDigitTerakhirTahun.length() == 1)
//     {
//         duaDigitTerakhirTahun = "0" + duaDigitTerakhirTahun;
//     }
//     if (urutanPeminjaman.length() == 1)
//     {
//         urutanPeminjaman = "0" + urutanPeminjaman;
//     }

//     string id = idBuku + inisialGenre + duaDigitTerakhirTahun + duaKarakterNama + urutanPeminjaman;
//     return id;
// }

// void tambahKeLinkedList(Buku *buku)
// {
//     if (head == NULL)
//     {
//         head = buku;
//         tail = buku;
//     }
//     else
//     {
//         buku->next = head;
//         buku->prev = tail;
//         tail->next = buku;
//         head->prev = buku;
//         tail = buku;
//     }
// }

// void hapusDariLinkedList(string id)
// {
//     Buku *bantu = head;

//     if (head == tail && head->id == id)
//     {
//         delete head;
//         head = NULL;
//         tail = NULL;
//         return;
//     }

//     do
//     {
//         if (bantu->id == id)
//         {
//             if (bantu == head)
//             {
//                 head = bantu->next;
//             }
//             else if (bantu == tail)
//             {
//                 tail = bantu->prev;
//             }

//             bantu->prev->next = bantu->next;
//             bantu->next->prev = bantu->prev;
//             delete bantu;
//             break;
//         }
//         bantu = bantu->next;
//     } while (bantu != head);
// }

// void enqueuePinjaman(Pinjaman *pinjaman)
// {
//     if (headPinjaman == NULL)
//     {
//         headPinjaman = pinjaman;
//         tailPinjaman = pinjaman;
//     }
//     else
//     {
//         tailPinjaman->next = pinjaman;
//         tailPinjaman = pinjaman;
//         tailPinjaman->next = NULL;
//     }
// }

// void dequeuePinjaman()
// {
//     if (headPinjaman == NULL)
//     {
//         return;
//     }

//     if (headPinjaman == tailPinjaman)
//     {
//         delete headPinjaman;
//         headPinjaman = NULL;
//         tailPinjaman = NULL;
//         return;
//     }

//     Pinjaman *hapus = headPinjaman;
//     headPinjaman = headPinjaman->next;
//     delete hapus;
// }

// Buku *lineKeBuku(string line)
// {
//     Buku *buku = new Buku;
//     string delimiter = "|";
//     int delimiterIndex = 0;
//     int i = 0;
//     string token;

//     // Mengubah satu baris data menjadi objek buku
//     while (line.find(delimiter) != string::npos)
//     {
//         delimiterIndex = line.find(delimiter);
//         token = line.substr(0, delimiterIndex);

//         switch (i)
//         {
//         case 0:
//             buku->id = token;
//             break;
//         case 1:
//             buku->judul = token;
//             break;
//         case 2:
//             buku->penulis = token;
//             break;
//         case 3:
//             buku->tahun = stoi(token);
//             break;
//         case 4:
//             buku->genre = token;
//             break;
//         }
//         line.erase(0, delimiterIndex + 1);
//         i++;
//     }

//     buku->tersedia = line == "1";
//     buku->next = buku;
//     buku->prev = buku;

//     return buku;
// }

// Pinjaman *lineKeQueue(string line)
// {
//     Pinjaman *pinjaman = new Pinjaman;
//     string delimiter = "|";
//     int delimiterIndex = 0;
//     int i = 0;
//     string token;

//     // Mengubah satu baris data menjadi objek pinjaman
//     while (line.find(delimiter) != string::npos)
//     {
//         delimiterIndex = line.find(delimiter);
//         token = line.substr(0, delimiterIndex);

//         switch (i)
//         {
//         case 0:
//             pinjaman->id = token;
//             break;
//         case 1:
//             pinjaman->judul = token;
//             break;
//         case 2:
//             pinjaman->nama = token;
//             break;
//         case 3:
//             pinjaman->nim = token;
//             break;
//         }
//         line.erase(0, delimiterIndex + 1);
//         i++;
//     }

//     pinjaman->tanggal = stoi(line);
//     pinjaman->next = NULL;

//     return pinjaman;
// }

// Buku *buatBukuBaru()
// {
//     Buku *buku = new Buku;

//     buku->judul = inputData("Judul");
//     buku->penulis = inputData("Penulis");
//     buku->tahun = inputDataNumerik("Tahun");
//     buku->genre = inputData("Genre");
//     buku->id = buatIdBuku(buku->judul, buku->tahun);
//     buku->tersedia = true;
//     buku->next = buku;
//     buku->prev = buku;

//     return buku;
// }

// Pinjaman *buatPinjamanBaru(Buku *buku)
// {
//     Pinjaman *pinjaman = new Pinjaman;

//     pinjaman->judul = buku->judul;
//     pinjaman->nama = inputData("Nama Peminjam");
//     pinjaman->nim = inputData("NIM Peminjam");
//     pinjaman->id = buatIdPinjaman(buku, pinjaman->nama);
//     pinjaman->tanggal = time(0);
//     pinjaman->next = NULL;

//     return pinjaman;
// }

// /* Fungsi Utama */

// void tampilkanBuku(bool sebagaiPegawai = true)
// {
//     if (head == NULL)
//     {
//         cout << "Data buku masih kosong!" << endl;
//         return;
//     }

//     Buku *tampil = head;
//     Pinjaman *pinjaman;
//     int opsi;

//     do
//     {
//         outputHeaderPerpustakaan();
//         lihatBuku(tampil);
//         sebagaiPegawai ? outputDaftarBukuAdmin() : outputDaftarBukuPengunjung();
//         opsi = inputOpsi();

//         switch (opsi)
//         {
//         case 1:
//             tampil = tampil->next;
//             break;
//         case 2:
//             tampil = tampil->prev;
//             break;
//         case 3:
//             if (sebagaiPegawai)
//             {
//                 cout << "Opsi" << opsi << " tidak ditemukan!" << endl;
//                 break;
//             }
//             if (!tampil->tersedia)
//             {
//                 cout << "Buku tidak tersedia!" << endl;
//                 break;
//             }
//             pinjaman = buatPinjamanBaru(tampil);
//             enqueuePinjaman(pinjaman);
//             cout << "Permintaan pinjaman telah dibuat!" << endl;
//             lihatPinjaman(pinjaman);
//             cout << "Menunggu persetujuan pegawai" << endl;
//             // cout << "Log: tampikanBuku() cetakQueue()" << endl;
//             // cetakQueue();
//             break;
//         case 0:
//             if (!sebagaiPegawai)
//             {
//                 cout << "Terima kasih telah mengunjungi perpustakaan!" << endl;
//             }
//             break;

//         default:
//             cout << "Opsi" << opsi << " tidak ditemukan!" << endl;
//             break;
//         }
//     } while (opsi != 0);
// }

// void tambahBuku()
// {
//     outputTambahBuku();

//     Buku *bukuBaru = buatBukuBaru();
//     tambahKeLinkedList(bukuBaru);

//     string input = inputData("Tambah lainnya? (y/N)");

//     if (input == "y" || input == "Y")
//     {
//         tambahBuku();
//     }
//     else
//     {
//         cout << "Data berhasil ditambahkan!" << endl;
//     }
// }

// void ubahBuku()
// {
//     if (head == NULL)
//     {
//         cout << "Data buku masih kosong!" << endl;
//         return;
//     }

//     string idDicari = inputData("ID Buku yang ingin diubah");
//     Buku *buku = cariBuku(idDicari);

//     if (buku == NULL)
//     {
//         cout << "Buku dengan ID " << idDicari << " tidak ditemukan!" << endl;
//         return;
//     }

//     int opsi;

//     do
//     {
//         outputUbahBuku();

//         opsi = inputOpsi();

//         switch (opsi)
//         {
//         case 1:
//             buku->judul = inputData("Judul baru");
//             buku->id = buatIdBuku(buku->judul, buku->tahun);
//             break;
//         case 2:
//             buku->penulis = inputData("Penulis baru");
//             break;
//         case 3:
//             buku->tahun = inputDataNumerik("Tahun baru");
//             buku->id = buatIdBuku(buku->judul, buku->tahun);
//             break;
//         case 4:
//             buku->genre = inputData("Genre baru");
//             break;
//         case 0:
//             break;
//         default:
//             cout << "Opsi " << opsi << " tidak ditemukan!" << endl;
//             break;
//         }

//         lihatBuku(buku);
//         cout << endl;
//     } while (opsi != 0);
// }

// void hapusBuku()
// {
//     if (head == NULL)
//     {
//         cout << "Data buku masih kosong!" << endl;
//         return;
//     }

//     string idDicari = inputData("ID Buku yang ingin dihapus");
//     Buku *buku = cariBuku(idDicari);

//     if (buku == NULL)
//     {
//         cout << "Buku dengan ID " << idDicari << " tidak ditemukan!" << endl;
//         return;
//     }

//     lihatBuku(buku);

//     string konfirmasiInput = inputData("Apakah Anda yakin ingin menghapus buku ini? (y/N)");

//     if (konfirmasiInput == "y" || konfirmasiInput == "Y")
//     {
//         string idDihapus = buku->id;
//         hapusDariLinkedList(idDicari);
//         cout << "Buku dengan ID " << idDihapus << " berhasil dihapus!" << endl;
//     }
//     else
//     {
//         cout << "Buku tidak jadi dihapus!" << endl;
//     }
// }

// bool tandaiBukuDipinjam(string idPinjam)
// {
//     Buku *bantu = head;
//     string id = idPinjam.substr(0, indexHurufPertama(idPinjam));
//     bool found = false;

//     do
//     {
//         if (bantu->id == id)
//         {
//             bantu->tersedia = false;
//             found = true;
//             break;
//         }
//         bantu = bantu->next;
//     } while (bantu != head);

//     return found;
// }

// void prosesPermintaanPinjaman()
// {
//     if (headPinjaman == NULL)
//     {
//         cout << "Data pinjaman masih kosong!" << endl;
//         return;
//     }

//     int opsi = 0;
//     bool bukuDitandai;
//     string idPinjaman;
//     Buku *buku;

//     do
//     {
//         idPinjaman = headPinjaman->id;
//         outputHeaderPermintaanPinjaman(nomorPinjaman(idPinjaman));
//         lihatPinjaman(headPinjaman);
//         outputMenuPermintaanPinjam();
//         opsi = inputOpsi();

//         switch (opsi)
//         {
//         case 1:
//             buku = cariBukuDipinjam(idPinjaman);

//             if (buku == NULL || !buku->tersedia)
//             {
//                 cout << "Permintaan pinjaman dengan ID " << idPinjaman << " ditolak karena buku sudah dipinjam!" << endl;
//                 dequeuePinjaman();
//                 break;
//             }

//             tandaiBukuDipinjam(idPinjaman);
//             dequeuePinjaman();
//             cout << "Permintaan pinjaman dengan ID " << idPinjaman << " berhasil disetujui!" << endl;
//             break;
//         case 2:
//             dequeuePinjaman();
//             cout << "Permintaan pinjaman dengan ID " << idPinjaman << " ditolak!" << endl;
//             break;
//         case 0:
//             break;
//         default:
//             cout << "Opsi " << opsi << " tidak ditemukan!" << endl;
//             break;
//         }

//         if (headPinjaman == NULL)
//         {
//             cout << "Data pinjaman sudah kosong!" << endl;
//             break;
//         }
//     } while (opsi != 0);
// }

// void muatDataBukuKeLinkedList()
// {
//     ifstream file(FILE_DATA);

//     if (!file.is_open())
//     {
//         return;
//     }

//     string line;

//     // Tambah semua data buku ke linked list
//     while (getline(file, line))
//     {
//         Buku *buku = lineKeBuku(line);
//         tambahKeLinkedList(buku);
//     }
// }

// void muatDataPinjamanKeQueue()
// {
//     ifstream file(FILE_PINJAMAN);

//     if (!file.is_open())
//     {
//         return;
//     }

//     string line;
//     while (getline(file, line))
//     {
//         Pinjaman *pinjaman = lineKeQueue(line);
//         lihatPinjaman(pinjaman);
//         enqueuePinjaman(pinjaman);
//     }
// }

// void kosongkanLinkedList()
// {
//     if (head == NULL)
//     {
//         return;
//     }
//     Buku *bantu = head;

//     do
//     {
//         Buku *hapus = bantu;
//         bantu = bantu->next;
//         delete hapus;
//     } while (bantu != head);

//     head = NULL;
//     tail = NULL;
// }

// void kosongkanQueue()
// {
//     if (headPinjaman == NULL)
//     {
//         return;
//     }

//     Pinjaman *bantu = headPinjaman;

//     do
//     {
//         Pinjaman *hapus = bantu;
//         bantu = bantu->next;
//         delete hapus;
//     } while (bantu != NULL);

//     headPinjaman = NULL;
//     tailPinjaman = NULL;
// }

// void simpanDataBukuKeFile()
// {
//     ofstream file(FILE_DATA);

//     if (head == NULL)
//     {
//         return;
//     }

//     Buku *bantu = head;

//     do
//     {
//         file << bantu->id << "|" << bantu->judul << "|" << bantu->penulis << "|" << bantu->tahun << "|" << bantu->genre << "|" << (bantu->tersedia ? "1" : "0") << endl;
//         bantu = bantu->next;
//     } while (bantu != head);

//     file.close();
// }

// void simpanDataPinjamanKeFile()
// {
//     ofstream file(FILE_PINJAMAN);

//     if (headPinjaman == NULL)
//     {
//         return;
//     }

//     Pinjaman *bantu = headPinjaman;

//     do
//     {
//         file << bantu->id << "|" << bantu->judul << "|" << bantu->nama << "|" << bantu->nim << "|" << bantu->tanggal << endl;
//         bantu = bantu->next;
//     } while (bantu != NULL);

//     file.close();
// }

// void simpanDanKeluar()
// {
//     cout << "Menyimpan data..." << endl;
//     simpanDataBukuKeFile();
//     simpanDataPinjamanKeFile();
//     cout << "Data berhasil disimpan!" << endl;
//     kosongkanLinkedList();
//     kosongkanQueue();
//     cout << "Terima kasih telah mengunjungi perpustakaan!" << endl;
// }

// void masukSebagaiPegawai()
// {
//     outputDasboardAdmin();

//     int opsi = inputOpsi();

//     switch (opsi)
//     {
//     case 1:
//         tambahBuku();
//         break;
//     case 2:
//         tampilkanBuku();
//         break;
//     case 3:
//         ubahBuku();
//         break;
//     case 4:
//         hapusBuku();
//         break;
//     case 5:
//         prosesPermintaanPinjaman();
//         break;
//     case 0:
//         break;
//     default:
//         cout << "Opsi " << opsi << " tidak ditemukan!" << endl;
//         cout << endl;
//         break;
//     }

//     if (opsi != 0)
//     {
//         masukSebagaiPegawai();
//     }
// }

// void masukSebagaiPengunjung()
// {
//     tampilkanBuku(false);
// }

// void perpustakaan()
// {
//     muatDataBukuKeLinkedList();
//     muatDataPinjamanKeQueue();
//     int role = 0;

//     do
//     {
//         outputSelamatDatang();
//         role = inputOpsi();

//         switch (role)
//         {
//         case 1:
//             masukSebagaiPegawai();
//             break;
//         case 2:
//             masukSebagaiPengunjung();
//             break;
//         case 0:
//             simpanDanKeluar();
//             break;
//         default:
//             cout << "Opsi " << role << " tidak ditemukan!" << endl;
//             cout << endl;
//             break;
//         }
//     } while (role != 0);
// }

// int main()
// {
//     perpustakaan();
//     return 0;
// }

// /* End Fungsi Utama */