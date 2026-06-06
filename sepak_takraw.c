/*
 * PROGRAM PENDATAAN SEPAK TAKRAW D.I.Y
 * =====================================
 * Program ini adalah sistem manajemen data untuk turnamen Sepak Takraw.
 * Fitur utama: CRUD (Create, Read, Update, Delete) untuk data tim, pemain,
 * dan juara, serta menampilkan laporan-laporan terkait.
 *
 * Bahasa          : C
 * Kompiler        : GCC / MinGW
 * Standar         : C90 / C99
 */

#include <stdio.h>   // Untuk fungsi input/output: printf, scanf, fgets, getchar
#include <stdlib.h>  // Untuk system("cls") - membersihkan layar konsol
#include <string.h>  // Untuk strcmp (membandingkan string), strcpy (menyalin string), strcspn

// ==================== KONSTANTA GLOBAL ====================

#define MAX_TIM 10      // Maksimum jumlah tim yang bisa didaftarkan (10 tim)
#define MAX_PEMAIN 3    // Maksimum pemain per tim (3 orang, sesuai aturan Sepak Takraw)
#define MAX_NAMA 50     // Panjang maksimum untuk string nama (termasuk null terminator)

// ==================== STRUKTUR DATA ====================

/*
 * Struktur Pemain
 * Menyimpan data seorang pemain.
 * Anggota:
 *   - nama: array char sepanjang MAX_NAMA untuk menyimpan nama pemain
 */
typedef struct {
    char nama[MAX_NAMA];
} Pemain;

/*
 * Struktur Tim
 * Menyimpan data sebuah tim Sepak Takraw.
 * Anggota:
 *   - nomor        : nomor unik tim (integer, di-generate otomatis)
 *   - namaTim      : nama tim (string)
 *   - manajer      : nama manajer/coach tim (string)
 *   - pemain       : array of Pemain dengan ukuran MAX_PEMAIN (maks 3 pemain)
 *   - jumlahPemain : counter jumlah pemain yang sudah terisi di tim ini
 */
typedef struct {
    int nomor;
    char namaTim[MAX_NAMA];
    char manajer[MAX_NAMA];
    Pemain pemain[MAX_PEMAIN];
    int jumlahPemain;
} Tim;

// ==================== VARIABEL GLOBAL ====================

Tim tim[MAX_TIM];          // Array statis untuk menyimpan semua tim (max 10)
int jumlahTim = 0;          // Counter jumlah tim yang sudah terdaftar
int juara1 = -1, juara2 = -1, juara3 = -1;  // Nomor tim untuk juara 1, 2, 3 (-1 = belum diisi)
int nextNomor = 1;          // Nomor otomatis untuk tim berikutnya (increment otomatis)

// ==================== FUNGSI BANTU (HELPER) ====================

/*
 * cariTimByNomor
 * -----------------
 * Mencari index tim dalam array berdasarkan nomor tim.
 * Parameter:
 *   - nomor : nomor tim yang dicari
 * Return:
 *   - index tim di array tim[] jika ditemukan
 *   - -1 jika tidak ditemukan
 *
 * Cara kerja: loop linear search dari i=0 hingga jumlahTim-1.
 */
int cariTimByNomor(int nomor) {
    int i;
    for (i = 0; i < jumlahTim; i++) {
        if (tim[i].nomor == nomor)  // Bandingkan nomor tim dengan parameter
            return i;               // Kembalikan index jika cocok
    }
    return -1;  // Tidak ditemukan
}

/*
 * cariTimByNama
 * -----------------
 * Mencari index tim dalam array berdasarkan nama tim.
 * Parameter:
 *   - nama : string nama tim yang dicari
 * Return:
 *   - index tim di array tim[] jika ditemukan
 *   - -1 jika tidak ditemukan
 *
 * strcmp() mengembalikan 0 jika dua string identik.
 */
int cariTimByNama(char nama[]) {
    int i;
    for (i = 0; i < jumlahTim; i++) {
        if (strcmp(tim[i].namaTim, nama) == 0)  // Bandingkan nama dengan strcmp
            return i;
    }
    return -1;
}

// ==================== FITUR ENTRI TIM ====================

/*
 * entriTim
 * ---------
 * Mendaftarkan tim baru ke dalam sistem.
 * Nomor tim di-generate otomatis (incremental).
 * Input: nama tim dan nama manajer dari user.
 * Setelah input, jumlahTim bertambah 1 dan nextNomor increment.
 *
 * Batasan: maksimum MAX_TIM (10) tim.
 * Dua kali getchar() di awal: sekali untuk consume newline, sekali untuk pause.
 */
