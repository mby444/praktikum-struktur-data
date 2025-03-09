#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Input.H>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define FILE_DATA "database.txt"

// Struktur Data Buku (Circular Doubly Linked List)
struct Buku
{
    string id, judul, penulis, tahun, genre;
    bool tersedia;
    Buku *next;
    Buku *prev;
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
Fl_Input *judulInputPengunjung, *penulisInputPengunjung, *tahunInputPengunjung, *genreInputPengunjung;

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

bool cekNumerik(string input)
{
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] > '9' || input[i] < '0')
        {
            return false;
        }
    }
    return true;
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

// **Peminjaman Buku (Khusus Pengunjung)**
void pinjamBuku(Fl_Widget *)
{
    // if (tampil && tampil->tersedia)
    // {
    //     tampil->tersedia = false;
    //     lihatBuku();
    // }
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

void muatDataKeLinkedList()
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

// **Tampilan Pegawai**
void showAdmin(Fl_Widget *)
{
    Fl_Window *winAdmin = new Fl_Window(300, 400, "Dashboard Admin");
    Fl_Button *btnTambah = new Fl_Button(50, 50, 200, 40, "Tambah Buku");
    Fl_Button *btnLihat = new Fl_Button(50, 100, 200, 40, "Lihat Buku");
    Fl_Button *btnUbah = new Fl_Button(50, 150, 200, 40, "Ubah Buku");
    Fl_Button *btnHapus = new Fl_Button(50, 200, 200, 40, "Hapus Buku");
    Fl_Button *btnSimpanDanKeluar = new Fl_Button(50, 250, 200, 40, "Simpan dan Keluar");

    btnTambah->callback(showTambahBuku);
    btnLihat->callback(showBukuAdmin);
    btnUbah->callback(showInputIdUbahBuku);
    btnHapus->callback(showInputIdHapusBuku);
    btnSimpanDanKeluar->callback(simpanDanKeluar);
    winAdmin->end();
    winAdmin->show();
}

// **Tampilan Utama**
int main()
{
    muatDataKeLinkedList();

    Fl_Window *win = new Fl_Window(300, 200, "Perpustakaan");
    Fl_Button *btnAdmin = new Fl_Button(50, 50, 200, 40, "Masuk Sebagai Pegawai");
    Fl_Button *btnPengunjung = new Fl_Button(50, 110, 200, 40, "Masuk Sebagai Pengunjung");

    btnAdmin->callback(showAdmin);
    btnPengunjung->callback(showBukuPengunjung);

    win->end();
    win->show();
    return Fl::run();
}
