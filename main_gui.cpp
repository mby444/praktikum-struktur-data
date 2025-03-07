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
Buku *tampil = NULL;

// Elemen tampilan buku
Buku *bukuDiubah;
Fl_Box *judulBox, *idBox, *penulisBox, *tahunBox, *genreBox, *tersediaBox;
Fl_Button *btnPinjam, *btnSelanjutnya, *btnSebelumnya, *btnKembali;
Fl_Input *judulInput, *penulisInput, *tahunInput, *genreInput;
Fl_Input *idDicariInput;

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

// **Fungsi Menampilkan Satu Buku**
void lihatBuku()
{
    if (!tampil)
    {
        judulBox->label("Judul: -");
        idBox->label("ID: -");
        penulisBox->label("Penulis: -");
        tahunBox->label("Tahun: -");
        genreBox->label("Genre: -");
        tersediaBox->label("Ketersediaan: -");
        return;
    }

    string judulLabel = ("Judul: " + tampil->judul);
    string idLabel = ("ID: " + tampil->id);
    string penulisLabel = ("Penulis: " + tampil->penulis);
    string tahunLabel = ("Tahun: " + tampil->tahun);
    string genreLabel = ("Genre: " + tampil->genre);
    string tersediaLabel = (tampil->tersedia) ? "Ketersediaan: Tersedia" : "Ketersediaan: Tidak Tersedia";

    judulBox->copy_label(judulLabel.c_str());
    idBox->copy_label(idLabel.c_str());
    penulisBox->copy_label(penulisLabel.c_str());
    tahunBox->copy_label(tahunLabel.c_str());
    genreBox->copy_label(genreLabel.c_str());
    tersediaBox->copy_label(tersediaLabel.c_str());
}

// **Navigasi ke Buku Berikutnya**
void selanjutnya(Fl_Widget *)
{
    if (tampil)
    {
        tampil = tampil->next;
    }
    lihatBuku();
}

// **Navigasi ke Buku Sebelumnya**
void sebelumnya(Fl_Widget *)
{
    if (tampil)
    {
        tampil = tampil->prev;
    }
    lihatBuku();
}