void entriTim() {
    system("cls");  // Bersihkan layar (Windows-specific)
    if (jumlahTim >= MAX_TIM) {
        // Jika sudah penuh, tampilkan pesan error
        printf("Maaf, maksimum %d tim sudah tercapai.\n", MAX_TIM);
        printf("Tekan enter untuk kembali...");
        getchar();
        getchar();
    } else {
        // Bisa menambah tim baru
        printf("=================================\n");
        printf("       ENTRI DATA TIM\n");
        printf("=================================\n");
        printf("Nomor tim: %d (otomatis)\n", nextNomor);
        tim[jumlahTim].nomor = nextNomor;  // Set nomor tim ke index saat ini

        // Input nama tim — fgets membaca hingga MAX_NAMA karakter atau newline
        printf("Nama tim: ");
        fgets(tim[jumlahTim].namaTim, MAX_NAMA, stdin);
        // strcspn mencari posisi newline "\n" dan menggantinya dengan null terminator '\0'
        // ini penting agar fgets tidak menyertakan newline di akhir string
        tim[jumlahTim].namaTim[strcspn(tim[jumlahTim].namaTim, "\n")] = '\0';

        // Input nama manajer
        printf("Nama manajer: ");
        fgets(tim[jumlahTim].manajer, MAX_NAMA, stdin);
        tim[jumlahTim].manajer[strcspn(tim[jumlahTim].manajer, "\n")] = '\0';

        tim[jumlahTim].jumlahPemain = 0;  // Tim baru belum punya pemain
        nextNomor++;   // Increment nomor untuk tim berikutnya
        jumlahTim++;   // Tambah jumlah tim yang terdaftar

        printf("\nData tim berhasil ditambahkan!\n");
        printf("Tekan enter untuk kembali...");
        getchar();
    }
}

// ==================== FITUR EDIT TIM ====================

/*
 * editTim
 * --------
 * Mengedit data tim yang sudah ada.
 * User memasukkan nomor tim yang akan diedit.
 * Jika ditemukan, tampilkan data lama lalu minta data baru.
 * Hanya nama tim dan manajer yang bisa diubah.
 *
 * scanf("%d", &nomor) membaca integer — getchar() setelahnya
 * untuk mengonsumsi karakter newline yang tersisa.
 */
void editTim() {
    int nomor, idx;
    system("cls");
    if (jumlahTim == 0) {
        printf("Belum ada tim terdaftar.\n");
        printf("Tekan enter untuk kembali...");
        getchar();
    } else {
        printf("=================================\n");
        printf("        EDIT DATA TIM\n");
        printf("=================================\n");
        printf("Masukkan nomor tim yang akan diedit: ");
        scanf("%d", &nomor);   // Baca nomor tim dari user
        getchar();             // Konsumsi newline (\n) dari buffer stdin

        idx = cariTimByNomor(nomor);  // Cari index tim berdasarkan nomor
        if (idx == -1) {
            printf("Tim dengan nomor %d tidak ditemukan.\n", nomor);
            printf("Tekan enter untuk kembali...");
            getchar();
        } else {
            // Tampilkan data lama
            printf("\nData lama:\n");
            printf("Nama tim: %s\n", tim[idx].namaTim);
            printf("Manajer: %s\n", tim[idx].manajer);

            // Minta data baru
            printf("\nMasukkan data baru:\n");
            printf("Nama tim: ");
            fgets(tim[idx].namaTim, MAX_NAMA, stdin);
            tim[idx].namaTim[strcspn(tim[idx].namaTim, "\n")] = '\0';
            printf("Nama manajer: ");
            fgets(tim[idx].manajer, MAX_NAMA, stdin);
            tim[idx].manajer[strcspn(tim[idx].manajer, "\n")] = '\0';

            printf("\nData tim berhasil diubah!\n");
            printf("Tekan enter untuk kembali...");
            getchar();
        }
    }
}

// ==================== FITUR HAPUS TIM ====================

/*
 * hapusTim
 * ---------
 * Menghapus data tim beserta pemainnya.
 * User memasukkan nomor tim, lalu konfirmasi (y/n).
 * Jika dihapus, data tim setelahnya digeser ke kiri (array shift).
 * Juga membersihkan data juara jika tim yang dihapus adalah juara.
 */
