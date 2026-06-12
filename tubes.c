#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TIM 10
#define MAX_PEMAIN 3

typedef char string[100];

typedef struct {
    string nama;
} Pemain;

typedef struct {
    int nomor;
    string namaTim;
    string manajer;
    Pemain pemain[MAX_PEMAIN];
    int jumlahPemain;
} Tim;

Tim tim[MAX_TIM];
Pemain pemain[MAX_PEMAIN];
int jumlahTim = 0;
int jumlahPemain = 0;
int juara1 = -1, juara2 = -1, juara3 = -1;
int nextNomor = 1;

int cariTimByNomor(int nomor) {
    int i;
    for (i = 0; i < jumlahTim; i++) {
        if (tim[i].nomor == nomor)
            return i;
    }
    return -1;
}

int cariTimByNama(string nama) {
    int i;
    for (i = 0; i < jumlahTim; i++) {
        if (strcmp(tim[i].namaTim, nama) == 0)
            return i;
    }
    return -1;
}

void entriTim() {  // No(01)
    system("cls");
    if (jumlahTim >= MAX_TIM) {
        printf("Maaf, maksimum tim sudah tercapai.\n");
        printf("Tekan enter untuk kembali...");
        getch();
        getch();
    } else {
        printf("=================================\n");
        printf("       ENTRI DATA TIM\n");
        printf("=================================\n");
        printf("Nomor tim: %d (otomatis)\n", nextNomor);
        tim[jumlahTim].nomor = nextNomor;

        printf("Nama tim      : ");
        fflush(stdin);
        gets(tim[jumlahTim].namaTim);

        printf("Nama manajer  : ");
        fflush(stdin);
        gets(tim[jumlahTim].manajer);

        tim[jumlahTim].jumlahPemain = 0;
        nextNomor++;
        jumlahTim++;

        printf("\nData tim berhasil ditambahkan!\n");
        printf("Tekan enter untuk kembali...");
        getch();
    }
}

void editTim() {  // No(02)
    int nomor, idx;
    system("cls");
    if (jumlahTim == 0) {
        printf("Belum ada tim terdaftar.\n");
        printf("Tekan enter untuk kembali...");
        getch();
    } else {
        printf("=================================\n");
        printf("        EDIT DATA TIM\n");
        printf("=================================\n");
        printf("Masukkan nomor tim yang akan diedit: ");
        scanf("%d", &nomor);
        getch();

        idx = cariTimByNomor(nomor);
        if (idx == -1) {
            printf("Tim dengan nomor %d tidak ditemukan.\n", nomor);
            printf("Tekan enter untuk kembali...");
            getch();
        } else {
            printf("\n--- Data lama ---\n");
            printf("Nama tim   : %s\n", tim[idx].namaTim);
            printf("Manajer    : %s\n", tim[idx].manajer);

            printf("\nMasukkan data baru :\n");
            printf("Nama tim     : ");
            fflush(stdin);
            gets(tim[idx].namaTim);
            printf("Nama manajer : ");
            fflush(stdin);
            gets(tim[idx].manajer);

            printf("\nData tim berhasil diubah!\n");
            printf("Tekan enter untuk kembali...");
            getch();
        }
    }
}

void tampilSemuaTim() {  // No(03)
    int i;
    system("cls");
    printf("========================================\n");
    printf("     SEMUA TIM DAN MANAJER\n");
    printf("========================================\n");
    if (jumlahTim == 0) {
        printf("Belum ada tim terdaftar.\n");
    } else {
        for (i = 0; i < jumlahTim; i++) {
            printf("--TIM [%d]--\n", tim[i].nomor);
            printf("%s\n", tim[i].namaTim);
            printf("Manajer: %s\n", tim[i].manajer);
            printf("-----------------------------\n");
        }
    }
    printf("\nTekan enter untuk kembali...");
    getch();
}

void hapusTim() {  // No(04)
    int nomor, idx, i;
    char konfirmasi;
    system("cls");
    if (jumlahTim == 0) {
        printf("Belum ada tim terdaftar.\n");
        printf("Tekan enter untuk kembali...");
        getch();
    } else {
        printf("=================================\n");
        printf("       HAPUS DATA TIM\n");
        printf("=================================\n");
        printf("Masukkan nomor tim yang akan dihapus: ");
        scanf("%d", &nomor);
        getch();

        idx = cariTimByNomor(nomor);
        if (idx < 0 || idx > 10) {
            printf("Tim dengan nomor %d tidak ditemukan.\n", nomor);
            printf("Tekan enter untuk kembali...");
            getch();
        } else {
            printf("\nData tim yang akan dihapus:\n");
            printf("Nama tim: %s\n", tim[idx].namaTim);
            printf("Manajer: %s\n", tim[idx].manajer);
            printf("Pemain: %d orang\n", tim[idx].jumlahPemain);

            printf("\nYakin ingin menghapus? (y/n): ");
            fflush(stdin);
            scanf("%c", &konfirmasi);
            if (konfirmasi == 'n' || konfirmasi == 'N') {
                printf("[!]Tim Batal Dihapus[!]");
            }
            getch();
        }
    }
}

