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
int jumlahTim = 0;
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

void entriTim() {
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

        printf("Nama tim: ");
        fflush(stdin);
        gets(tim[jumlahTim].namaTim);

        printf("Nama manajer: ");
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

void editTim() {
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
            printf("Nama tim: %s\n", tim[idx].namaTim);
            printf("Manajer: %s\n", tim[idx].manajer);

            printf("\nMasukkan data baru:\n");
            printf("Nama tim: ");
            fflush(stdin);
            gets(tim[idx].namaTim);
            printf("Nama manajer: ");
            fflush(stdin);
            gets(tim[idx].manajer);

            printf("\nData tim berhasil diubah!\n");
            printf("Tekan enter untuk kembali...");
            getch();
        }
    }
}

void tampilSemuaTim() {
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

void hapusTim() {
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
        } else  {
            printf("\nData tim yang akan dihapus:\n");
            printf("Nama tim: %s\n", tim[idx].namaTim);
            printf("Manajer: %s\n", tim[idx].manajer);
            printf("Pemain: %d orang\n", tim[idx].jumlahPemain);

            printf("\nYakin ingin menghapus? (y/n): ");
            fflush(stdin);
            scanf("%c", &konfirmasi);
            if (konfirmasi == 'n' || konfirmasi == 'N'){
            	printf("[!]Tim Batal Dihapus[!]");
			}
            getch();
        }
    }
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

int main() {
    int pilih;

    do {
        menu();
		printf("Pilih Menu : ");
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
        }
    } while (pilih != 0);
    return 0;
}