void hapusTim() {
    int nomor, idx, i;
    char konfirmasi;
    system("cls");
    if (jumlahTim == 0) {
        printf("Belum ada tim terdaftar.\n");
        printf("Tekan enter untuk kembali...");
        getchar();
    } else {
        printf("=================================\n");
        printf("       HAPUS DATA TIM\n");
        printf("=================================\n");
        printf("Masukkan nomor tim yang akan dihapus: ");
        scanf("%d", &nomor);
        getchar();

        idx = cariTimByNomor(nomor);
        if (idx == -1) {
            printf("Tim dengan nomor %d tidak ditemukan.\n", nomor);
            printf("Tekan enter untuk kembali...");
            getchar();
        } else {
            // Tampilkan data tim sebelum dihapus
            printf("\nData tim yang akan dihapus:\n");
            printf("Nama tim: %s\n", tim[idx].namaTim);
            printf("Manajer: %s\n", tim[idx].manajer);
            printf("Pemain: %d orang\n", tim[idx].jumlahPemain);

            // Konfirmasi penghapusan
            printf("\nYakin ingin menghapus? (y/n): ");
            scanf("%c", &konfirmasi);
            getchar();

            if (konfirmasi != 'y' && konfirmasi != 'Y') {
                printf("Penghapusan dibatalkan.\n");
                printf("Tekan enter untuk kembali...");
                getchar();
            } else {
                // Jika tim yang dihapus adalah juara, reset nilai juara ke -1
                if (juara1 == tim[idx].nomor) juara1 = -1;
                if (juara2 == tim[idx].nomor) juara2 = -1;
                if (juara3 == tim[idx].nomor) juara3 = -1;

                // Geser semua elemen setelah idx ke kiri (menimpa data yang dihapus)
                for (i = idx; i < jumlahTim - 1; i++) {
                    tim[i] = tim[i + 1];  // Copy struct (shallow copy)
                }
                jumlahTim--;  // Kurangi jumlah tim

                printf("\nData tim beserta pemainnya berhasil dihapus!\n");
                printf("Tekan enter untuk kembali...");
                getchar();
            }
        }
    }
}

// ==================== FITUR ENTRI PEMAIN ====================

/*
 * entriPemain
 * ------------
 * Menambahkan pemain ke dalam tim tertentu.
 * Maksimum MAX_PEMAIN (3) pemain per tim.
 * User bisa memilih untuk menambah pemain satu per satu (konfirmasi y/n).
 * Jika sudah 3 pemain, loop berhenti otomatis.
 */
void entriPemain() {
    int nomor, idx, i;
    char lagi;
    system("cls");
    if (jumlahTim == 0) {
        printf("Belum ada tim. Silakan daftarkan tim terlebih dahulu.\n");
        printf("Tekan enter untuk kembali...");
        getchar();
    } else {
        printf("=================================\n");
        printf("      ENTRI NAMA PEMAIN\n");
        printf("=================================\n");
        printf("Masukkan nomor tim: ");
        scanf("%d", &nomor);
        getchar();

        idx = cariTimByNomor(nomor);
        if (idx == -1) {
            printf("Tim dengan nomor %d tidak ditemukan.\n", nomor);
            printf("Tekan enter untuk kembali...");
            getchar();
        } else if (tim[idx].jumlahPemain >= MAX_PEMAIN) {
            // Tim sudah penuh (3 pemain)
            printf("Tim %s sudah memiliki %d pemain (maksimum).\n", tim[idx].namaTim, MAX_PEMAIN);
            printf("Tekan enter untuk kembali...");
            getchar();
        } else {
            printf("\nTim: %s | Manajer: %s\n", tim[idx].namaTim, tim[idx].manajer);
            printf("Jumlah pemain saat ini: %d/3\n", tim[idx].jumlahPemain);

            // Loop dari jumlah pemain yang sudah ada hingga MAX_PEMAIN
            for (i = tim[idx].jumlahPemain; i < MAX_PEMAIN; i++) {
                printf("Nama pemain ke-%d: ", i + 1);
                fgets(tim[idx].pemain[i].nama, MAX_NAMA, stdin);
                tim[idx].pemain[i].nama[strcspn(tim[idx].pemain[i].nama, "\n")] = '\0';
                tim[idx].jumlahPemain++;  // Increment counter pemain

                // Jika belum mencapai pemain terakhir (ke-3), tanya apakah mau lanjut
                if (i < MAX_PEMAIN - 1) {
                    printf("Tambah pemain lagi? (y/n): ");
                    scanf("%c", &lagi);
                    getchar();
                    if (lagi != 'y' && lagi != 'Y')
                        break;  // Keluar dari loop jika user menjawab tidak
                }
            }
            printf("\nData pemain berhasil ditambahkan!\n");
            printf("Tekan enter untuk kembali...");
            getchar();
        }
    }
}