void entriPemain() {  // No(05)
    int nomor, idx;
    char konfirmasi;

    system("cls");
    if (jumlahTim == 0) {
        printf("Belum ada tim terdaftar. Daftarkan tim terlebih dahulu.\n");
    } else {
        printf("=================================\n");
        printf("       ENTRI NAMA PEMAIN\n");
        printf("=================================\n");
        printf("Masukkan nomor tim: ");
        scanf("%d", &nomor);

        idx = cariTimByNomor(nomor);
        printf("\ntim nya adalah : %s", tim[idx].namaTim);
        printf("\n-----------------------------\n");

        //konfirmasi jumlah pemain apakah sudah penuh atau tidak
        if (tim[idx].jumlahPemain >= MAX_PEMAIN) {
            printf("\n[!] Maaf, jumlah pemain untuk tim %s sudah penuh (Maks %d). [!]\n", tim[idx].namaTim, MAX_PEMAIN);
        }

        // cek apakah tim dengan nomor yang dimasukkan user terdaftar
        if (idx < 1 && idx > MAX_TIM) {
            printf("Tim dengan nomor %d tidak ditemukan.\n", nomor);
        } else {
            do {
                int idxPemain = tim[idx].jumlahPemain;
                printf("\nNama pemain ke-%d: ", idxPemain + 1);

                // pertanyaan konfirmasi untuk menambah jumlah tim yang ingin ditambahkan dihapus, langsung saja

                fflush(stdin);
                gets(tim[idx].pemain[idxPemain].nama);

                tim[idx].jumlahPemain++;

            } while (tim[idx].jumlahPemain < MAX_PEMAIN);
            printf("\nPengisian pemain untuk tim %s selesai.\n", tim[idx].namaTim);
        }
    }
    printf("Tekan enter untuk kembali...");
    getch();
}

void editPemain() {  // No(06)
    int i, idx, nomor, noPemain;
    char konfirmasi;
    system("cls");
    printf("=================================\n");
    printf("       ENTRI NAMA PEMAIN\n");
    printf("=================================\n");
    printf("\nMasukkan nomor Tim: ");
    scanf("%d", &nomor);

    idx = cariTimByNomor(nomor);
    if (idx < 1 && idx > 10) {
        printf("\nTim Tidak Ditemukan.\n");
    } else if (tim[idx].jumlahPemain == 0) {
        printf("Tim %s belum memiliki pemain.\n", tim[idx].namaTim);
    } else {
        do {
            printf("\nDaftar Pemain Tim %s:\n", tim[idx].namaTim);
            for (i = 0; i < tim[idx].jumlahPemain; i++) {
                printf("%d, %s\n", i + 1, tim[idx].pemain[i].nama);
            }

            printf("Pilih Nomor Pemain yang Ingin DIedit (1-%d): ", tim[idx].jumlahPemain);

            if (scanf("%d", &noPemain) != 1) {
                printf("[!] Error, Inputan bukan Nomor [!]");
            }

            if (noPemain < 1 || noPemain > tim[idx].jumlahPemain) {
                printf("Nomor Pemain tidak Valid\n");
            } else {
                printf("Masukkan Nama Baru: ");
                fflush(stdin);
                gets(tim[idx].pemain[noPemain - 1].nama);
                printf("Ingin edit Nama Pemain Lainnya? [Y/N]: ");
                fflush(stdin);
                scanf("%c", &konfirmasi);
            }
        } while (konfirmasi == 'y' || konfirmasi == 'Y');
        printf("\nPengeditan pemain untuk tim %s selesai.\n", tim[idx].namaTim);
    }
    printf("Tekan enter untuk kembali...");
    getch();
}