// **Peminjaman Buku (Khusus Pengunjung)**
void pinjamBuku(Fl_Widget *)
{
    if (tampil && tampil->tersedia)
    {
        tampil->tersedia = false;
        lihatBuku();
    }
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

// **Tampilan Pengunjung**
void showBukuPengunjung(Fl_Widget *)
{
    Fl_Window *winPengunjung = new Fl_Window(400, 300, "Lihat Buku");

    judulBox = new Fl_Box(50, 20, 300, 30);
    idBox = new Fl_Box(50, 50, 300, 30);
    penulisBox = new Fl_Box(50, 80, 300, 30);
    tahunBox = new Fl_Box(50, 110, 300, 30);
    genreBox = new Fl_Box(50, 140, 300, 30);
    tersediaBox = new Fl_Box(50, 170, 300, 30);

    btnSebelumnya = new Fl_Button(50, 210, 100, 30, "Sebelumnya");
    btnSelanjutnya = new Fl_Button(160, 210, 100, 30, "Selanjutnya");
    btnPinjam = new Fl_Button(50, 250, 100, 30, "Pinjam");
    btnKembali = new Fl_Button(160, 250, 100, 30, "Kembali");

    btnSebelumnya->callback(sebelumnya);
    btnSelanjutnya->callback(selanjutnya);
    btnPinjam->callback(pinjamBuku);
    btnKembali->callback(kembali, winPengunjung);

    winPengunjung->end();
    winPengunjung->show();

    tampil = head;
    lihatBuku();
}

// **Tampilan Pegawai**
void showBukuAdmin(Fl_Widget *)
{
    Fl_Window *winPegawai = new Fl_Window(400, 300, "Lihat Buku");

    if (head == NULL)
    {
        Fl_Box *box = new Fl_Box(50, 50, 300, 30, "Data buku masih kosong!");
        Fl_Button *btnKembali = new Fl_Button(160, 100, 100, 30, "Kembali");

        btnKembali->callback(kembali, winPegawai);
        box->box(FL_NO_BOX);
        winPegawai->end();
        winPegawai->show();
        return;
    }

    judulBox = new Fl_Box(50, 20, 300, 30);
    idBox = new Fl_Box(50, 50, 300, 30);
    penulisBox = new Fl_Box(50, 80, 300, 30);
    tahunBox = new Fl_Box(50, 110, 300, 30);
    genreBox = new Fl_Box(50, 140, 300, 30);
    tersediaBox = new Fl_Box(50, 170, 300, 30);

    btnSebelumnya = new Fl_Button(50, 210, 100, 30, "Sebelumnya");
    btnSelanjutnya = new Fl_Button(160, 210, 100, 30, "Selanjutnya");
    btnKembali = new Fl_Button(160, 250, 100, 30, "Kembali");

    btnSebelumnya->callback(sebelumnya);
    btnSelanjutnya->callback(selanjutnya);
    btnKembali->callback(kembali, winPegawai);

    winPegawai->end();
    winPegawai->show();

    tampil = head;
    lihatBuku();
}

// **Menambahkan Buku ke Linked List**
void tambahBuku(string judul, string penulis, string tahun, string genre)
{
    Buku *b = new Buku();
    b->id = "B" + to_string((head ? stoi(head->prev->id.substr(1)) + 1 : 1));
    b->judul = judul;
    b->penulis = penulis;
    b->tahun = tahun;
    b->genre = genre;
    b->tersedia = true;

    if (!head)
    {
        head = b;
        head->next = head;
        head->prev = head;
    }
    else
    {
        Buku *tail = head->prev;
        tail->next = b;
        b->prev = tail;
        b->next = head;
        head->prev = b;
    }
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

string buatIdBuku(string judul, string tahun)
{
    return "00000";
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

Buku *buatBukuBaru()
{
    string judul = judulInput->value();
    string penulis = penulisInput->value();
    string tahun = tahunInput->value();
    string genre = genreInput->value();

    if (judul.empty() || penulis.empty() || tahun.empty() || genre.empty())
    {
        Fl_Box *box = new Fl_Box(50, 250, 200, 30, "Data tidak boleh kosong!");
        box->box(FL_NO_BOX);
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
        Fl_Box *box = new Fl_Box(50, 50, 200, 30, "Buku tidak ditemukan!");
        box->box(FL_NO_BOX);
        return;
    }

    if (bukuDiubah->judul.empty() || bukuDiubah->penulis.empty() || bukuDiubah->tahun.empty() || bukuDiubah->genre.empty())
    {
        Fl_Box *box = new Fl_Box(50, 50, 200, 30, "Data tidak boleh kosong!");
        box->box(FL_NO_BOX);
        return;
    }

    if (bukuDiubah->judul != judulInput->value() || bukuDiubah->tahun != tahunInput->value())
    {
        bukuDiubah->id = buatIdBuku(judulInput->value(), tahunInput->value());
    }

    bukuDiubah->judul = judulInput->value();
    bukuDiubah->penulis = penulisInput->value();
    bukuDiubah->tahun = tahunInput->value();
    bukuDiubah->genre = genreInput->value();
    ((Fl_Window *)win)->hide();
}

void showTambahBuku(Fl_Widget *)
{
    Fl_Window *winTambah = new Fl_Window(300, 300, "Tambah Buku");

    Fl_Box *judulLabel = new Fl_Box(50, 20, 200, 30, "Judul");
    Fl_Box *penulisLabel = new Fl_Box(50, 70, 200, 30, "Penulis");
    Fl_Box *tahunLabel = new Fl_Box(50, 120, 200, 30, "Tahun");
    Fl_Box *genreLabel = new Fl_Box(50, 170, 200, 30, "Genre");

    judulInput = new Fl_Input(50, 50, 200, 30);
    penulisInput = new Fl_Input(50, 100, 200, 30);
    tahunInput = new Fl_Input(50, 150, 200, 30);
    genreInput = new Fl_Input(50, 200, 200, 30);

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
        Fl_Box *box = new Fl_Box(50, 50, 200, 30, "Buku tidak ditemukan!");
        cout << "Buku tidak ditemukan!" << endl;
        box->box(FL_NO_BOX);
        return;
    }

    Fl_Box *judulLabel = new Fl_Box(50, 20, 200, 30, "Judul");
    Fl_Box *penulisLabel = new Fl_Box(50, 70, 200, 30, "Penulis");
    Fl_Box *tahunLabel = new Fl_Box(50, 120, 200, 30, "Tahun");
    Fl_Box *genreLabel = new Fl_Box(50, 170, 200, 30, "Genre");

    judulInput = new Fl_Input(50, 50, 200, 30);
    penulisInput = new Fl_Input(50, 100, 200, 30);
    tahunInput = new Fl_Input(50, 150, 200, 30);
    genreInput = new Fl_Input(50, 200, 200, 30);

    judulInput->value(bukuDiubah->judul.c_str());
    penulisInput->value(bukuDiubah->penulis.c_str());
    tahunInput->value(bukuDiubah->tahun.c_str());
    genreInput->value(bukuDiubah->genre.c_str());

    Fl_Button *btnUbah = new Fl_Button(50, 250, 100, 30, "Simpan");
    Fl_Button *btnBatal = new Fl_Button(160, 250, 100, 30, "Batal");

    btnUbah->callback(ubah, winUbah);
    btnBatal->callback(kembali, winUbah);

    winUbah->end();
    winUbah->show();
    ((Fl_Window *)win)->hide();
}

void showInputIdUbahBuku(Fl_Widget *)
{
    Fl_Window *winInputId = new Fl_Window(300, 200, "Ubah Buku");

    if (head == NULL)
    {
        Fl_Box *box = new Fl_Box(50, 50, 200, 30, "Data buku masih kosong!");
        cout << "Data buku masih kosong!" << endl;
        box->box(FL_NO_BOX);
        return;
    }

    Fl_Box *box = new Fl_Box(50, 50, 200, 30, "Masukkan ID Buku");
    idDicariInput = new Fl_Input(50, 100, 200, 30);
    Fl_Button *btnCari = new Fl_Button(50, 150, 100, 30, "Cari");

    btnCari->callback(showUbahBuku, winInputId);

    winInputId->end();
    winInputId->show();
}

// **Tampilan Pegawai**
void showAdmin(Fl_Widget *)
{
    // tambahBuku("BUMI MANUSIA", "Pramoedya Ananta Toer", "1980", "Fiksi");
    // tambahBuku("LASKAR PELANGI", "Andrea Hirata", "2005", "Inspirasi");
    // tambahBuku("HABIS GELAP TERBITLAH TERANG", "R.A. Kartini", "1922", "Biografi");

    Fl_Window *winAdmin = new Fl_Window(300, 400, "Dashboard Admin");
    Fl_Button *btnTambah = new Fl_Button(50, 50, 200, 40, "Tambah Buku");
    Fl_Button *btnLihat = new Fl_Button(50, 100, 200, 40, "Lihat Buku");
    Fl_Button *btnUbah = new Fl_Button(50, 150, 200, 40, "Ubah Buku");
    Fl_Button *btnHapus = new Fl_Button(50, 200, 200, 40, "Hapus Buku");
    Fl_Button *btnSimpanDanKeluar = new Fl_Button(50, 250, 200, 40, "Simpan dan Keluar");

    btnTambah->callback(showTambahBuku);
    btnLihat->callback(showBukuAdmin);
    btnUbah->callback(showInputIdUbahBuku);
    btnSimpanDanKeluar->callback(simpanDanKeluar);
    winAdmin->end();
    winAdmin->show();
}

// **Tampilan Utama**
int main()
{
    muatDataKeLinkedList();

    Fl_Window *win = new Fl_Window(300, 200, "Perpustakaan");
    Fl_Button *btnAdmin = new Fl_Button(80, 50, 140, 40, "Masuk Sebagai Pegawai");
    Fl_Button *btnPengunjung = new Fl_Button(80, 110, 140, 40, "Masuk Sebagai Pengunjung");

    btnAdmin->callback(showAdmin);
    btnPengunjung->callback(showBukuPengunjung);

    win->end();
    win->show();
    return Fl::run();
}