// ==================== FITUR EDIT PEMAIN ====================

/*
 * editPemain
 * -----------
 * Mengedit nama pemain dalam suatu tim.
 * User memilih nomor tim, lalu melihat daftar pemain, dan memilih
 * nomor pemain yang akan diedit, lalu memasukkan nama baru.
 */
void editPemain() {
    int nomor, idx, noPemain, i;
    system("cls");
    if (jumlahTim == 0) {
        printf("Belum ada tim terdaftar.\n");
        printf("Tekan enter untuk kembali...");
        getchar();
    } else {
        printf("=================================\n");
        printf("       EDIT NAMA PEMAIN\n");
        printf("=================================\n");
        printf("Masukkan nomor tim: ");
        scanf("%d", &nomor);
        getchar();

        idx = cariTimByNomor(nomor);
        if (idx == -1) {
            printf("Tim dengan nomor %d tidak ditemukan.\n", nomor);
            printf("Tekan enter untuk kembali...");
            getchar();
        } else if (tim[idx].jumlahPemain == 0) {
            printf("Belum ada pemain di tim %s.\n", tim[idx].namaTim);
            printf("Tekan enter untuk kembali...");
            getchar();
        } else {
            // Tampilkan daftar pemain tim tersebut
            printf("\nDaftar pemain tim %s:\n", tim[idx].namaTim);
            for (i = 0; i < tim[idx].jumlahPemain; i++) {
                printf("  %d. %s\n", i + 1, tim[idx].pemain[i].nama);
            }

            printf("\nMasukkan nomor pemain yang akan diedit: ");
            scanf("%d", &noPemain);
            getchar();

            // Validasi nomor pemain (harus 1..jumlahPemain)
            if (noPemain < 1 || noPemain > tim[idx].jumlahPemain) {
                printf("Nomor pemain tidak valid.\n");
                printf("Tekan enter untuk kembali...");
                getchar();
            } else {
                // Tampilkan nama lama, minta nama baru
                printf("Nama lama: %s\n", tim[idx].pemain[noPemain - 1].nama);
                printf("Nama baru: ");
                fgets(tim[idx].pemain[noPemain - 1].nama, MAX_NAMA, stdin);
                tim[idx].pemain[noPemain - 1].nama[strcspn(tim[idx].pemain[noPemain - 1].nama, "\n")] = '\0';
                printf("\nData pemain berhasil diubah!\n");
                printf("Tekan enter untuk kembali...");
                getchar();
            }
        }
    }
}

// ==================== FITUR HAPUS PEMAIN ====================

/*
 * hapusPemain
 * ------------
 * Menghapus pemain dari suatu tim.
 * User memilih nomor tim, lalu melihat daftar pemain, memilih nomor pemain,
 * dan konfirmasi. Data pemain di-shift kiri untuk menutup celah.
 * jumlahPemain dikurangi 1.
 */
void hapusPemain() {
    int nomor, idx, noPemain, i;
    char konfirmasi;
    system("cls");
    if (jumlahTim == 0) {
        printf("Belum ada tim terdaftar.\n");
        printf("Tekan enter untuk kembali...");
        getchar();
    } else {
        printf("=================================\n");
        printf("       HAPUS NAMA PEMAIN\n");
        printf("=================================\n");
        printf("Masukkan nomor tim: ");
        scanf("%d", &nomor);
        getchar();

        idx = cariTimByNomor(nomor);
        if (idx == -1) {
            printf("Tim dengan nomor %d tidak ditemukan.\n", nomor);
            printf("Tekan enter untuk kembali...");
            getchar();
        } else if (tim[idx].jumlahPemain == 0) {
            printf("Belum ada pemain di tim %s.\n", tim[idx].namaTim);
            printf("Tekan enter untuk kembali...");
            getchar();
        } else {
            // Tampilkan daftar pemain
            printf("\nDaftar pemain tim %s:\n", tim[idx].namaTim);
            for (i = 0; i < tim[idx].jumlahPemain; i++) {
                printf("  %d. %s\n", i + 1, tim[idx].pemain[i].nama);
            }

            printf("\nMasukkan nomor pemain yang akan dihapus: ");
            scanf("%d", &noPemain);
            getchar();

            if (noPemain < 1 || noPemain > tim[idx].jumlahPemain) {
                printf("Nomor pemain tidak valid.\n");
                printf("Tekan enter untuk kembali...");
                getchar();
            } else {
                // Konfirmasi
                printf("Yakin ingin menghapus %s? (y/n): ", tim[idx].pemain[noPemain - 1].nama);
                scanf("%c", &konfirmasi);
                getchar();

                if (konfirmasi != 'y' && konfirmasi != 'Y') {
                    printf("Penghapusan dibatalkan.\n");
                    printf("Tekan enter untuk kembali...");
                    getchar();
                } else {
                    // Geser pemain setelah index yang dihapus ke kiri (dengan strcpy)
                    for (i = noPemain - 1; i < tim[idx].jumlahPemain - 1; i++) {
                        strcpy(tim[idx].pemain[i].nama, tim[idx].pemain[i + 1].nama);
                    }
                    tim[idx].jumlahPemain--;  // Kurangi jumlah pemain
                    printf("\nData pemain berhasil dihapus!\n");
                    printf("Tekan enter untuk kembali...");
                    getchar();
                }
            }
        }
    }
}