void hapusPemain() {  // No(07)
    int i, idx, nomor, noPemain;
    char konfirmasi;
    system("cls");
    printf("=================================\n");
    printf("       HAPUS NAMA PEMAIN\n");
    printf("=================================\n");
    printf("\nMasukkan nomor Tim: ");
    scanf("%d", &nomor);

    idx = cariTimByNomor(nomor);
    if (idx == -1) {
        printf("\nTim Tidak Ditemukan.\n");
    } else if (tim[idx].jumlahPemain == 0) {
        printf("Tim %s belum memiliki pemain.\n", tim[idx].namaTim);
    } else {
        do {
            printf("\nDaftar Pemain Tim %s:\n", tim[idx].namaTim);
            for (i = 0; i < tim[idx].jumlahPemain; i++) {
                printf("%d, %s\n", i + 1, tim[idx].pemain[i].nama);
            }

            printf("Pilih Nomor Pemain yang Ingin Dihapus (1-%d): ", tim[idx].jumlahPemain);

            if (scanf("%d", &noPemain) != 1) {
                printf("[!] Error, Inputan bukan Nomor [!]");
            }

            if (noPemain < 1 || noPemain > tim[idx].jumlahPemain) {
                printf("Nomor Pemain tidak Valid\n");
            } else {
                for (i = noPemain - 1; i < tim[idx].jumlahPemain; i++) {
                    strcpy(tim[idx].pemain[i].nama, tim[idx].pemain[i + 1].nama);
                }
                tim[idx].jumlahPemain--;
                printf("\nPemain Berhasil Dihapus.\n");
                printf("Ingin edit Nama Pemain Lainnya? [Y/N]: ");
                fflush(stdin);
                scanf("%c", &konfirmasi);
            }
        } while (konfirmasi == 'y' || konfirmasi == 'Y');
        printf("\nProses Penghapusan untuk tim %s Selesai.\n", tim[idx].namaTim);
    }
    printf("Tekan enter untuk kembali...");
    getch();
}

void tampilPemainTim() {  // No(08)
    int i, j;
    system("cls");
    printf("========================================\n");
    printf("          SEMUA PEMAIN DAN TIM\n");
    printf("========================================\n");

    if (jumlahTim == 0) {
        printf("Belum ada tim terdaftar.\n");
    } else {
        for (i = 0; i < jumlahTim; i++) {
            printf("\n--TIM [%d]--\n", tim[i].nomor);
            printf("Nama Tim: %s\n", tim[i].namaTim);
            printf("Manajer: %s\n", tim[i].manajer);
            printf("Daftar Pemain:\n");

            if (tim[i].jumlahPemain == 0) {
                printf("Belum ada Pemain...\n");
            } else {
                for (j = 0; j < tim[i].jumlahPemain; j++) {
                    printf("%d, %s\n", j + 1, tim[i].pemain[j].nama);
                }
            }
            printf("----------------------------------------\n");
        }
    }
    printf("Tekan enter untuk kembali...");
    getch();
}

void menu() {
    printf("====================================\n");
    printf("    PENDATAAN SEPAK TAKRAW D.I.Y\n");
    printf("====================================\n");
    printf("  1. Entri Data Tim\n");
    printf("  2. Tampilkan Semua Tim dan Manajer\n");
    printf("  3. Edit Data Tim\n");
    printf("  4. Hapus Data Tim\n");
    printf("  5. Entri Nama Pemain\n");
    printf("  6. Edit Nama Pemain\n");
    printf("  7. Hapus Nama Pemain\n");
    printf("  8. Tampilkan Semua Pemain dan Tim\n");
    printf("  9. Tampilkan Pemain pada Tim Tertentu (ganjil) \n");  // ganjil
    printf("  10. Entri Juara 1, 2, dan 3\n");
    printf("  11. Edit Juara\n");
    printf("  12. Hapus Juara\n");
    printf("  13. Tampilkan Juara 1, 2, dan 3\n");
    printf("  0. Keluar\n");
    printf("====================================\n");
}

//==================================================================================

int main() {
    int pilih;
    do {
        system("cls");
        menu();
        scanf("%d", &pilih);
        system("cls");
        switch (pilih) {
            case 1: {
                entriTim();
                system("cls");
                break;
            }
            case 2: {
                tampilSemuaTim();
                system("cls");
                break;
            }

            case 3: {
                editTim();
                system("cls");
                break;
            }

            case 4: {
                hapusTim();
                system("cls");
                break;
            }

            case 5: {
                entriPemain();
                system("cls");
                break;
            }

            case 6: {
                editPemain();
                system("cls");
                break;
            }

            case 7: {
                hapusPemain();
                system("cls");
                break;
            }

            case 8: {
                tampilPemainTim();
                system("cls");
                break;
            }

            case 0: {
                printf("Program Selesai...\n\n");
                printf("Anggota Kelompok:\n");
                printf("Yosef Kelfian P [250713171]\n");
                printf("Gebriella Wibie N [250713194]\n");
                printf("Stefanus Kevin Oktaf K [250713192]\n");
                break;
            }

            default: {
                printf("\n\t[!] Menu Tidak Tersedia [!]");
                getch();
                break;
            }
        }
    } while (pilih != 0);

    return 0;
}
