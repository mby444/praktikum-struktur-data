#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Display.H>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

struct Buku
{
    string id, judul, penulis, tahun, genre;
    bool tersedia;
    Buku *next;
    Buku *prev;
};

struct Pinjaman
{
    string idPinjaman;
    Buku *buku;
    string namaPeminjam;
    time_t tanggal;
    Pinjaman *next;
};

// **Variabel Global**
Buku *head = NULL;
Buku *tail = NULL;
Buku *current = NULL;
Pinjaman *headPinjaman = NULL, *tailPinjaman = NULL;

// **Komponen GUI**
Fl_Box *judulBox, *idBox, *penulisBox, *tahunBox, *genreBox, *statusBox;
Fl_Button *btnPinjam, *btnSelanjutnya, *btnSebelumnya, *btnSetujui, *btnTolak;

// **Fungsi Menampilkan Buku di GUI**
void tampilkanBuku()
{
    if (!current)
    {
        judulBox->label("Judul: -");
        idBox->label("ID: -");
        penulisBox->label("Penulis: -");
        tahunBox->label("Tahun: -");
        genreBox->label("Genre: -");
        statusBox->label("Ketersediaan: -");
        return;
    }

    string status = current->tersedia ? "Tersedia" : "Dipinjam";
    static string judulLabel, idLabel, penulisLabel, tahunLabel, genreLabel, tersediaLabel;

    judulLabel = "Judul: " + current->judul;
    idLabel = "ID: " + current->id;
    penulisLabel = "Penulis: " + current->penulis;
    tahunLabel = "Tahun: " + current->tahun;
    genreLabel = "Genre: " + current->genre;
    tersediaLabel = "Ketersediaan: " + status;

    judulBox->label(judulLabel.c_str());
    idBox->label(idLabel.c_str());
    penulisBox->label(penulisLabel.c_str());
    tahunBox->label(tahunLabel.c_str());
    genreBox->label(genreLabel.c_str());
    statusBox->label(tersediaLabel.c_str());
}

// **Navigasi ke Buku Berikutnya**
void selanjutnya(Fl_Widget *)
{
    if (current)
        current = current->next;
    tampilkanBuku();
}

// **Navigasi ke Buku Sebelumnya**
void sebelumnya(Fl_Widget *)
{
    if (current)
        current = current->prev;
    tampilkanBuku();
}

// **Peminjaman Buku**
void pinjamBuku(Fl_Widget *)
{
    if (current && current->tersedia)
    {
        current->tersedia = false;
        tampilkanBuku();
        cout << "Buku " << current->judul << " berhasil dipinjam!" << endl;
    }
}

// **Tampilan Pegawai untuk Memproses Pinjaman**
void prosesPinjaman(Fl_Widget *, void *mode)
{
    if (!headPinjaman)
    {
        cout << "Tidak ada pinjaman dalam antrian!" << endl;
        return;
    }

    Pinjaman *diproses = headPinjaman;
    headPinjaman = headPinjaman->next;
    if (!headPinjaman)
        tailPinjaman = NULL;

    string aksi = (mode == (void *)1) ? "disetujui" : "ditolak";
    if (mode == (void *)1)
    {
        diproses->buku->tersedia = false;
    }
    cout << "Pinjaman " << diproses->idPinjaman << " " << aksi << endl;
    delete diproses;
}

// **Tampilan GUI Buku**
void showBukuGUI(Fl_Widget *)
{
    Fl_Window *winBuku = new Fl_Window(400, 300, "Lihat Buku");

    judulBox = new Fl_Box(50, 20, 300, 30);
    idBox = new Fl_Box(50, 50, 300, 30);
    penulisBox = new Fl_Box(50, 80, 300, 30);
    tahunBox = new Fl_Box(50, 110, 300, 30);
    genreBox = new Fl_Box(50, 140, 300, 30);
    statusBox = new Fl_Box(50, 170, 300, 30);

    btnSebelumnya = new Fl_Button(50, 210, 100, 30, "Sebelumnya");
    btnSelanjutnya = new Fl_Button(160, 210, 100, 30, "Selanjutnya");
    btnPinjam = new Fl_Button(50, 250, 100, 30, "Pinjam");

    btnSebelumnya->callback(sebelumnya);
    btnSelanjutnya->callback(selanjutnya);
    btnPinjam->callback(pinjamBuku);

    winBuku->end();
    winBuku->show();

    current = head;
    tampilkanBuku();
}

// **Tampilan Pegawai untuk Memproses Pinjaman**
void showPegawaiGUI(Fl_Widget *)
{
    Fl_Window *winPegawai = new Fl_Window(400, 200, "Proses Pinjaman");

    btnSetujui = new Fl_Button(50, 50, 140, 40, "Setujui");
    btnTolak = new Fl_Button(200, 50, 140, 40, "Tolak");

    btnSetujui->callback(prosesPinjaman, (void *)1);
    btnTolak->callback(prosesPinjaman, (void *)0);

    winPegawai->end();
    winPegawai->show();
}

// **Tampilan Utama GUI**
int main()
{
    Fl_Window *win = new Fl_Window(300, 200, "Perpustakaan FLTK");
    Fl_Button *btnAdmin = new Fl_Button(80, 50, 140, 40, "Pegawai");
    Fl_Button *btnPengunjung = new Fl_Button(80, 110, 140, 40, "Pengunjung");

    btnAdmin->callback(showPegawaiGUI);
    btnPengunjung->callback(showBukuGUI);

    win->end();
    win->show();
    return Fl::run();
}