// ==================== FITUR ENTRI JUARA ====================

/*
 * entriJuara
 * -----------
 * Mencatat juara 1, 2, dan 3 dari turnamen.
 * User memasukkan 3 nomor tim yang berbeda.
 * Setiap nomor divalidasi: harus terdaftar dan tidak boleh sama.
 * Jika valid, simpan ke variabel global juara1, juara2, juara3.
 */
void entriJuara() {
    int n1, n2, n3;
    system("cls");
    if (jumlahTim == 0) {
        printf("Belum ada tim terdaftar.\n");
        printf("Tekan enter untuk kembali...");
        getchar();
    } else {
        printf("======================================\n");
        printf("   ENTRI JUARA 1, 2, DAN 3\n");
        printf("======================================\n");
        printf("Pilih nomor tim untuk:\n");

        // Input juara 1
        printf("Juara 1: ");
        scanf("%d", &n1);
        getchar();
        if (cariTimByNomor(n1) == -1) {
            printf("Tim dengan nomor %d tidak ditemukan.\n", n1);
            printf("Tekan enter untuk kembali...");
            getchar();
            return;  // Keluar dari fungsi jika tidak valid
        }

        // Input juara 2
        printf("Juara 2: ");
        scanf("%d", &n2);
        getchar();
        if (cariTimByNomor(n2) == -1) {
            printf("Tim dengan nomor %d tidak ditemukan.\n", n2);
            printf("Tekan enter untuk kembali...");
            getchar();
            return;
        } else if (n2 == n1) {
            // Juara 2 tidak boleh sama dengan juara 1
            printf("Juara 2 harus berbeda dengan juara 1.\n");
            printf("Tekan enter untuk kembali...");
            getchar();
            return;
        }

        // Input juara 3
        printf("Juara 3: ");
        scanf("%d", &n3);
        getchar();
        if (cariTimByNomor(n3) == -1) {
            printf("Tim dengan nomor %d tidak ditemukan.\n", n3);
            printf("Tekan enter untuk kembali...");
            getchar();
            return;
        } else if (n3 == n1 || n3 == n2) {
            // Juara 3 tidak boleh sama dengan juara 1 atau 2
            printf("Juara 3 harus berbeda dengan juara 1 dan 2.\n");
            printf("Tekan enter untuk kembali...");
            getchar();
            return;
        }

        // Semua valid, simpan data juara
        juara1 = n1;
        juara2 = n2;
        juara3 = n3;
        printf("\nData juara berhasil dimasukkan!\n");
        printf("Tekan enter untuk kembali...");
        getchar();
    }
}

// ==================== FITUR EDIT JUARA ====================

/*
 * editJuara
 * ----------
 * Mengedit posisi juara tertentu.
 * User memilih posisi (1/2/3) dan memasukkan nomor tim baru.
 * Validasi: tim baru harus terdaftar dan tidak sedang menjadi juara di posisi lain.
 *
 * Pointer *juaraKe digunakan untuk menunjuk ke variabel global
 * (juara1, juara2, atau juara3) yang sesuai dengan posisi yang dipilih.
 */
void editJuara() {
    int idx1, idx2, idx3;
    int posisi, nomorBaru;
    int *juaraKe;  // Pointer ke variabel juara yang akan diedit
    system("cls");
    if (juara1 == -1 && juara2 == -1 && juara3 == -1) {
        printf("Belum ada data juara.\n");
        printf("Tekan enter untuk kembali...");
        getchar();
    } else {
        printf("=================================\n");
        printf("          EDIT JUARA\n");
        printf("=================================\n");

        // Tampilkan data juara saat ini
        printf("Data juara saat ini:\n");
        idx1 = cariTimByNomor(juara1);
        idx2 = cariTimByNomor(juara2);
        idx3 = cariTimByNomor(juara3);
        if (idx1 != -1) printf("Juara 1: %s (nomor %d)\n", tim[idx1].namaTim, juara1);
        if (idx2 != -1) printf("Juara 2: %s (nomor %d)\n", tim[idx2].namaTim, juara2);
        if (idx3 != -1) printf("Juara 3: %s (nomor %d)\n", tim[idx3].namaTim, juara3);

        printf("\nPilih posisi juara yang akan diedit (1/2/3): ");
        scanf("%d", &posisi);
        getchar();

        if (posisi < 1 || posisi > 3) {
            printf("Posisi tidak valid.\n");
            printf("Tekan enter untuk kembali...");
            getchar();
        } else {
            printf("Masukkan nomor tim baru untuk juara %d: ", posisi);
            scanf("%d", &nomorBaru);
            getchar();

            // Validasi: tim harus terdaftar
            if (cariTimByNomor(nomorBaru) == -1) {
                printf("Tim dengan nomor %d tidak ditemukan.\n", nomorBaru);
                printf("Tekan enter untuk kembali...");
                getchar();
            } else {
                // Arahkan pointer ke variabel juara yang sesuai
                if (posisi == 1)
                    juaraKe = &juara1;
                else if (posisi == 2)
                    juaraKe = &juara2;
                else
                    juaraKe = &juara3;

                // Validasi: tim baru tidak boleh sudah menjadi juara di posisi lain
                // Misal: mengedit juara 2 dengan tim X, dan X sudah jadi juara 1 -> ditolak
                if ((posisi != 1 && nomorBaru == juara1) ||
                    (posisi != 2 && nomorBaru == juara2) ||
                    (posisi != 3 && nomorBaru == juara3)) {
                    printf("Tim tersebut sudah menjadi juara di posisi lain.\n");
                    printf("Tekan enter untuk kembali...");
                    getchar();
                } else {
                    *juaraKe = nomorBaru;  // Ubah nilai melalui pointer
                    printf("\nData juara berhasil diubah!\n");
                    printf("Tekan enter untuk kembali...");
                    getchar();
                }
            }
        }
    }
}

// ==================== FITUR HAPUS JUARA ====================

/*
 * hapusJuara
 * -----------
 * Menghapus data juara berdasarkan posisi yang dipilih (1/2/3),
 * atau menghapus semua data juara sekaligus (pilihan 0).
 * Menyetel variabel juara ke -1 (tanda tidak ada data).
 */
void hapusJuara() {
    int posisi;
    system("cls");
    if (juara1 == -1 && juara2 == -1 && juara3 == -1) {
        printf("Belum ada data juara.\n");
        printf("Tekan enter untuk kembali...");
        getchar();
    } else {
        printf("=================================\n");
        printf("       HAPUS DATA JUARA\n");
        printf("=================================\n");
        printf("Pilih posisi juara yang akan dihapus (1/2/3), atau 0 untuk menghapus semua: ");
        scanf("%d", &posisi);
        getchar();

        if (posisi == 0) {
            // Hapus semua juara
            juara1 = -1;
            juara2 = -1;
            juara3 = -1;
            printf("Semua data juara berhasil dihapus!\n");
        } else if (posisi >= 1 && posisi <= 3) {
            // Hapus juara di posisi tertentu
            if (posisi == 1)
                juara1 = -1;
            else if (posisi == 2)
                juara2 = -1;
            else
                juara3 = -1;
            printf("Data juara %d berhasil dihapus!\n", posisi);
        } else {
            printf("Posisi tidak valid.\n");
        }

        printf("Tekan enter untuk kembali...");
        getchar();
    }
}

// ==================== FITUR TAMPILAN (LIHAT DATA) ====================

/*
 * tampilSemuaTim
 * ---------------
 * Menampilkan daftar semua tim beserta manajernya.
 * Output format: [nomor] NamaTim, Manajer: namaManajer
 */
void tampilSemuaTim() {
    int i;
    system("cls");
    printf("========================================\n");
    printf("     SEMUA TIM DAN MANAJER\n");
    printf("========================================\n");
    if (jumlahTim == 0) {
        printf("Belum ada tim terdaftar.\n");
    } else {
        // Loop seluruh array tim dan tampilkan data
        for (i = 0; i < jumlahTim; i++) {
            printf("  [%d] %s\n", tim[i].nomor, tim[i].namaTim);
            printf("      Manajer: %s\n", tim[i].manajer);
            printf("  -----------------------------\n");
        }
    }
    printf("\nTekan enter untuk kembali...");
    getchar();
}

/*
 * tampilSemuaPemain
 * ------------------
 * Menampilkan semua pemain dari semua tim yang terdaftar.
 * Format: nama pemain diikuti nama tim-nya.
 * totalPemain menghitung jumlah seluruh pemain (untuk cek jika 0).
 */
void tampilSemuaPemain() {
    int i, j, totalPemain;
    system("cls");
    printf("========================================\n");
    printf("     SEMUA PEMAIN DAN TIM\n");
    printf("========================================\n");
    if (jumlahTim == 0) {
        printf("Belum ada tim terdaftar.\n");
    } else {
        totalPemain = 0;
        for (i = 0; i < jumlahTim; i++) {
            if (tim[i].jumlahPemain > 0) {
                // Loop setiap pemain di dalam tim
                for (j = 0; j < tim[i].jumlahPemain; j++) {
                    printf("  Pemain : %s\n", tim[i].pemain[j].nama);
                    printf("  Tim    : %s\n", tim[i].namaTim);
                    printf("  -----------------------------\n");
                    totalPemain++;
                }
            }
        }
        if (totalPemain == 0) {
            printf("Belum ada pemain terdaftar.\n");
        }
    }
    printf("\nTekan enter untuk kembali...");
    getchar();
}

/*
 * tampilPemainTim
 * ----------------
 * Menampilkan daftar pemain dari satu tim tertentu.
 * User memasukkan nomor tim, lalu sistem menampilkan
 * nama tim, manajer, dan daftar pemain (atau "Belum ada pemain").
 */
void tampilPemainTim() {
    int nomor, idx, i;
    system("cls");
    if (jumlahTim == 0) {
        printf("Belum ada tim terdaftar.\n");
        printf("Tekan enter untuk kembali...");
        getchar();
    } else {
        printf("========================================\n");
        printf("     PEMAIN PADA TIM TERTENTU\n");
        printf("========================================\n");
        printf("Masukkan nomor tim: ");
        scanf("%d", &nomor);
        getchar();

        idx = cariTimByNomor(nomor);
        if (idx == -1) {
            printf("Tim dengan nomor %d tidak ditemukan.\n", nomor);
            printf("Tekan enter untuk kembali...");
            getchar();
        } else {
            printf("\nTim: %s\n", tim[idx].namaTim);
            printf("Manajer: %s\n", tim[idx].manajer);
            printf("Pemain:\n");
            if (tim[idx].jumlahPemain == 0) {
                printf("  (Belum ada pemain)\n");
            } else {
                for (i = 0; i < tim[idx].jumlahPemain; i++) {
                    printf("  %d. %s\n", i + 1, tim[idx].pemain[i].nama);
                }
            }
            printf("\nTekan enter untuk kembali...");
            getchar();
        }
    }
}

/*
 * tampilJuara
 * ------------
 * Menampilkan data juara 1, 2, dan 3 (jika ada).
 * Mencari nama tim berdasarkan nomor yang tersimpan di variabel
 * juara1, juara2, juara3.
 * Jika data tim sudah dihapus, tampilkan "(data tim tidak ditemukan)".
 */
void tampilJuara() {
    int idx;
    system("cls");
    printf("========================================\n");
    printf("       JUARA 1, 2, DAN 3\n");
    printf("========================================\n");
    if (juara1 == -1 && juara2 == -1 && juara3 == -1) {
        printf("Belum ada data juara.\n");
    } else {
        // Tampilkan juara 1 jika ada
        if (juara1 != -1) {
            idx = cariTimByNomor(juara1);
            printf("  Juara 1: %s {nomor %d}\n",
                   idx != -1 ? tim[idx].namaTim : "(data tim tidak ditemukan)",
                   juara1);
        }
        // Tampilkan juara 2 jika ada
        if (juara2 != -1) {
            idx = cariTimByNomor(juara2);
            printf("  Juara 2: %s {nomor %d}\n",
                   idx != -1 ? tim[idx].namaTim : "(data tim tidak ditemukan)",
                   juara2);
        }
        // Tampilkan juara 3 jika ada
        if (juara3 != -1) {
            idx = cariTimByNomor(juara3);
            printf("  Juara 3: %s {nomor %d}\n",
                   idx != -1 ? tim[idx].namaTim : "(data tim tidak ditemukan)",
                   juara3);
        }
    }
    printf("\nTekan enter untuk kembali...");
    getchar();
}

// ==================== MENU UTAMA ====================

/*
 * menuUtama
 * ----------
 * Menampilkan menu interaktif ke layar dan memproses pilihan user.
 * Menggunakan loop do-while agar menu terus muncul hingga user memilih 0 (keluar).
 * Setiap pilihan akan memanggil fungsi yang sesuai via switch-case.
 *
 * Pilihan menu:
 *   1  - Entri Data Tim
 *   2  - Edit Data Tim
 *   3  - Hapus Data Tim
 *   4  - Entri Nama Pemain
 *   5  - Edit Nama Pemain
 *   6  - Hapus Nama Pemain
 *   7  - Entri Juara 1, 2, dan 3
 *   8  - Edit Juara
 *   9  - Hapus Juara
 *   10 - Tampilkan Semua Tim dan Manajer
 *   11 - Tampilkan Semua Pemain dan Tim
 *   12 - Tampilkan Pemain pada Tim Tertentu
 *   13 - Tampilkan Juara 1, 2, dan 3
 *   0  - Keluar
 */
void menuUtama() {
    int pilihan;
    do {
        system("cls");  // Bersihkan layar setiap kali loop
        printf("====================================\n");
        printf("    PENDATAAN SEPAK TAKRAW D.I.Y\n");
        printf("====================================\n");
        printf("  1. Entri Data Tim\n");
        printf("  2. Edit Data Tim\n");
        printf("  3. Hapus Data Tim\n");
        printf("  4. Entri Nama Pemain\n");
        printf("  5. Edit Nama Pemain\n");
        printf("  6. Hapus Nama Pemain\n");
        printf("  7. Entri Juara 1, 2, dan 3\n");
        printf("  8. Edit Juara\n");
        printf("  9. Hapus Juara\n");
        printf(" 10. Tampilkan Semua Tim dan Manajer\n");
        printf(" 11. Tampilkan Semua Pemain dan Tim\n");
        printf(" 12. Tampilkan Pemain pada Tim Tertentu\n");
        printf(" 13. Tampilkan Juara 1, 2, dan 3\n");
        printf("  0. Keluar\n");
        printf("====================================\n");
        printf("Pilihan: ");
        scanf("%d", &pilihan);  // Baca pilihan integer
        getchar();              // Konsumsi newline

        // Proses pilihan user
        switch (pilihan) {
            case 1:
                entriTim();       // Entri data tim baru
                break;
            case 2:
                editTim();        // Edit data tim yang ada
                break;
            case 3:
                hapusTim();       // Hapus data tim
                break;
            case 4:
                entriPemain();    // Entri pemain ke tim
                break;
            case 5:
                editPemain();     // Edit nama pemain
                break;
            case 6:
                hapusPemain();    // Hapus pemain dari tim
                break;
            case 7:
                entriJuara();     // Entri juara 1, 2, 3
                break;
            case 8:
                editJuara();      // Edit posisi juara
                break;
            case 9:
                hapusJuara();     // Hapus data juara
                break;
            case 10:
                tampilSemuaTim(); // Tampilkan semua tim
                break;
            case 11:
                tampilSemuaPemain(); // Tampilkan semua pemain
                break;
            case 12:
                tampilPemainTim();   // Tampilkan pemain tim tertentu
                break;
            case 13:
                tampilJuara();    // Tampilkan juara
                break;
            case 0:
                // Keluar program
                printf("\nTerima kasih! Program selesai.\n");
                break;
            default:
                // Pilihan tidak dikenali
                printf("Pilihan tidak valid!\n");
                printf("Tekan enter untuk kembali...");
                getchar();
        }
    } while (pilihan != 0);  // Ulangi sampai user memilih 0 (keluar)
}

// ==================== FUNGSI UTAMA (ENTRY POINT) ====================

/*
 * main
 * ----
 * Fungsi utama program. Titik masuk pertama saat program dijalankan.
 * Cukup memanggil menuUtama() untuk memulai antarmuka pengguna,
 * lalu mengembalikan 0 ke sistem operasi (menandakan sukses).
 */
int main() {
    menuUtama();   // Panggil menu utama — program berjalan di sini
    return 0;      // Kembalikan 0 sebagai exit code sukses
}
